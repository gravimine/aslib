#ifndef ABSTRACTSTRUCT
#define ABSTRACTSTRUCT
#include <QString>
#include <QMap>
#include <QVariant>
#define ASTRUCT_META_END return result;}
#define ASTRUCT_META_INIT(classname,n) virtual QString GetName() {return n;} \
bool operator==(classname h) \
{ \
    if(GetName()==h.GetName()) return true; \
    else return false;\
} \
virtual QMap<QString, QVariant> GetAllValues() { QMap<QString, QVariant> result;
#define ASTRUCT_META_APPEND(name) result[#name]=name;
#define ASTRUCT_META_APPEND_S(name,value) result[name]=value;
struct AbstractStruct
{
public:
    virtual QString GetName();
    virtual QMap<QString, QVariant> GetAllValues();
    bool FullRavno(AbstractStruct* h)
    { if(GetAllValues()==h->GetAllValues()) return true; else return false; }
};
#endif // ABSTRACTSTRUCT

