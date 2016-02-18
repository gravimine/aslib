#ifndef ATCPSERVER_H
#define ATCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <acore.h>
#include <QMutex>
#include <QLinkedList>
#include <QTimer>
enum ClientState
{
    NoAuthState,
    AuthState,
    waitCloseInServer,
    WaitCliseInClient
};


struct validClient
{
    QMutex mutex;
    int m_NextBlockSize;
    QTcpSocket* socket;
    ClientState state;
    int numUsingCommands;
    bool isAuth,isUseCommand;
};
struct ArrayCommand
{
    QByteArray command;
    QTcpSocket* client;
    bool operator ==(ArrayCommand h)
    {
        if(client==h.client) return true;
        else return false;
    }
};
class ATCPServer;
class ServerThread : public QThread
{
    Q_OBJECT
private:
    void run();
public:
    ServerThread(ATCPServer* serv);
    void UseCommand();
    QMutex arrayMutex;
    QLinkedList<ArrayCommand> ArrayCommands;
    ATCPServer* server;
    bool isStaticThread,isSleep;
    int currentIdThread,currentCommandID;
signals:
    void sendToClient(QTcpSocket* socket, QString str);
    void sendToClient(int client, QString str);
    void CloseClient(int clientID);
public slots:
    void NewCommand(int idThread);
};
class ATCPServer : public QObject
{
    Q_OBJECT
public:
    ATCPServer();
    ~ATCPServer();
    QList<validClient*> ClientsList;
    QList<ServerThread*> ThreadList;
    QString ClientInConnectText;
    bool launch(QHostAddress host, int port);
    validClient *getClient(QTcpSocket* socket);
    int GetIDClient(QTcpSocket* socket);
    virtual void UseCommand(QByteArray sCommand, validClient* nClient,int mClientID,ServerThread* thisThread)
    {
        Q_UNUSED(sCommand);
        Q_UNUSED(nClient);
        Q_UNUSED(mClientID);
        Q_UNUSED(thisThread);
    }
    virtual validClient* NewValidClient()
    {
        validClient* result = new validClient();
        return result;
    }
    virtual void DelValidClient(validClient* h)
    {
        delete h;
    }
    QTcpServer* serverd;
    int MinThread,MaxThread;
signals:
void signalCommand(int idThread);
public slots:
    void clientConnected();
    void clientDisconnect();
    void clientReadyRead();
    void sendToClient(QTcpSocket* socket, QString str);
    void sendToClient(int clientID, QString str);
    void CloseClient(int clientID);
};
#endif // ATCPSERVER_H
