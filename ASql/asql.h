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
protected:
    QString reply;
};
}
#endif // ASQL_H
