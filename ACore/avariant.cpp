#include "avariant.h"
using namespace ACore;
AVariant::AVariant(std::map<QString, AVariant> map)
{
    if(typed!=AVUnknown) clear();
    uni = new std::map<QString,AVariant>();
    *((std::map<QString,AVariant>*)uni)=map;
    typed =AVString;
}

AVariant::AVariant(QString strd)
{
    if(typed!=AVUnknown) clear();
    uni = new QString();
    *((QString*)uni)=strd;
    typed =AVString;
}
void AVariant::clear()
{
    switch(typed)
    {
    case AVString:
    {
        delete (QString*)uni;
        break;
    }
    case AVMap:
    {
        delete (std::map<QString,AVariant>*)uni;
        break;
    }
    case AVInteger:
    {
        delete (int*)uni;
        break;
    }
    case AVDouble:
    {
        delete (double*)uni;
        break;
    }
    case AVList:
    {
        delete (QList<QString>*)uni;
        break;
    }
    case AVBool:
    {
        delete (bool*)uni;
        break;
    }
    default:
    {
        break;
    }
    }
}

AVariant::AVariant()
{
    typed =AVUnknown;
}
AVariant::AVariant(int integer)
{
    if(typed!=AVUnknown) clear();
    uni = new int;
    *((int*)uni)=integer;
    typed =AVInteger;
}
AVariant::AVariant(double double1)
{
    if(typed!=AVUnknown) clear();
    uni = new double;
    *((double*)uni)=double1;
    typed =AVDouble;
}
AVariant::AVariant(QList<QString> list)
{
    if(typed!=AVUnknown) clear();
    uni = new QList<QString>();
    *((QList<QString>*)uni)=list;
    typed =AVList;
}
std::map<QString,AVariant> AVariant::toMap()
{
    return *((std::map<QString,AVariant>*)uni);
}
AVariant::~AVariant()
{
    clear();
}

QString AVariant::toString()
{
    return *((QString*)uni);
}

int AVariant::toInt()
{
    return *((int*)uni);
}

double AVariant::toDouble()
{
    return *((double*)uni);
}
bool AVariant::toBool()
{
    return *((bool*)uni);
}
void* AVariant::toVoid()
{
    return uni;
}


void AVariant::operator =(std::map<QString,AVariant> mapd)
{
    if(typed!=AVUnknown) clear();
    uni = new std::map<QString,AVariant>();
    *((std::map<QString,AVariant>*)uni)=mapd;
    typed =AVMap;
}

void AVariant::operator =(QString strd)
{
    if(typed!=AVUnknown) clear();
    uni = new QString();
    *((QString*)uni)=strd;
    typed =AVString;
}

void AVariant::operator =(int integerd)
{
    if(typed!=AVUnknown) clear();
    uni = new int;
    *((int*)uni)=integerd;
    typed =AVInteger;
}

void AVariant::operator =(double double1d)
{
    if(typed!=AVUnknown) clear();
    uni = new double;
    *((double*)uni)=double1d;
    typed =AVDouble;
}

void AVariant::operator =(QList<QString> listd)
{
    if(typed!=AVUnknown) clear();
    uni = new QList<QString>();
    *((QList<QString>*)uni)=listd;
    typed =AVList;
}



QList<QString> AVariant::toList()
{
    return *((QList<QString>*)uni);
}
AVariantTypes AVariant::type()
{
    return typed;
}
