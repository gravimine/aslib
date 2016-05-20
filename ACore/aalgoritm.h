#ifndef AALGORITM_H
#define AALGORITM_H
#include <QString>
#include <QDebug>
#define REPLACE_TEXT_I "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
namespace ACore
{
QString html_find(QString htmlcode,QString sMin,QString sMax, int start=0);
QString html_find_s(QString htmlcode,QString html, int start=0);
QString QtHtmlRecoder(QString html);
class AConctructDestructTest
{
    AConctructDestructTest()
    {
        qDebug() << "Constructor";
    }
    ~AConctructDestructTest()
    {
        qDebug() << "Destructor";
    }
};
QString dtime();
QStringList splitStringArgs(QString value);
bool isFullEmpry(QString str);
}
#endif // AALGORITM_H
