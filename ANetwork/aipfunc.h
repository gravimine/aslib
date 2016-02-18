#ifndef AIPFUNC
#define AIPFUNC
#include <QString>
#include <Q_IPV6ADDR>
namespace ANetwork
{
qint32 GetIP4Adress(QString ipv4);
Q_IPV6ADDR GetIP6Adress(QString ipv6);
}

#endif // AIPFUNC

