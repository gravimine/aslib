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
