#ifndef ASQL_H
#define ASQL_H
#include "acore.h"
#include <QSqlQuery>
namespace ASql
{
class ASqlQuery : public QSqlQuery
{
public:
    void concat(QString par);
    void concatPar(QString par);
    bool send();
protected:
    QString reply;
};
}
#endif // ASQL_H
