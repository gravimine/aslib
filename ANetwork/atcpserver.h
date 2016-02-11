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

struct newClient
{
    QTcpSocket* socket;
    QString data;
    ClientState state;
    int numUsingCommands;
    bool isAuth,isUseCommand;
};

struct validClient : public newClient
{

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
    QMutex mutexArray;
    QLinkedList<ArrayCommand> ArrayCommands;
    ATCPServer* server;
    bool isStaticThread,isSleep;
    int currentIdThread,currentCommandID;
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
    bool launch(int port);
    void sendToClient(QTcpSocket* socket, QString str);
    void sendToClient(int clientID, QString str);
    validClient *getClient(QTcpSocket* socket);
    int GetIDClient(QTcpSocket* socket);
    virtual void UseCommand(ArrayCommand sCommand, validClient* nClient,int mClientID)
    {
        Q_UNUSED(sCommand);
        Q_UNUSED(nClient);
        Q_UNUSED(mClientID);
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
protected:
    QTimer* timer;
signals:
void signalCommand(int idThread);
public slots:
    void clientConnected();
    void clientDisconnect();
    void clientReadyRead();
    void MonitorTimer();
};
#endif // ATCPSERVER_H
