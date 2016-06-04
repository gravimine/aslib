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
        //Создание потоков
        for(int i=0;i<MinThread;i++)
        {
            ServerThread * newServerThread = new ServerThread(this);
            newServerThread->currentIdThread = i;
            newServerThread->isStaticThread = true;
            connect(this, &ATCPServer::signalCommand,newServerThread ,
                         &ServerThread::NewCommand);
            connect(newServerThread, &ServerThread::CloseClient, this,
                        &ATCPServer::CloseClient);
            connect(newServerThread, &ServerThread::sendToClient, this,
                        &ATCPServer::sendToClient);
            newServerThread->start();
            ThreadList << newServerThread;
        }
        return true;
    }
}
void ATCPServer::clientConnected()
{
    QTcpSocket* pClientSocket = serverd->nextPendingConnection();
    connect(pClientSocket, &QTcpSocket::disconnected,
                this , &ATCPServer::clientDisconnect
               );
        connect(pClientSocket, &QTcpSocket::readyRead,
                this,          &ATCPServer::clientReadyRead
               );
        //pClientSocket->setReadBufferSize(128000);
    validClient* h = NewValidClient();
    h->socket = pClientSocket;
    h->isAuth = false;
    h->numUsingCommands = 0;
    h->state = NoAuthState;
    h->servers = this;
    ClientsList << h;
    h->iterator = ClientsList.end()--;
    signalClientConnected(h);
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
    if((*mClientID)->numUsingCommands <= 0){
        mutex.lock();
        ClientsList.erase(mClientID);
        delete (*mClientID);
        mutex.unlock();
        qDebug() << "Client disconnect Normal";
    }
    else
    {
        (*mClientID)->state = WaitCliseInClient;
        qDebug() << "Client disconnect WaitState";
    }
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
void ATCPServer::sendToClient(validClient *clientID, QByteArray str)
{
    clientID->numUsingCommands--;
    if(clientID->state != WaitCliseInClient && clientID->socket->state() != QTcpSocket::ClosingState){
    clientID->socket->write(str);}

    if(clientID->state == WaitCliseInClient && clientID->numUsingCommands <= 0)
    {
        clientID->DeleteMe();
    }
}
void validClient::DeleteMe()
{
    servers->ClientsList.erase(iterator);
    delete this;
}
void ATCPServer::CloseClient(validClient *clientID)
{
    clientID->numUsingCommands--;
    if(clientID->state != WaitCliseInClient && clientID->socket->state() != QTcpSocket::ClosingState){
    clientID->socket->disconnectFromHost();}

    if(clientID->state == WaitCliseInClient && clientID->numUsingCommands <= 0)
    {
        clientID->DeleteMe();
    }
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

    arrayMutex.lock();
    if(ArrayCommands.isEmpty())
    {
        arrayMutex.unlock();
        return;
    }
    ArrayCommand sCommand = ArrayCommands.takeFirst();
    arrayMutex.unlock();
    server->mutex.lock();
    QLinkedList<validClient*>::iterator mClientID= server->GetIDClient(sCommand.client);
    validClient* nClient = (*mClientID);
    server->mutex.unlock();
    nClient->numUsingCommands++;
    server->UseCommand(sCommand.command,nClient,this);
    nClient->numUsingCommands--;
    if(nClient->state == WaitCliseInClient && nClient->numUsingCommands <= 0)
    {
        server->mutex.lock();
        server->ClientsList.erase(mClientID);
        delete nClient ;
        server->mutex.unlock();
        qDebug() << "Client disconnect END";
    }
}
