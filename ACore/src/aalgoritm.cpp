#include "aalgoritm.h"
#include <QString>
QString AAlgoritm::StringRevert(QString str)
{
  QString result;
  result.resize(str.size());
  for(int i=0;i<str.size();i++) result+=str[i];
  return result;
}
