#include "aalgoritm.h"
#include <QString>
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
