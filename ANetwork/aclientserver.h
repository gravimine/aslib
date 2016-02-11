#ifndef ACLIENTSERVER_H
#define ACLIENTSERVER_H
#include <QTcpSocket>
#include <QTime>
#include <QString>
#include <QByteArray>
#include "anetwork.h"
class ATcpNetwork : public ANetwork::ANetworkAccessManager {
	Q_OBJECT
protected:
	QTcpSocket* m_pTcpSocket;
	quint16     m_nNextBlockSize;
	int Type; //Тип запроса
	bool isSendPost;
	bool isPoste;
    QList<ANetwork::AQuest> Posts;
public:
	ATcpNetwork();
	void  ConnectToServer(QString url,int port);
	void ClooseConnect();
	ATcpNetwork(const QString strHost,
	int nPort);
	void post(QString text, int Typ);
	public slots:
	void slotReadyRead();
	void slotError(QAbstractSocket::SocketError err);
	void slotConnected();
signals:
    void ARequestTcp(ANetwork::ANetworkReply reply);
};
#endif // ACLIENTSERVER

