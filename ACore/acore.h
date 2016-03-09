#ifndef ACORE_H
#define ACORE_H
#include <QSettings>
#include <QString>
#include <QList>
#include <QMap>
#include <QFile>
#include <QtWidgets/QMessageBox>
#include <QThread>
#include <QByteArray>
#include <QDate>
#include <QDateTime>
#include <QDebug>
#include "amath.h"
#define MAX_MESSAGE_BOX 2

#define MapFind(n,Map,keyd) QStringList keyd=Map.keys(); for(int n=0;n<keyd.size();n++)
#define ACORE_VERSION "1.0.8"
#define foreash(n,mas) for(int n=0;n<mas.size();n++)
#define ASTRUCT_META_ENTER virtual QMap<QString, QVariant> GetAllValues() { QMap<QString, QVariant> result;
#define ASTRUCT_META_END return result;}
#define ASTRUCT_META_INIT(classname,n) virtual QString GetName() {return n;} \
bool operator==(classname h) \
{ \
    if(GetName()==h.GetName()) return true; \
    else return false;\
}

#define ASTRUCT_META_APPEND(name) result[#name]=name;
#define ASTRUCT_META_APPEND_S(name,value) result[name]=value;
//#define REPLACE_TEXT_II "<body style=\" font-family:\'MS Shell Dlg 2\'; font-size:8.25pt; font-weight:400; font-style:normal;\">"
namespace ACore
{
	enum ArrayFormates
	{
		StdHTMLTagesFormat,
		YumFormat,
        PostGetFormat,
        CfgFormat
	};
    struct AbstractStruct
    {
    public:
        virtual QString GetName();
        virtual QMap<QString, QVariant> GetAllValues();
        bool FullRavno(AbstractStruct* h)
        { if(GetAllValues()==h->GetAllValues()) return true; else return false; }
    };
    class AConctructDestructTest
    {
        AConctructDestructTest()
        {
            qDebug() << "Constructor";
        }
        ~AConctructDestructTest()
        {
            qDebug() << "Destructor";
        }
    };

	class RecursionArray : public QMap<QString,QVariant>
	{
	private:
		QString _toHTMLTegsFormat(RecursionArray Map);
		QString _toYUMFormat(RecursionArray Map,QString Tabulator="");
        QString _toCFGFormat(RecursionArray Map);
        QMap<QString,QVariant> _fromYumFormat(QString yum, QString level="");
        QMap<QString,QVariant> _fromCfgFormat(QString yum);
        QMap<QString,QVariant> _fromHTMLTegsFormat(QString value);
        QMap<QString,QVariant> _fromPostGetFormat(QString post);
	public:
        static QString printList(const QList<QVariant> List);
        static QString printMap(const RecursionArray Map,const QString NameMap="",const QString Tabulator="");
        static QString VariantToString(const QVariant tmp);

        RecursionArray(const QMap<QString,QVariant> h);
		RecursionArray();
        void operator<<(AbstractStruct* h);
        void fromYumFormat(const QString yum);
        void fromCfgFormat(const QString yum);
        void fromPostGetFormat(const QString post);
        void fromHTMLTegsFormat(const QString value);
		QString toHTMLTegsFormat();
		QString toYUMFormat();
		QString toCFGFormat();
        QString toPostGetFormat();
		QString print();
	};
    class ALog : public QObject, public QStringList
	{
        Q_OBJECT
	public:
		void SaveLog();
		QString toString();
		QString toHTML();
        void SetCoutDebug(bool i);
		void operator <<(QString h);
		void addInfo(QString h);
		void addError(QString h);
		void SetFile(QString data);
        ALog();
        ~ALog();
	protected:
		QString patch;
        bool isDebug;
    signals:
        void AddLog();
	};
    class AArguments
    {
    public:
        QStringList OtcherArgs;
        QStringList SymbolArgs;
        void Load(QStringList args);
    };

	class ASettings : public RecursionArray
	{
	public:
		ASettings(QString patch,ArrayFormates format);
		ASettings();
		void setPatch(QString patch,ArrayFormates format);
		void LoadSettings();
		void SaveSettings();
	protected:
		QString file;
		ArrayFormates FileFormat;
	};
	class AAppCore
	{
	public:
		void SendM(QString text);
		void Error(QString text);
		bool MessageQuest(QString text);
		void SetProgramName(QString name);
		AAppCore();
		~AAppCore();
		AAppCore(QString ProgName);
	protected:
		int MessageBoxNumber;
		QString ProgramName;
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
	QString SpecialSybmolCoder(QString value,bool isDecode);
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
	QString DeleteSpaceStart(QString str);
    bool isFullEmpry(QString str);
	QString DeleteQuotes(QString str);

}
#endif // ACORE

