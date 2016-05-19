#ifndef ACORE_H
#define ACORE_H
#include <QSettings>
#include <QString>
#include <QList>
#include <QLinkedList>
#include <QMap>
#include <QFile>
//#include <QtWidgets/QMessageBox>
#include <QThread>
#include <QByteArray>
#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <initializer_list>
#include "amath.h"
#include "recursionarray.h"
#define ACORE_VERSION "1.1.2"
//#define REPLACE_TEXT_II "<body style=\" font-family:\'MS Shell Dlg 2\'; font-size:8.25pt; font-weight:400; font-style:normal;\">"
namespace ACore
{
    class ALog : public QObject, public QLinkedList<QString>
	{
        Q_OBJECT
	public:
        void savelog();
		QString toString();
		QString toHTML();
        void setcoutdebug(bool i);
		void operator <<(QString h);
		void addInfo(QString h);
		void addError(QString h);
        void setfile(QString data);
        ALog();
        ~ALog();
	protected:
		QString patch;
        bool isDebug;
    signals:
        void AddLog();
	};
typedef QList<ALog> ALogPool;
    class AArguments
    {
    public:
        QStringList OtcherArgs;
        QStringList SymbolArgs;
        void load(QStringList args);
    };

	class ASettings : public RecursionArray
	{
	public:
		ASettings(QString patch,ArrayFormates format);
		ASettings();
        ~ASettings();
		void setPatch(QString patch,ArrayFormates format);
		void LoadSettings();
		void SaveSettings();
        void setAutoSave(bool on);
	protected:
        bool isAutoSave;
		QString file;
		ArrayFormates FileFormat;
	};

	QString dtime();

	class Sleeper: public QThread
	{
	public:
		static void msleep(int ms)
		{
			QThread::msleep(ms);
		}
        static void usleep(int us)
        {
            QThread::usleep(us);
        }
	};
	QStringList splitStringArgs(QString value);
	struct ASett
	{
		QString value;
		QString value_std;
		QString key;
		bool operator ==(ASett h)
		{
			if(key==h.key) return true;
			else return false;
		}
	};
	QString timeEx(QTime starture);
	QString timeEx(int second, int minutes=0,int hour=0);
	QString dataEx(QDate starture);
	QString dataEx(int year,int month,int day);
	QString dataTimeEx(int second, int minutes=0,int hour=0,int year=0,int month=0,int day=0);
    QDateTime dataTimeEx(QDateTime timedate,QDateTime datatime);
    bool isFullEmpry(QString str);

}
#endif // ACORE

