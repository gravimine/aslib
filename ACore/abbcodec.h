#ifndef ABBCODEC_H
#define ABBCODEC_H
#include <QString>
#include <QList>
namespace ACore
{ //BBCode уже готовые к использованию
  struct BBCodeArg
  {
    QString name;
    QString data;

  };
struct BBCode
{
  QString name;
  QList<BBCodeArg> args;
  QString data;
  bool operator ==(BBCode h)
  {
    if(name==h.name) return true;
    else return false;
  }
};
struct BBCodeRule
{
  QString nameBBCode;
  QString replaceHTML;
  bool operator ==(BBCodeRule h)
  {
    if(nameBBCode==h.nameBBCode) return true;
    else return false;
  }
  BBCodeRule() {}
  BBCodeRule(QString nameBBCode1,QString replaceHTML1)
  {
      nameBBCode=nameBBCode1;
      replaceHTML=replaceHTML1;
  }
};


QList<BBCode> ParsBBCode(QString str, QList<BBCode> validcodes);
QString replacerBBCode(QString str,QList<BBCodeRule> rules);
}
#endif // ABBCODEC_H
