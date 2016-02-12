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
    for(int i=0;i<ClientsList.size();i++)
    {
        validClient* nClient = ClientsList[i];
        delete nClient;
        ClientsList.removeAt(i);
    }
    serverd->close();
    delete serverd;
}

bool ATCPServer::launch(int port)
{
    if(!serverd->listen(QHostAddress::Any,port))
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
            connect(newServerThread, SIGNAL(CloseClient(int)),
                        this , SLOT(CloseClient(int))
                       , Qt::QueuedConnection);
            connect(newServerThread, SIGNAL(sendToClient(int, QString)),
                        this , SLOT(sendToClient(int, QString))
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
    validClient* h = NewValidClient();
    h->socket = pClientSocket;
    h->isAuth = false;
    h->isUseCommand = false;
    h->numUsingCommands = 0;
    h->state = NoAuthState;
    ClientsList << h;
    qDebug() << "Client connect "+pClientSocket->peerAddress().toString()+":"+QString::number(pClientSocket->peerPort());
    sendToClient(pClientSocket, "Server Response: Connected!\n");
}
void ATCPServer::clientDisconnect()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    for(int i=0;i<ThreadList.size();i++)
    {
        ArrayCommand t;
        t.client = socket;
        while(ThreadList[i]->ArrayCommands.removeOne(t)) {
        }
    }
    int mClientID = GetIDClient(socket);
    if(mClientID > -1 && mClientID < ClientsList.size()) {
    if(!ClientsList.value(mClientID)->isUseCommand){
        DelValidClient( ClientsList[mClientID] );
        ClientsList.removeAt(mClientID);
        socket->deleteLater();
    }
    else
    {
        ClientsList[mClientID]->state = WaitCliseInClient;
    }}
    qDebug() << "Client disconnect";
}
validClient* ATCPServer::getClient(QTcpSocket* socket)
{
    validClient* result;
    for(int i=0;i<ClientsList.size();i++)
    {
        if(ClientsList.value(i)->socket == socket) result = ClientsList[i];
    }
    return result;
}
int ATCPServer::GetIDClient(QTcpSocket* socket)
{
    int result;
    for(int i=0;i<ClientsList.size();i++)
    {
        if(ClientsList.value(i)->socket == socket) result = i;
    }
    return result;
}

void ATCPServer::clientReadyRead()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    QByteArray data = socket->readAll();
    ArrayCommand cmd;
    cmd.client = socket;
    cmd.command = data;
    int ThreadID  = MaxThread+1;
    int MinCommands=INT_MAX;
    for(int i=0;i<ThreadList.size();i++)
    {
        if(ThreadList.value(i)->ArrayCommands.size()==0)
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
    ThreadList.value(ThreadID)->ArrayCommands << cmd;
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
void ATCPServer::sendToClient(int clientID, QString str)
{
    validClient* n = ClientsList.value(clientID);
    if(n->state != WaitCliseInClient)
    ClientsList.value(clientID)->socket->write(str.toUtf8());
}
void ATCPServer::CloseClient(int clientID)
{
    validClient* n = ClientsList.value(clientID);
    if(n->state != WaitCliseInClient)
    ClientsList.value(clientID)->socket->close();
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
        UseCommand();
        if(!ArrayCommands.isEmpty()) UseCommand();
        else isSleep=true;
    }
}

void ServerThread::UseCommand()
{
    isSleep=false;
    if(ArrayCommands.isEmpty())
    {
        return;
    }
    ArrayCommand sCommand = ArrayCommands.takeFirst();
    int mClientID= server->GetIDClient(sCommand.client);
    validClient* nClient = server->ClientsList.value(mClientID);
    nClient->isUseCommand = true;
    nClient->numUsingCommands++;
    server->UseCommand(sCommand,nClient,mClientID,this);
    nClient->isUseCommand = false;
    nClient->numUsingCommands--;
    if(nClient->state == WaitCliseInClient && nClient->numUsingCommands == 0)
    {
        server->DelValidClient( server->ClientsList[mClientID] );
        server->ClientsList.removeAt(mClientID);
        sCommand.client->deleteLater();
    }
}
