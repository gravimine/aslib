#ifndef ATCPCLIENT_H
#define ATCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include "anetwork.h"
namespace ANetwork
{
class ATCPClient : public QObject
{
    Q_OBJECT
protected:
    QTcpSocket* socket;
    quint16 m_nNextBlockSize;
    QString datad;
public:
    ATCPClient();
    ~ATCPClient();
    void connectToHost(QString Host,int port);
    void disconnectToHost();
    void Send(QString data);
    void SetProxy(QString host,int port);
    QTcpSocket* currentSocket();
signals:
    void signalConnected();
    void signalDisconnect();
    void signalError(QString err);
    void signalRead(QString read);
public slots:
    void slotConected();
    void slotDisconnect();
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError error);
};
}

#endif // ATCPCLIENT_H
