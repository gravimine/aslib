#include "aalgoritm.h"
#include <QString>
#include <QTime>
#include <QDate>
namespace ACore
{
QString QtHtmlRecoder(QString html)
{
    QString result;
    QString sMin=REPLACE_TEXT_I;
    QString sMax="</p>";
    int iMin,iMax;
    iMin=html.indexOf(sMin,0)+sMin.size();
    iMax=html.indexOf(sMax,iMin);
    for(int i=iMin;i<iMax;i++) result+=html[i];
    return result;
}
void GetBytes(void* get,const int size)
{
    for(int j=0; j<size; j++){
    char getup=*(((char*)(void*)get) + j);
    bool k;
    for(unsigned int i=0; i<sizeof(char)*8; i++) {
    k = ((getup>>i)&1);
    if(k) std::cout << 1;
    else std::cout << 0;
    }}
    std::cout << std::endl;
}
QByteArray IntToByte(int integer)
{
   QByteArray result;
   char* zen=(char*)((void*)&integer);
   if(integer==0)result.append((char)0);
   else result.append(*zen);
   if(integer>255 || integer<0)result.append(*(zen+1));
   if(integer>(256*256-1) || integer<0)result.append(*(zen+2));
   if(integer>(256*256*256-1) || integer<0)result.append(*(zen+3));
   return result;
}
QString html_find(QString htmlcode,QString sMin,QString sMax, int start)
{
    QString result;
    int iMin,iMax;
    iMin=htmlcode.indexOf(sMin,start)+sMin.size();
    iMax=htmlcode.indexOf(sMax,iMin);
    for(int i=iMin;i<iMax;i++) result+=htmlcode[i];
    return result;
}
QString html_find_s(QString htmlcode,QString html, int start)
{
    QString result;
    int iMin,iMax;
    QString sMax,sMin;
    sMin="<"+html+">";
    sMax="</"+html+">";
    iMin=htmlcode.indexOf(sMin,start)+sMin.size();
    iMax=htmlcode.indexOf(sMax,iMin);
    for(int i=iMin;i<iMax;i++) result+=htmlcode[i];
    return result;
}
QString dtime()
{
    QTime time=QTime::currentTime();
    QDate date=QDate::currentDate();
    QString datatime;
    datatime.sprintf("[%d.%d.%d][%d:%d:%d][%d]",date.day(),date.month(),date.year(),time.hour(),time.minute(),time.second(),time.msec());
    return datatime;
}
}
