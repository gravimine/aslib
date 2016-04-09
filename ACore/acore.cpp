#include "acore.h"
#include "amath.h"
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
    RecursionArray::RecursionArray(std::initializer_list<RecVal> list)
    {
        for(auto &i : list)
        {
            this->operator [](i.name) = i.val;
        }
    }

    QMap<QString, QVariant> RecursionArray::_fromPostGetFormat(const QString post)
    {
        QMap<QString,QVariant> result;
        QStringList argList = post.split("&");
        if(argList.isEmpty()) argList << post;
        for(int i=0;i<argList.size();i++)
        {
            QString str = argList.value(i);
            QStringList values = str.split("=");
             if(!values.value(0).isEmpty() && !values.value(1).isEmpty()) result[values.value(0)]=values.value(1);
        }
        return result;
    }

    QString RecursionArray::toPost()
    {
        QString result;
        QStringList keyd=keys();
        for(int i=0;i<size();i++)
        {
            if(i==0) result+=keyd.value(i)+"="+VariantToString(value(keyd.value(i)));
            else result+="&"+keyd.value(i)+"="+VariantToString(value(keyd.value(i)));
        }
        return result;
    }
	QString SpecialSybmolCoder(QString value,bool isDecode)
	{
		if(!isDecode) return value.replace("<","/k0001").replace(">","/k0002");
		else return value.replace("/k0001","<").replace("/k0002",">");
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
	QString DeleteSpaceStart(QString str)
	{
		int deleteSize=0;
        for(int i=0;str[i]==' ';i++) deleteSize++;
		return str.remove(0,deleteSize);
	}
    bool isFullEmpry(QString str)
    {
        int deleteSize=0;
        for(int i=0;str[i]==' ' || str[i]=='\n';i++) deleteSize++;
        return str.remove(0,deleteSize).isEmpty();
    }
	QString DeleteQuotes(QString str)
	{
		if(str[0]=='"' && str[str.size()-1]=='"')
		return str.remove(0,1).remove(str.size()-2,1);
		else return str;
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
	double GradusToRadian(double grad)
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
		for(int i=0;i<size();i++)
		{
			QString m=value(i)+"\n";
			logging.write(m.toLocal8Bit());
		}
		logging.close();
		clear();
	}
	QString ALog::toString()
	{
		QString result;
		for(int i=0;i<size();i++)
		{
			result+=value(i)+"\n";
		}
		return result;
	}
	QString ALog::toHTML()
	{
		QString result;
		for(int i=0;i<size();i++)
		{
			result+=value(i)+"<br>";
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
	}

	ASettings::ASettings()
	{
        file = "this.cfg";
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



	QString RecursionArray::_toHTMLTegsFormat(RecursionArray Map)
	{
		QString ReturnValue;
		int i=0;
		QList<QString> keys=Map.keys();
		while(i<keys.size())
		{
			ReturnValue+="<"+keys.value(i)+">";
			QString tmp= _toHTMLTegsFormat(Map.value(keys.value(i)).toMap());
			if(tmp.isEmpty()) ReturnValue+=VariantToString(Map.value(keys.value(i)));
			else ReturnValue+=tmp;
			ReturnValue+="</"+keys.value(i)+">";
			i++;
		}
		return ReturnValue;
	}
	QString RecursionArray::_toYUMFormat(RecursionArray Map,QString Tabulator)
	{
		QString ReturnValue;
		int i=0;
		QList<QString> keys=Map.keys();
		while(i<keys.size())
		{
			ReturnValue+="\n"+Tabulator+keys.value(i)+":";
			QString tmp= _toYUMFormat(Map.value(keys.value(i)).toMap(),Tabulator+" ");
			if(tmp.isEmpty()) ReturnValue+=VariantToString(Map.value(keys.value(i)));
			else ReturnValue+=tmp;
			i++;
		}
		return ReturnValue;
	}
    QString RecursionArray::_toCFGFormat(RecursionArray Map)
    {
        bool isStart=true;
		QString ReturnValue;
		int i=0;
		QList<QString> keys=Map.keys();
		while(i<keys.size())
		{
			QString TypeValue;
			QVariant Value=Map.value(keys.value(i));
			if(Value.type()==QVariant::String) TypeValue="S";
			else if(Value.type()==QVariant::Int) TypeValue="I";
            else if(Value.type()==QVariant::Double) TypeValue="D";
			else if(Value.type()==QVariant::Bool) TypeValue="B";
            else if(Value.type()==QVariant::Map)
            {
                QString tmp= _toCFGFormat(Map.value(keys.value(i)).toMap());
                if(!tmp.isEmpty()){
                ReturnValue+="\n"+keys.value(i)+" {\n"+tmp+"\n}";
                }
                i++;
                continue;
            }
			else
			{
				i++;
				continue;
            }
            if(!isStart)ReturnValue+="\n"+TypeValue+":"+keys.value(i)+"=";
            else ReturnValue+=TypeValue+":"+keys.value(i)+"=";
            ReturnValue+=VariantToString(Map.value(keys.value(i)));
			i++;
            if(isStart) isStart=false;
		}
		return ReturnValue;
	}
	QString RecursionArray::printList(QList<QVariant> List)
	{
		QString ReturnValue="[";
		QVariant Value0=List.value(0);
		if(Value0.type()==QVariant::String) if(!Value0.toString().isEmpty()) ReturnValue+=Value0.toString();
		for(int i=1;i<List.size();i++)
		{
			QVariant Value=List.value(i);
			if(Value.type()==QVariant::String) if(!Value.toString().isEmpty())
			ReturnValue+=","+Value.toString();
		}
		ReturnValue+="]";
		return ReturnValue;
	}
	QString RecursionArray::printMap(RecursionArray Map,QString NameMap,QString Tabulator)
	{
		QString ReturnValue;
		int i=0;
		QList<QString> keys=Map.keys();
		if(keys.size()<=0) return "";
		ReturnValue+=Tabulator+"QMap("+NameMap+")\n"+Tabulator+"{\n";
		while(i<keys.size())
		{
			QString NameKey=keys.value(i);
			QVariant tmp=Map.value(NameKey);
			if(tmp.type()!=QVariant::Map) ReturnValue+=Tabulator+"   ["+NameKey+"] = ";
			if(tmp.type()==QVariant::String) ReturnValue+= Map.value(NameKey).toString();
			else if(tmp.type()==QVariant::ByteArray) ReturnValue+= QString(Map.value(NameKey).toByteArray());
			else if(tmp.type()==QVariant::List) ReturnValue+=printList(Map.value(NameKey).toList());
			else if(tmp.type()==QVariant::Int) ReturnValue+=QString::number(Map.value(NameKey).toInt());
			else if(tmp.type()==QVariant::Double) ReturnValue+=QString::number(Map.value(NameKey).toDouble());
			else if(tmp.type()==QVariant::Bool)
			{
				if(tmp.toBool()) ReturnValue+="true";
				else ReturnValue+="false";
			}
			else if(tmp.type()==QVariant::Map) {
				ReturnValue+=printMap(Map.value(NameKey).toMap(),NameKey,Tabulator+"   ");
			}
			else ReturnValue+="Unkown()";
			ReturnValue+="\n";
			i++;
		}
		ReturnValue+=Tabulator+"}";
		return ReturnValue;
	}
	QString RecursionArray::VariantToString(QVariant tmp)
	{
		if(tmp.type()==QVariant::String) return  tmp.toString();
		else if(tmp.type()==QVariant::ByteArray) return QString(tmp.toByteArray());
		else if(tmp.type()==QVariant::List) return printList(tmp.toList());
		else if(tmp.type()==QVariant::Int) return QString::number(tmp.toInt());
		else if(tmp.type()==QVariant::Double) return QString::number(tmp.toDouble());
		else if(tmp.type()==QVariant::Bool)
		{
			if(tmp.toBool()) return "true";
			else return "false";
		}
		else if(tmp.type()==QVariant::Map) {
			return printMap(tmp.toMap());
		}
		else return "Unkown()";
	}
    QMap<QString,QVariant> RecursionArray::_fromYumFormat(QString yum,QString level)
	{
		QStringList fromBR=yum.split("\n");
		QMap<QString,QVariant> ReturnMap;
		for(int i=0;i<fromBR.size();i++)
		{
			QString ValueString=fromBR.value(i);
			if(level.size()>0) ValueString.remove(0,level.size());
			if(ValueString.toLocal8Bit()[0]=='#') continue;
			int position=ValueString.indexOf(":");
			if(position<=0) continue;
			QString NameValue=ValueString.mid(0,position);
			QString Value=ValueString.mid(position+1);
			if(!Value.isEmpty())
			{
                ReturnMap[NameValue]=Value;
			}
			else
			{
				int unusedsize=0;bool stop=true; QString sendString,nextLevel;
				nextLevel=level+ " ";
				while (stop) {

					if(fromBR.value(i+unusedsize+1).mid(0,nextLevel.size())==nextLevel){
						unusedsize++;
						sendString+=fromBR.value(i+unusedsize)+"\n";
					}
					else stop=false;
				}
                QMap<QString,QVariant> ValueMap=_fromYumFormat(sendString,nextLevel);
                ReturnMap[NameValue]=ValueMap;
				i+=unusedsize;
			}
		}
		return ReturnMap;
	}
    void RecursionArray::operator<<(AbstractStruct* h)
    {
        this->operator [](h->GetName())=h->GetAllValues();
    }

    QMap<QString,QVariant> RecursionArray::_fromCfgFormat(QString yum)
	{
		QStringList fromBR=yum.split("\n");
		QMap<QString,QVariant> ReturnMap;
		for(int i=0;i<fromBR.size();i++)
		{
			QString ValueString=fromBR.value(i);
			ValueString=DeleteSpaceStart(ValueString);
			if(ValueString[0]=='#') continue;
			int position=ValueString.indexOf("=");
			if(position<=0 && ValueString.indexOf("{")<=0 && ValueString.indexOf("}")<=0) continue;
			QString NameValue=ValueString.mid(0,position);
			QString tmp=ValueString.mid(position+1);
			QStringList ListValued;
			ListValued << NameValue.mid(0,1);
			ListValued << NameValue.mid(2);
			ListValued << ValueString.mid(position+1);
			NameValue=DeleteQuotes(ListValued.value(1));
			QVariant Value;
			if(ListValued.value(0)=="I") Value=ListValued.value(2).toInt();
            if(ListValued.value(0)=="D") Value=ListValued.value(2).toDouble();
			else if(ListValued.value(0)=="S") {if(!tmp.isEmpty()) Value=ListValued.value(2);}
			else if(ListValued.value(0)=="B")
			{
				if(ListValued.value(2).toLower().indexOf("false")>=0) Value=false;
				else if(ListValued.value(2).toLower().indexOf("true")>=0) Value=true;
			}
			if(ValueString.indexOf("{")<=0)
			{
                ReturnMap[NameValue]=Value;
			}
			else
			{

                int unusedsize=0,balance=0;bool stop=true; QString sendString;
				while (stop) {
					QString x=fromBR.value(i+unusedsize+1);
                    if(unusedsize>fromBR.size()-i) stop=false;
                    if(x.indexOf("{")>=0)
                    {
                        balance++;unusedsize++;sendString+=fromBR.value(i+unusedsize)+"\n";}
                    else if(x.indexOf("}")>=0)
                    {
                        if(balance>0) {
                            balance--;unusedsize++;sendString+=fromBR.value(i+unusedsize)+"\n";}
                        else {
                            stop=false;
                        }
                    }
                    else
                    {
                        unusedsize++;
                        sendString+=fromBR.value(i+unusedsize)+"\n";
                    }
				}
                QMap<QString,QVariant> ValueMap=_fromCfgFormat(sendString);
				QString sNameValue=DeleteQuotes(ValueString.remove("{").split(" ").value(0));
                ReturnMap[sNameValue]=ValueMap;
				i+=unusedsize;
			}
		}
		return ReturnMap;
	}
	RecursionArray::RecursionArray(QMap<QString,QVariant> h)
	{
		QMap::operator =(h);
	}
	RecursionArray::RecursionArray()
	{
	}
    QMap<QString,QVariant> RecursionArray::_fromHTMLTegsFormat(const QString value)
	{
		QMap<QString,QVariant> ReturnValue; //Возвращаемый массив
		int i=0;
		while(i<value.size()) //Главный цикл
		{
			int iMin=value.indexOf("<",i); //Поиск символа <
			if(iMin<0) break;
			int iMax=value.indexOf(">",iMin); //Поиск символа >
			if(iMax<0) break;
			QString NameValue;//Имя переменной
			NameValue=value.mid(iMin+1,iMax-iMin-1);
			int nextiMin=value.indexOf("<"+NameValue+">",iMax); //Поиск <ИмяПеременной>
			int nMax=value.indexOf("</"+NameValue+">",iMax); //Поиск закрывающего тега
			if(nextiMin!=-1) if(nextiMin<nMax) //Если существует еще по крайней мере один <ИмяПеременной>
			{
				int numNextiMin=0; //Колличество включений <ИмяПеременной>
				while(nextiMin!=-1 && nextiMin<nMax) //Получаю колличество включений
				{nextiMin=value.indexOf("<"+NameValue+">",nextiMin+1);
					numNextiMin+=1; }
				while(numNextiMin!=0) //Использую колличество включений для определения последнего закрывающего тега
				{nMax=value.indexOf("</"+NameValue+">",nMax+1);
					numNextiMin-=1;}
			}
			if(nMax<0) //Если nMax невалидный
			{
				i=iMax+2;
			}
			else {
				QString sValue; //Значение переменной
				sValue=value.mid(iMax+1,nMax-iMax-1);
                QMap<QString,QVariant> temp=_fromHTMLTegsFormat(sValue); //Рекурсия

				if(sValue.isEmpty() || NameValue.isEmpty()) {
					int tmp=iMax-iMin;
					i=nMax+tmp;
					continue; }
                if(temp.isEmpty()) //Если рекурсия не нашла других переменных в переменной
                ReturnValue[NameValue]=sValue;
				else
                ReturnValue[NameValue]=temp;
				int tmp=iMax-iMin; //Переход к следующему
				i=nMax+tmp;}
		}
		return ReturnValue; //Возврат готового Map
	}
    QString RecursionArray::toHtml()
	{
		return _toHTMLTegsFormat(*this);
	}
    void RecursionArray::fromHtml(const QString value)
    {
        operator=(_fromHTMLTegsFormat(value));
    }
    void RecursionArray::fromPost(QString post)
    {
        operator=(_fromPostGetFormat(post));
    }
    void RecursionArray::fromCfg(const QString yum)
    {
        operator=(_fromCfgFormat(yum));
    }
    void RecursionArray::fromYum(const QString yum)
    {
        operator=(_fromYumFormat(yum));
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

    QString RecursionArray::toYum()
	{
		return _toYUMFormat(*this);
	}
    QString RecursionArray::toCfg()
	{
		return _toCFGFormat(*this);
	}
	QString RecursionArray::print()
	{
		return printMap(*this);
	}
}
