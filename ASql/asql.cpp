#include "asql.h"
using namespace ASql;
void ASqlQuery::concat(QString par)
{
    reply+=par;
}
void ASqlQuery::concatPar(QString par)
{
    reply+=par.replace("'","''");
}
bool ASqlQuery::send()
{
    return exec(reply);
}
