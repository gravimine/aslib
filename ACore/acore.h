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
#define ACORE_VERSION "1.1.3"
#define ACORE_TYPEVERSION "release"
//#define REPLACE_TEXT_II "<body style=\" font-family:\'MS Shell Dlg 2\'; font-size:8.25pt; font-weight:400; font-style:normal;\">"
namespace ACore
{
    class AArguments
    {
    public:
        QStringList OtcherArgs;
        QStringList SymbolArgs;
        void load(QStringList args);
    };


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

	QString timeEx(QTime starture);
	QString timeEx(int second, int minutes=0,int hour=0);
	QString dataEx(QDate starture);
	QString dataEx(int year,int month,int day);
	QString dataTimeEx(int second, int minutes=0,int hour=0,int year=0,int month=0,int day=0);
    QDateTime dataTimeEx(QDateTime timedate,QDateTime datatime);


}
#endif // ACORE

