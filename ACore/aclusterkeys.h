#ifndef ACLUSTERKEYS_H
#define ACLUSTERKEYS_H
#include <QByteArray>
#include "recursionarray.h"
class AClusterKeys
{
public:
    AClusterKeys();
    QByteArray TWE,ROK,RNO,RBAD,SQLE,ISE,NPE,BANE,KR,NFR,BAR,EVENTR;
    QByteArray iTWE,iROK,iRNO,iRBAD,iSQLE,iISE,iNPE,iBANE,iKR,iNFR,iBAR,iEVENTR;
    ACore::RecursionArray toRecArray();
};

#endif // ACLUSTERKEYS_H
