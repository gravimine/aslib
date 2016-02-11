#include "abbcodec.h"
#include <QStringList>
using ACore::BBCode;
using ACore::BBCodeArg;
using ACore::BBCodeRule;
QList<BBCode> ACore::ParsBBCode(QString str, QList<BBCode> validcodes)
{
  QList<BBCode> result;
  int i=0;
  while(true)
    {
      int pos=str.indexOf("[",i);
      if(pos<0) break;
      int pos2=str.indexOf("]",pos);
      if(pos2<0) break;
      QString tmp=str.mid(pos+1,pos2-pos-1);
      QStringList args=tmp.split(" ");
      QList<BBCodeArg> BBArgs;
      QString namevalue=args.value(0);
      BBCode BB;
      BB.name=namevalue;
      if(!validcodes.contains(BB))
        {
          i=pos2;
          continue;
        }
      else
        {
          args.removeAt(0);
          for(int j=0;j<args.size();j++)
            {
              QString namearg=args.value(i).split("=").value(0);
              QString dataarg=args.value(i).split("=").value(1);
              BBCodeArg BBArg;
              BBArg.name=namearg;
              BBArg.data=dataarg;
              BBArgs << BBArg;
            }
          BB.args=BBArgs;
          int pos3=str.indexOf("["+namevalue+"]",pos2);
          if(pos3>=0) BB.data=str.mid(pos2+1,pos3-pos2-1);
          result <<BB;
          i=pos3+2;
        }
    }
  return result;
}

QString ACore::replacerBBCode(QString str,QList<BBCodeRule> rules)
{
  QString result=str;
  int i=0;
  while(true)
    {
      int pos=str.indexOf("[",i);
      if(pos<0) break;
      int pos2=str.indexOf("]",pos);
      if(pos2<0) break;
      QString tmp=str.mid(pos+1,pos2-pos-1);
      QStringList args=tmp.split(" ");
      QList<BBCodeArg> BBArgs;
      QString namevalue=args.value(0);
      BBCode BB;
      BB.name=namevalue;
      BBCodeRule tmprule;
      tmprule.nameBBCode=namevalue;
      if(!rules.contains(tmprule))
        {
          i=pos2+2;
          continue;
        }
      else
        {
          tmprule=rules.value(rules.indexOf(tmprule));
          args.removeAt(0);
          for(int j=0;j<args.size();j++)
            {
              QString namearg=args.value(i).split("=").value(0);
              QString dataarg=args.value(i).split("=").value(1);
              BBCodeArg BBArg;
              BBArg.name=namearg;
              BBArg.data=dataarg;
              BBArgs << BBArg;
            }
          BB.args=BBArgs;
          int pos3=str.indexOf("[/"+namevalue+"]",pos2);
          if(pos3>=0) BB.data=str.mid(pos2+1,pos3-pos2-1);
          QString replacecode=tmprule.replaceHTML;
          replacecode.replace("${data}",BB.data);
          for(int j=0;j<BBArgs.size();j++) replacecode.replace("${"+BBArgs.value(j).name+"}",BBArgs.value(j).data);
          result.replace(str.mid(pos,(pos3+namevalue.size()+3)-pos),replacecode);
          if(pos3>=0) i=pos3+2;
          else i=pos2+2;
        }
    }
  return result;
}
