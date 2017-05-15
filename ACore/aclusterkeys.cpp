#include "aclusterkeys.h"

AClusterKeys::AClusterKeys()
{
    iOK="1";
    iNO="";
    iBAD="33";
    iNPE="61";
    iISE="40";
    iASF="32";
    iR404="50";
    iEVENTR="2";

    OK=ACore::RecursionArray{{"key",iOK}}.toArcan().toUtf8();
    NO=ACore::RecursionArray{{"key",iNO}}.toArcan().toUtf8();
    BAD=ACore::RecursionArray{{"key",iBAD}}.toArcan().toUtf8();
    NPE=ACore::RecursionArray{{"key",iNPE}}.toArcan().toUtf8();
    ISE=ACore::RecursionArray{{"key",iISE}}.toArcan().toUtf8();
    ASF=ACore::RecursionArray{{"key",iASF}}.toArcan().toUtf8();
    R404=ACore::RecursionArray{{"key",iR404}}.toArcan().toUtf8();
    EVENTR=ACore::RecursionArray{{"key",iEVENTR}}.toArcan().toUtf8();
}

ACore::RecursionArray AClusterKeys::toRecArray()
{
}
