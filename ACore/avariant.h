#ifndef AVARIANT_H
#define AVARIANT_H
#include <map>
#include <QString>
#include <QList>
#include <QVariant>
namespace ACore
{
class AVariant;
enum AVariantTypes
{
    AVString,AVInteger,AVDouble,AVMap,AVList,AVUnknown,AVVoid,AVBool
};

class AVariant
{
public:
    AVariant();
    ~AVariant();
    AVariant(std::map<QString,AVariant> mapd);
    AVariant(QString strd);
    AVariant(int integerd);
    AVariant(double double1d);
    AVariant(QList<QString> listd);
    void operator =(std::map<QString,AVariant> mapd);
    void operator =(QString strd);
    void operator =(int integerd);
    void operator =(double double1d);
    void operator =(QList<QString> listd);
    std::map<QString,AVariant> toMap();
    QString toString();
    int toInt();
    double toDouble();
    bool toBool();
    QList<QString> toList();
    void clear();
    void* toVoid();
    AVariantTypes type();
protected:
    void* uni;
    AVariantTypes typed;
};
}
#endif // AVARIANT_H
