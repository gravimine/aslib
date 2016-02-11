#ifndef AALGORITM_H
#define AALGORITM_H
#include <QString>
#define REPLACE_TEXT_I "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
namespace AAlgoritm
{
QString StringRevert(QString str);
QString html_find(QString htmlcode,QString sMin,QString sMax, int start=0);
QString html_find_s(QString htmlcode,QString html, int start=0);
QString QtHtmlRecoder(QString html);
}
#endif // AALGORITM_H
