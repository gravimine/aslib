#ifndef ACLUSTERKEYS_H
#define ACLUSTERKEYS_H
#include <QByteArray>
#include "recursionarray.h"
class AClusterKeys
{
public:
    AClusterKeys();
    QByteArray OK,NO,BAD,ISE,NPE,ASF,R404,EVENTR;
    QByteArray iOK,iNO,iBAD,iISE,iNPE,iASF,iR404,iEVENTR;
    ACore::RecursionArray toRecArray();
};

#endif // ACLUSTERKEYS_H
