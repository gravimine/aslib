#ifndef RECURSIONARRAY_H
#define RECURSIONARRAY_H
#include <QString>
#include <QMap>
#include <QVariant>
#include <QStringList>
#include <QList>
#include <initializer_list>
#include "abstractstruct.h"

namespace ACore
{
QString DeleteQuotes(QString str);
QString DeleteSpaceStart(QString str);
enum ArrayFormates
{
    StdHTMLTagesFormat,
    YumFormat,
    PostGetFormat,
    CfgFormat
};
struct RecVal
{
    QString name;
    QVariant val;
};
class RecursionArray : public QMap<QString,QVariant>
{
private:
    QString _toHTMLTegsFormat(const RecursionArray Map);
    QString _toYUMFormat(const RecursionArray Map, QString Tabulator="");
    QString _toCFGFormat(const RecursionArray Map);
    QString _toArcanFormat(const RecursionArray Map);
    QMap<QString,QVariant> _fromYumFormat(const QString yum, QString level="", bool isReturn=false);
    QMap<QString,QVariant> _fromCfgFormat(const QString yum, bool isReturn=false);
    QMap<QString,QVariant> _fromHTMLTegsFormat(const QString value, bool isReturn=false);
    QMap<QString,QVariant> _fromPostGetFormat(const QString post);
    QMap<QString,QVariant> _fromArcanFromat(const QString post);
public:
    static QString printList(const QList<QVariant> List);
    static QString printMap(const RecursionArray Map,const QString NameMap="",const QString Tabulator="");
    static QString VariantToString(const QVariant tmp);

    RecursionArray(const QMap<QString,QVariant> h);
    RecursionArray();
    RecursionArray(std::initializer_list<RecVal> list);
    void operator<<(AbstractStruct* h);
    void fromYum(const QString yum);
    void fromCfg(const QString yum);
    void fromPost(const QString post);
    void fromHtml(const QString value);
    void fromArcan(const QString value);
    QString toHtml();
    QString toYum();
    QString toCfg();
    QString toPost();
    QString toArcan();
    QString print();
};
}
#endif // RECURSIONARRAY_H
