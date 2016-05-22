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
    for(auto &i : *this)
    {
        logging.write(i);
        logging.write(&chr);
    }
    logging.close();
    clear();
}
QString ALog::toString()
{
    QString result;
    for(auto &i : *this)
    {
        result+=QString::fromUtf8(i)+"\n";
    }
    return result;
}
QByteArray ALog::toByteArray()
{
    QByteArray result;
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
        result+=QString::fromUtf8(i)+"<br>";
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
