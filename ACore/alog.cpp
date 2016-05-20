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
}
