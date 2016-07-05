#include "aclusterkeys.h"

AClusterKeys::AClusterKeys()
{
    iROK="1";
    iRNO="";
    iRBAD="33";
    iISE="40";
    iBAR="60";
    iNPE="61";
    iBANE="62";
    ROK=ACore::RecursionArray{{"key",iROK}}.toArcan().toUtf8();
    RNO=ACore::RecursionArray{{"key",iRNO}}.toArcan().toUtf8();
    RBAD=ACore::RecursionArray{{"key",iRBAD}}.toArcan().toUtf8();
    BAR=ACore::RecursionArray{{"key",iBAR}}.toArcan().toUtf8();
    NPE=ACore::RecursionArray{{"key",iNPE}}.toArcan().toUtf8();

}

ACore::RecursionArray AClusterKeys::toRecArray()
{
}
