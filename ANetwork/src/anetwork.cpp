#include "anetwork.h"
#include "aclientserver.h"
#include <QNetworkProxy>
using namespace ANetwork;
void ANetworkAccessManager::RealPost(QString posti)
{
    Network->post(Server,posti.toLocal8Bit().data());
}
void ANetworkAccessManager::RealGet(QString geti)
{
	Network->get(QNetworkRequest(QUrl(geti.toLocal8Bit().data())));
}
void ANetworkAccessManager::getReplyFinished1(QNetworkReply*reply)
{
    timer->stop();
	if(Type==RESERVE_CODE)
	{
		FileFinished(QString::fromUtf8(reply->readAll()));
	}
	else
	{
        ANetworkReply _value;
        _value.TextError=reply->errorString();
        _value.TextReply=QString::fromUtf8(reply->readAll()) ;
        reply->deleteLater();
        _value.Type=Type;
        ARequest(_value);
	}
    if(Posts.size()>0)
	{
		isSendPost=1;
		if(Posts.value(0).isPost)RealPost(Posts.value(0).post);
		else RealGet(Posts.value(0).post);
		Type=Posts.value(0).type;
		Posts.removeAt(0);
	}
	else isSendPost=0;
}
void ANetworkAccessManager::nextReqest()
{


    if(Posts.size()>0)
    {
        isSendPost=1;
        if(Posts.value(0).isPost) RealPost(Posts.value(0).post);
        else RealGet(Posts.value(0).post);
        Type=Posts.value(0).type;
        Posts.removeAt(0);
    }
    else isSendPost=0;
}
void ANetworkAccessManager::setDelayTimeout(int msec)
{
    timerDelay=msec;
    timer->setInterval(msec);
}

ANetworkAccessManager::ANetworkAccessManager()
{
	Network = new QNetworkAccessManager(this);
    timer = new QTimer(this);
    connect(Network, SIGNAL(finished(QNetworkReply*)),this, SLOT(getReplyFinished1(QNetworkReply*)));
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimer()));
	isSendPost=0;
}
ANetworkAccessManager::~ANetworkAccessManager()
{
	delete Network;
    delete timer;
}
void ANetworkAccessManager::slotTimer()
{
    ANetworkReply _value;
    _value.TextError="Timeout";
    _value.Type=Type;
    ARequest(_value);
}
void ANetworkAccessManager::downloadFile(QString url)
{
	get(url,RESERVE_CODE);
}
void ANetworkAccessManager::setCookie(QString name,QString value)
{
	QList<QNetworkCookie> cookies=Network->cookieJar()->cookiesForUrl(Server.url());
	cookies.append(QNetworkCookie(name.toLocal8Bit().data(), value.toLocal8Bit().data()));
	Network->cookieJar()->setCookiesFromUrl(cookies,Server.url());
}
QString ANetworkAccessManager::getCookie(QString name)
{
	QList<QNetworkCookie> cookies=Network->cookieJar()->cookiesForUrl(Server.url());
	QNetworkCookie s;
	s.setName(name.toLocal8Bit().data());
	s.setDomain(Server.url().toString());
    return cookies.value(cookies.indexOf(s)).value();
}
void ANetworkAccessManager::clearCookie(QString url)
{
	QList<QNetworkCookie> cookies;
	Network->cookieJar()->setCookiesFromUrl(cookies,url);
}
QList<QNetworkCookie> ANetworkAccessManager::getCookies(QString url)
{
	return Network->cookieJar()->cookiesForUrl(url);
}

void ANetworkAccessManager::post(QString text, int Typ)
{
    if(isSendPost==0) {
        timer->start();
        RealPost(text);
        Type=Typ;
        isSendPost=1;}
	else
	{
		AQuest tmp;
		tmp.post=text;
		tmp.type=Typ;
		tmp.isPost=true;
		Posts << tmp;
	}
    if(isDebug)qDebug()<< "Post:"+ text;
}
void ANetworkAccessManager::get(QString text, int Typ)
{

    if(isSendPost==0) {
        timer->start();
        RealGet(text);
        Type=Typ;
        isSendPost=1;}
	else
	{
		AQuest tmp;
		tmp.post=text;
		tmp.type=Typ;
		tmp.isPost=false;
		Posts << tmp;
	}
    if(isDebug) qDebug()<< "Get:"+ text;
}
void ANetworkAccessManager::setUrl(QString url)
{
    Server.setUrl(url);
}
QString ANetworkAccessManager::url()
{
    return Server.url().url();
}
void ANetworkAccessManager::setRawHeader(QByteArray name,QByteArray value)
{
    Server.setRawHeader(name,value);
}
ATcpNetwork::ATcpNetwork()
{
	m_pTcpSocket = new QTcpSocket(this);
	connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
	connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
	connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
	this,         SLOT(slotError(QAbstractSocket::SocketError))
	);
}
void  ATcpNetwork::ConnectToServer(QString url,int port)
{
	m_pTcpSocket->connectToHost(url,port);
}
void ATcpNetwork::ClooseConnect()
{
	m_pTcpSocket->close();
}

ATcpNetwork::ATcpNetwork(const QString strHost,
int nPort)
{
	m_pTcpSocket = new QTcpSocket(this);

	m_pTcpSocket->connectToHost(strHost, nPort);
	connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
	connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
	connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
	this,         SLOT(slotError(QAbstractSocket::SocketError))
	);
}
void ATcpNetwork::post(QString text, int Typ)
{
	if(isSendPost==0) { m_pTcpSocket->write(text.toLocal8Bit()); Type=Typ;isSendPost=1;}
	else
	{
		AQuest tmp;
		tmp.post=text;
		tmp.type=Typ;
		tmp.isPost=true;
		Posts << tmp;
	}
}
void ATcpNetwork::slotReadyRead()
{
	ANetworkReply _value;
	_value.TextError="Unkown Error";
	_value.TextReply=QString::fromUtf8(m_pTcpSocket->readAll());
	_value.Type=Type;
	ARequestTcp(_value);
	if(Posts.size()>=1)
	{
		isSendPost=1;
		m_pTcpSocket->write(Posts.value(0).post.toLocal8Bit());
		Type=Posts.value(0).type;
		Posts.removeAt(0);
	}
	else isSendPost=0;

}
void ATcpNetwork::slotError(QAbstractSocket::SocketError err)
{
	QString strError =
	"Error: " + (err == QAbstractSocket::HostNotFoundError ?
	"Хост не найден" :
	err == QAbstractSocket::RemoteHostClosedError ?
	"Сервер закрыл соеденение" :
	err == QAbstractSocket::ConnectionRefusedError ?
	"The connection was refused." :
	QString(m_pTcpSocket->errorString())
	);
	ANetworkReply _value;
	_value.TextError=strError;
	_value.TextReply="";
	_value.Type=Type;
	ARequestTcp(_value);
}
void ATcpNetwork::slotConnected()
{
}
