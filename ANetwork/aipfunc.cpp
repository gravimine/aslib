#include "aipfunc.h"
#include <QStringList>
using namespace ANetwork;
qint32 ANetwork::GetIP4Adress(QString ipv4)
{
    QStringList lst;
    lst = ipv4.split(".");
    if(lst.size()<4) return 0;
    int intlst[4];
    intlst[0] = lst.value(0).toInt();
    intlst[1] = lst.value(1).toInt();
    intlst[2] = lst.value(2).toInt();
    intlst[3] = lst.value(3).toInt();
    long long int ipv4d;
    ipv4d= intlst[3]+intlst[2]*256+intlst[1]*256*256+intlst[0]*256*256*256;
    return ipv4d;
}
Q_IPV6ADDR ANetwork::GetIP6Adress(QString ipv6)
{
    qint16 ipv6bit[8];
    QStringList lst1 = ipv6.split("::");
    if(lst1.size()==2)
    {
        QStringList lstd2;
        if(!lst1.value(0).isEmpty()){
            lstd2= lst1.value(0).split(":");
            if(lstd2.size()==0) lstd2 << lst1.value(0);
        }
        QStringList lstd3;
        if(!lst1.value(1).isEmpty()) {lstd3 = lst1.value(1).split(":");
        if(lstd3.size()==0 ) lstd3 << lst1.value(1);}
        int i=0;
        int nuled=8-lstd2.size()-lstd3.size();
        if(nuled<=0) return Q_IPV6ADDR();
        if(!lstd2.isEmpty())
        for(;i<lstd2.size();i++)
        {
            ipv6bit[i]=lstd2.value(i).toInt(0,16);
        }
        for(;i<nuled;i++)
        {
            ipv6bit[i]=0;
        }
        if(!lstd3.isEmpty())
        for(int j=0;j<lstd3.size();j++)
        {
            ipv6bit[i]=lstd3.value(i).toInt(0,16);
            i++;
        }
    }
    else
    {
        QStringList lstd1 = ipv6.split(":");
        if(lstd1.size()==8)
        for(int i=0;i<lstd1.size();i++)
        {
            ipv6bit[i]=lstd1.value(i).toInt(0,16);
        }
    }
    Q_IPV6ADDR s;
    for(int i=0;i<8;i++) {s[i] = (qint8)(ipv6bit[i]/256); s[i+1] = (qint8)(ipv6bit[i] - (ipv6bit[i]/256)); qDebug() << s[i] << s[i+1];}
    return s;
}
