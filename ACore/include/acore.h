#ifndef ACORE_H
#define ACORE_H
#include <QSettings>
#include <QString>
#include <QList>
#include <QMap>
#include <QFile>
#include <QMessageBox>
#include <QThread>
#include <QByteArray>
#include <QDate>
#include <QDateTime>
#include "amath.h"
#define MAX_MESSAGE_BOX 2
#define REPLACE_TEXT_I "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
#define MapFind(n,Map,keyd) QStringList keyd=Map.keys(); for(int n=0;n<keyd.size();n++)
#define foreash(n,mas) for(int n=0;n<mas.size();n++)
#define ASTRUCT_META_ENTER virtual QMap<QString, QVariant> GetAllValues() { QMap<QString, QVariant> result;
#define ASTRUCT_META_END return result;}
#define ASTRUCT_META_NAME(n) virtual QString GetName() {return n;}
#define ASTRUCT_APPEND(name) result[#name]=name;
//#define REPLACE_TEXT_II "<body style=\" font-family:\'MS Shell Dlg 2\'; font-size:8.25pt; font-weight:400; font-style:normal;\">"
namespace ACore
{
	enum ArrayFormates
	{
		StdHTMLTagesFormat,
		YumFormat,
		IniFormat,
        CfgFormat
	};
    struct AbstractStruct
    {
    public:
        bool operator==(AbstractStruct* h)
        {
            if(GetName()==h->GetName()) return true;
            else return false;
        }
        virtual QString GetName()
        { return QString(); }
        virtual QMap<QString, QVariant> GetAllValues()
        { return QMap<QString, QVariant>(); }
        bool FullRavno(AbstractStruct* h)
        { if(GetAllValues()==h->GetAllValues()) return true; else return false; }
    };
    struct TESTStruct : public AbstractStruct
    {
        QString MyName,TeXt,Key,StrKey;
        ASTRUCT_META_NAME(MyName)
        ASTRUCT_META_ENTER
            ASTRUCT_APPEND(TeXt)
            ASTRUCT_APPEND(Key)
            ASTRUCT_APPEND(StrKey)
        ASTRUCT_META_END
    };

	class RecursionArray : public QMap<QString,QVariant>
	{
	private:
		QString _toHTMLTegsFormat(RecursionArray Map);
		QString _toYUMFormat(RecursionArray Map,QString Tabulator="");
        QString _toCFGFormat(RecursionArray Map);
	public:
        static QString printList(const QList<QVariant> List);
        static QString printMap(const RecursionArray Map,const QString NameMap="",const QString Tabulator="");
        static QString VariantToString(const QVariant tmp);
        QMap<QString,QVariant> fromYumFormat(const QString yum,const QString level="",const bool isReturn=false);
        QMap<QString,QVariant> fromCfgFormat(const QString yum,const bool isReturn=false);
        RecursionArray(const QMap<QString,QVariant> h);
		RecursionArray();
        void operator<<(AbstractStruct* h);
        QMap<QString,QVariant> fromHTMLTegsFormat(const QString value, bool isReturn=false);
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
	};
	QString html_find(QString htmlcode,QString sMin,QString sMax, int start=0);
	QString html_find_s(QString htmlcode,QString html, int start=0);
	QStringList splitStringArgs(QString value);
	QString QtHtmlRecoder(QString html);
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

