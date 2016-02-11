#include "atcpclient.h"
#include "atcpinhttpfunc.h"
#include <QNetworkProxy>
namespace ANetwork
{
ATCPClient::ATCPClient()
{
    socket = new QTcpSocket();
    m_nNextBlockSize = 0;
    connect(socket, SIGNAL(connected()), SLOT(slotConected()));
    connect(socket, SIGNAL(readyRead()), SLOT(slotReadyRead()));

}
void ATCPClient::slotDisconnect()
{
    signalDisconnect();
}
void ATCPClient::disconnectToHost()
{
    socket->disconnectFromHost();
}

void ATCPClient::connectToHost(QString Host,int port)
{
    socket->connectToHost(Host,port);
}
void ATCPClient::slotConected()
{
    signalConnected();
}
void ATCPClient::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
            "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                         "The host was not found." :
                         err == QAbstractSocket::RemoteHostClosedError ?
                         "The remote host is closed." :
                         err == QAbstractSocket::ConnectionRefusedError ?
                         "The connection was refused." :
                         QString(socket->errorString())
                        );
    qDebug() << "Network Error:"+strError;
    signalError(strError);
}
void ATCPClient::slotReadyRead()
{
    datad+=socket->readAll();
    qDebug() << datad;
    if(!socket->bytesAvailable()){
    signalRead(datad);datad.clear();}
}
QTcpSocket* ATCPClient::currentSocket()
{
    return socket;
}
void ATCPClient::SetProxy(QString host,int port)
{
    QNetworkProxy s;
    s.setHostName(host);
    s.setPort(port);
    s.setType(QNetworkProxy::Socks5Proxy);
    socket->setProxy(s);
}

void ATCPClient::Send(QString data)
{
   socket->write(data.toUtf8());
}
ATCPClient::~ATCPClient()
{
    delete socket;
}
QString FormatHTTP(ACore::RecursionArray Map,QString host)
{
    return "GET / HTTP/1.1"+Map.toYUMFormat()+"\nHost:"+host+"\n\n";
}
}

