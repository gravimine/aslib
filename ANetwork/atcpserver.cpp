#include "atcpserver.h"

ATCPServer::ATCPServer()
{
    serverd = new QTcpServer(this);

    connect(serverd, SIGNAL(newConnection()),
                this,         SLOT(clientConnected())
               );


}


ATCPServer::~ATCPServer()
{
    for(int i=0;i<ThreadList.size();i++)
    {
        ServerThread* thread = ThreadList[i];
        thread->exit(0);
        thread->deleteLater();
        ThreadList.removeAt(i);
    }
    for(QLinkedList<validClient*>::iterator i=ClientsList.begin();i!=ClientsList.end();i++)
    {
        validClient* nClient = (*i);
        delete nClient;
        ClientsList.erase(i);
    }
    serverd->close();
    delete serverd;
}

bool ATCPServer::launch(QHostAddress host, int port)
{
    if(!serverd->listen(QHostAddress(host),port))
    {
        qDebug() << "Bad listen(): "+serverd->errorString();
        return false;
    }
    else {
        qDebug() << "Server started";
        //Создание потоков
        for(int i=0;i<MinThread;i++)
        {
            ServerThread * newServerThread = new ServerThread(this);
            newServerThread->currentIdThread = i;
            newServerThread->isStaticThread = true;
            connect(this, SIGNAL(signalCommand(int)),
                        newServerThread , SLOT(NewCommand(int))
                       , Qt::QueuedConnection);
            connect(newServerThread, SIGNAL(CloseClient(validClient*)),
                        this , SLOT(CloseClient(validClient*))
                       , Qt::QueuedConnection);
            connect(newServerThread, SIGNAL(sendToClient(validClient*, QString)),
                        this , SLOT(sendToClient(validClient*, QString))
                       , Qt::QueuedConnection);
            connect(newServerThread, SIGNAL(sendToClient(QTcpSocket*, QString)),
                        this , SLOT(sendToClient(QTcpSocket*, QString))
                       , Qt::QueuedConnection);
            newServerThread->start();
            ThreadList << newServerThread;
        }
        return true;
    }
}
void ATCPServer::clientConnected()
{
    QTcpSocket* pClientSocket = serverd->nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()),
                this , SLOT(clientDisconnect())
               );
        connect(pClientSocket, SIGNAL(readyRead()),
                this,          SLOT(clientReadyRead())
               );
        pClientSocket->setReadBufferSize(128000);
    validClient* h = NewValidClient();
    h->socket = pClientSocket;
    h->isAuth = false;
    h->isUseCommand = false;
    h->numUsingCommands = 0;
    h->state = NoAuthState;
    ClientsList << h;
    qDebug() << "Client connect "+pClientSocket->peerAddress().toString()+":"+QString::number(pClientSocket->peerPort());
    sendToClient(pClientSocket, ClientInConnectText);
}
void ATCPServer::clientDisconnect()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    for(int i=0;i<ThreadList.size();i++)
    {
        ArrayCommand t;
        t.client = socket;
        ThreadList[i]->arrayMutex.lock();
        ThreadList[i]->ArrayCommands.removeAll(t);
        ThreadList[i]->arrayMutex.unlock();
    }
    QLinkedList<validClient*>::iterator mClientID = GetIDClient(socket);
    if(!((*mClientID)->isUseCommand)){
        DelValidClient( (*mClientID) );
        ClientsList.erase(mClientID);
        socket->deleteLater();
    }
    else
    {
        (*mClientID)->state = WaitCliseInClient;
    }
    qDebug() << "Client disconnect";
}
validClient* ATCPServer::getClient(QTcpSocket* socket)
{
    validClient* result = NULL;
    for(QLinkedList<validClient*>::iterator i=ClientsList.begin();i!=ClientsList.end();i++)
    {
        if((*i)->socket == socket) result = (*i);
    }
    return result;
}
QLinkedList<validClient*>::iterator ATCPServer::GetIDClient(QTcpSocket* socket)
{
    QLinkedList<validClient*>::iterator result=NULL;
    for(QLinkedList<validClient*>::iterator i=ClientsList.begin();i!=ClientsList.end();i++)
    {
        if((*i)->socket == socket) result = i;
    }
    return result;
}

void ATCPServer::clientReadyRead()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    QByteArray data = socket->read(socket->bytesAvailable());
    if(data.isEmpty()) return;
    ArrayCommand cmd;
    cmd.client = socket;
    cmd.command = data;
    int ThreadID  = MaxThread+1;
    int MinCommands=INT_MAX;
    for(int i=0;i<ThreadList.size();i++)
    {
        if(ThreadList.value(i)->isSleep)
        {
            MinCommands=0;
            ThreadID = i;
            break;
        }
        else if(ThreadList.value(i)->ArrayCommands.size()<MinCommands)
        {
            MinCommands=ThreadList.value(i)->ArrayCommands.size();
            ThreadID = i;
        }
    }
    ThreadList.value(ThreadID)->arrayMutex.lock();
    ThreadList.value(ThreadID)->ArrayCommands << cmd;
    ThreadList.value(ThreadID)->arrayMutex.unlock();
    if(MinCommands==0) signalCommand(ThreadID);
    /*ClientsList[mClientID].data+=data;
    QString realData = ClientsList[mClientID].data;
    if(realData.mid(realData.size()-2) != "\n\n")
    {
        return;
    }
    ClientsList[mClientID].data.clear();*/
}
void ATCPServer::sendToClient(QTcpSocket* socket, QString str)
{
    socket->write(str.toUtf8());
}
void ATCPServer::sendToClient(validClient *clientID, QString str)
{
    validClient* n = clientID;
    if(n->state != WaitCliseInClient){
    n->socket->write(str.toUtf8());}
}
void ATCPServer::CloseClient(validClient *clientID)
{
    validClient* n = clientID;
    if(n->state != WaitCliseInClient)
    n->socket->close();
}
ServerThread::ServerThread(ATCPServer *serv)
{
    server=serv;
    isSleep = true;
    moveToThread(this);
}

void ServerThread::run()
{
    if(!isStaticThread) UseCommand();
    else exec();
}
void ServerThread::NewCommand(int idThread)
{
    if(idThread == currentIdThread)
    {
        isSleep=false;
        while(!ArrayCommands.isEmpty()){
        UseCommand();}
        isSleep=true;
    }
}

void ServerThread::UseCommand()
{
    if(ArrayCommands.isEmpty())
    {
        return;
    }
    arrayMutex.lock();
    ArrayCommand sCommand = ArrayCommands.takeFirst();
    arrayMutex.unlock();

    QLinkedList<validClient*>::iterator mClientID= server->GetIDClient(sCommand.client);
    validClient* nClient = (*mClientID);
    nClient->isUseCommand = true;
    nClient->numUsingCommands++;
    server->UseCommand(sCommand.command,nClient,mClientID,this);
    nClient->isUseCommand = false;
    nClient->numUsingCommands--;
    if(nClient->state == WaitCliseInClient && nClient->numUsingCommands == 0)
    {
        server->DelValidClient( nClient );
        server->ClientsList.erase(mClientID);
        nClient->socket->deleteLater();
        delete nClient;
    }
}
