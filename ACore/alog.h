#ifndef ALOG_H
#define ALOG_H
#include <QObject>
#include <QLinkedList>
#include <QString>
#include <QFile>
#include "aalgoritm.h"
namespace ACore
{
class ALog : public QObject, public QLinkedList<QString>
{
    Q_OBJECT
public:
    void savelog();
    QString toString();
    QString toHTML();
    void setcoutdebug(bool i);
    void operator <<(QString h);
    void addInfo(QString h);
    void addError(QString h);
    void setfile(QString data);
    ALog();
    ~ALog();
protected:
    QString patch;
    bool isDebug;
signals:
    void AddLog();
};
}
#endif // ALOG_H
