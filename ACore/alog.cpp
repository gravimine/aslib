#include "alog.h"
namespace ACore
{
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
    char chr = 10;
    for(auto i=begin();i!=end();++i)
    {
        logging.write((*i));
        logging.write(&chr);
    }
    logging.close();
    clear();
}
QString ALog::toString()
{
    QString result;
    for(auto i=begin();i!=end();++i)
    {
        result+=QString::fromUtf8((*i))+"\n";
    }
    return result;
}
QString ALog::toString(int start,int endd)
{
    int current=0;
    if(start<0) return QString();
    if(size()-start<endd) endd=size()-start;
    QString result;
    for(auto i=begin();i!=end();++i)
    {
        if(current>=start)
        result+=QString::fromUtf8((*i))+"\n";
        if(current>=endd) break;
        current++;
    }
    return result;
}

QByteArray ALog::toByteArray()
{
    QByteArray result;
    for(auto i=begin();i!=end();++i)
    {
        result+=(*i)+"\n";
    }
    return result;
}
QString ALog::toHTML()
{
    QString result;
    for(auto i=begin();i!=end();++i)
    {
        result+=QString::fromUtf8((*i))+"<br>";
    }
    return result;
}
void ALog::operator <<(QString h)
{
    append((dtime()+h).toUtf8());
    if(isDebug) qDebug() << dtime()+h;
    AddLog();
}
void ALog::addInfo(QString h)
{
    append(("[info]"+dtime()+h).toUtf8());
    if(isDebug) qDebug() << "[info]"+dtime()+h;
    AddLog();
}
void ALog::addError(QString h)
{
    append(("[error]"+dtime()+h).toUtf8());
    if(isDebug) qDebug() << "[error]"+dtime()+h;
    AddLog();
}
void ALog::setfile(QString data)
{
    patch=data;
}
}
