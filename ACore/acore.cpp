#include "acore.h"
#include "amath.h"
#include "atests.h"
#include <QString>
#include <QtWidgets/QMessageBox>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QDir>
#include <QFile>
#include <QByteArray>
#include <QVariant>
#include <QTime>
#include <QTextCodec>
namespace ACore
{
	QStringList splitStringArgs(QString value)
	{
		QString _value;
		QStringList ReturnValue;
		bool isBlock=false;
		for(int i=0;i<value.size();i++)
		{
			if(value[i]=='\"'){
				if(!isBlock) isBlock=true;
				else isBlock=false;
			}
			else if(value[i]==' ')
			{
				if(!isBlock)
				{
					ReturnValue << _value;
					_value="";
				}
				else _value+=value[i];
			}
			else
			{
				_value+=value[i];
			}
		}
		ReturnValue << _value;
		return ReturnValue;
	}

	QString dtime()
	{
		QTime time=QTime::currentTime();
		QDate date=QDate::currentDate();
		QString datatime;
		datatime.sprintf("[%d.%d.%d][%d:%d:%d][%d]",date.day(),date.month(),date.year(),time.hour(),time.minute(),time.second(),time.msec());
		return datatime;
	}
	QString timeEx(QTime starture)
	{
		QTime time=QTime::currentTime();
		int second=time.second()-starture.second();
		int minutes=time.minute()-starture.minute();
		int hour=time.hour()-starture.hour();
		QString result;
		if(hour>0) result+=QString::number(hour)+"h";
		if(minutes>0) result+=QString::number(minutes)+" min";
		if(second>0) result+=QString::number(second)+" s";
		return result;
	}

	QString timeEx(int second, int minutes,int hour)
	{
		QTime time=QTime::currentTime();
		QString result;
		int oum=time.minute()*60+time.second()+time.hour()*60*60;
		int ouk=second+minutes*60+hour*60*60;
		int our=oum-ouk;
		int tmp=our/60;
		int ahour=tmp/60;
		int amin=GetOstatok(tmp,60);
		int asec=GetOstatok(our,60);
		if(ahour>0) result+=QString::number(ahour)+"h";
		if(amin>0) result+=QString::number(amin)+" min";
		if(asec>0) result+=QString::number(asec)+" s";
		return result;
	}


	QString dataEx(QDate starture)
	{
		QDate time=QDate::currentDate();
		int year=time.year()-starture.year();
		int month=time.month()-starture.month();
		int day=time.day()-starture.day();
		QString result;
		if(year>0) result+=QString::number(year)+"year ";
		if(month>0) result+=QString::number(month)+"m ";
		if(day>0) result+=QString::number(day)+"d ";
		return result;
	}
	QString dataEx(int year,int month,int day)
	{
		QDate time=QDate::currentDate();
		year=time.year()-year;
		month=time.month()-month;
		day=time.day()-day;
		QString result;
		if(year>0) result+=QString::number(year)+"year ";
		if(month>0) result+=QString::number(month)+"m ";
		if(day>0) result+=QString::number(day)+"d ";
		return result;
	}
	QString dataTimeEx(int second, int minutes,int hour,int year,int month,int day)
	{
		QString result;
		QDateTime timedate;
		QDate date; date.setDate(year,month,day);
		QTime time; time.setHMS(hour,minutes,second);
		timedate.setDate(date);
		timedate.setTime(time);
		QDateTime datatime;datatime=QDateTime::currentDateTime();
        int our=timedate.secsTo(datatime);
        int tmp=our/60;
        int ahour=tmp/60;
        int amin=GetOstatok(tmp,60);
        int asec=GetOstatok(our,60);

		if(ahour/24>0) result+=QString::number(ahour/24)+"d ";
		if(GetOstatok(ahour,24)>0) result+=QString::number(GetOstatok(ahour,24))+"h ";
		if(amin>0) result+=QString::number(amin)+"min ";
		if(asec>0) result+=QString::number(asec)+"s ";
		return result;
	}
    QDateTime dataTimeEx(QDateTime timedate,QDateTime datatime=QDateTime::currentDateTime())
    {
        QDateTime result;
        unsigned int our=timedate.secsTo(datatime);
        unsigned int tmp=our/60;
        unsigned int ahour=tmp/60;
        unsigned int amin=GetOstatok(tmp,60);
        unsigned int asec=GetOstatok(our,60);
        QTime timed; QDate date;
        unsigned int sec,min,hour,days;
        if(ahour/24>0) days=ahour/24;
        if(GetOstatok(ahour,24)>0) hour=GetOstatok(ahour,24);
        if(amin>0) min=amin;
        if(asec>0) sec=asec;
        timed.setHMS(hour,min,sec);
        date.addDays(days);
        result.setDate(date);
        result.setTime(timed);
        return result;
    }
    bool isFullEmpry(QString str)
    {
        int deleteSize=0;
        for(int i=0;str[i]==' ' || str[i]=='\n';i++) deleteSize++;
        return str.remove(0,deleteSize).isEmpty();
    }
    float GetOstatok(float x,float k)
	{
		float x1=x/k;
		if((int)x1==(float)x1) return 0;
		else
		{
            float x2=(int)x1*(int)k;
			return x-x2;
		}
	}
    inline double GradusToRadian(double grad)
	{
		return grad*M_PI/180;
	}
    void ALog::setcoutdebug(bool i)
    {
        isDebug=i;
    }
    ALog::ALog()
    {
        isDebug=false;
    }
    ALog::~ALog()
    {
    }
    void ALog::savelog()
	{
		QFile logging;
		logging.setFileName(patch);
		logging.open(QIODevice::Append);
        for(auto &i : *this)
		{
            QString m=i+"\n";
			logging.write(m.toLocal8Bit());
		}
		logging.close();
		clear();
	}
	QString ALog::toString()
	{
		QString result;
        for(auto &i : *this)
		{
            result+=i+"\n";
		}
		return result;
	}
	QString ALog::toHTML()
	{
		QString result;
        for(auto &i : *this)
		{
            result+=i+"<br>";
		}
		return result;
	}
	void ALog::operator <<(QString h)
	{
		append(dtime()+h);
        if(isDebug) qDebug() << dtime()+h;
        AddLog();
	}
	void ALog::addInfo(QString h)
	{
		append("[info]"+dtime()+h);
        if(isDebug) qDebug() << "[info]"+dtime()+h;
        AddLog();
	}
	void ALog::addError(QString h)
	{
		append("[error]"+dtime()+h);
        if(isDebug) qDebug() << "[error]"+dtime()+h;
        AddLog();
	}
    void ALog::setfile(QString data)
	{
		patch=data;
	}

	ASettings::ASettings(QString patch,ArrayFormates format)
	{
		file=patch;
		FileFormat=format;
        isAutoSave=false;
	}

	ASettings::ASettings()
	{
        file = "this.cfg";
        isAutoSave=false;
	}
    ASettings::~ASettings()
    {
        if(isAutoSave)
            SaveSettings();
    }
    void ASettings::setAutoSave(bool on)
    {
        isAutoSave=on;
    }
	void ASettings::setPatch(QString patch,ArrayFormates format)
	{
		file=patch;
		FileFormat=format;
	}
	void ASettings::LoadSettings()
	{
		QFile stream;
		stream.setFileName(file);
		stream.open(QIODevice::ReadOnly);
        if(stream.error()!=QFile::NoError)
        {
            qDebug() << "[FILE] File not open. SaveSettings...";
            SaveSettings();
            qDebug() << "[FILE] Settings saved";
            return;
        }
        if(stream.bytesAvailable()<=0)
        {
            stream.close();
        }
		switch(FileFormat)
		{
		case CfgFormat:
			{
                fromCfg(stream.readAll());
				break;
			}
		case YumFormat:
			{
                fromYum(stream.readAll());
				break;
			}
        case PostGetFormat:
            {
                break;
            }
        case StdHTMLTagesFormat:
            {
                fromHtml(stream.readAll());
                break;
            }
		}
        stream.close();
	}
	void ASettings::SaveSettings()
	{
		QFile stream;
		stream.setFileName(file);
		stream.open(QIODevice::WriteOnly);
        if(stream.error()!=QFile::NoError)
            return;
		switch(FileFormat)
		{
		case CfgFormat:
			{
                stream.write(toCfg().toLocal8Bit());
				break;
			}
		case YumFormat:
			{
                stream.write(toYum().toLocal8Bit());
				break;
			}
        case PostGetFormat:
            {
                stream.write(toPost().toLocal8Bit());
                break;
            }
        case StdHTMLTagesFormat:
            {
                stream.write(toHtml().toLocal8Bit());
                break;
            }
		}
	}

    void ATester::RecursionArrayTest(RecursionArray arrd)
    {
        qDebug() << "START";
        QTime testtime;
        testtime.start();
        qDebug() << "fromHtml: " << testtime.elapsed();
        testtime.start();
        QString pas = arrd.print();
        qDebug() << "print: " << testtime.elapsed();
        testtime.start();
        QString pas2 = arrd.toHtml();
        qDebug() << "toHtml: " << testtime.elapsed();
        testtime.start();
        QString pas4 = arrd.toYum();
        qDebug() << "toYum: " << testtime.elapsed();
        testtime.start();
        QString pas3 = arrd.toCfg();
        qDebug() << "toCfg: " << testtime.elapsed();
        testtime.start();
        QString pas5 = arrd.toArcan();
        qDebug() << "toArcan: " << testtime.elapsed();
        testtime.start();
        ACore::RecursionArray arrd2; arrd2.fromCfg(pas3);
        qDebug() << "fromCfg: " << testtime.elapsed();
        testtime.start();
        ACore::RecursionArray arrd3; arrd3.fromYum(pas4);
        qDebug() << "fromYum: " << testtime.elapsed();
        testtime.start();
        ACore::RecursionArray arrd4; arrd4.fromHtml(pas2);
        qDebug() << "fromHTML: " << testtime.elapsed();
        testtime.start();
        ACore::RecursionArray arrd5; arrd5.fromArcan(pas5);
        qDebug() << "fromArcan: " << testtime.elapsed();
    }

    void AArguments::load(QStringList args)
    {
        for(int i=0;i<args.size();i++)
        {
            QString arg;
            if(arg.mid(0,1)=="-")
            {
                for(int i=1;i<arg.size();i++)
                {
                    SymbolArgs << arg.mid(i,1);
                }
            }
            else
            {
                OtcherArgs << arg;
            }
        }
    }


}
