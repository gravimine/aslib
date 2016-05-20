#include "asettings.h"
#include <QFile>
#include <QDebug>
namespace ACore
{
ASettings::ASettings(QString patch,ArrayFormates format)
{
    file=patch;
    FileFormat=format;
    isAutoSave=false;
}

ASettings::ASettings()
{
    file = "this.cfg";
    isAutoSave=false;
}
ASettings::~ASettings()
{
    if(isAutoSave)
        SaveSettings();
}
void ASettings::setAutoSave(bool on)
{
    isAutoSave=on;
}
void ASettings::setPatch(QString patch,ArrayFormates format)
{
    file=patch;
    FileFormat=format;
}
void ASettings::LoadSettings()
{
    QFile stream;
    stream.setFileName(file);
    stream.open(QIODevice::ReadOnly);
    if(stream.error()!=QFile::NoError)
    {
        SaveSettings();
        return;
    }
    if(stream.bytesAvailable()<=0)
    {
        stream.close();
    }
    switch(FileFormat)
    {
    case CfgFormat:
        {
            fromCfg(stream.readAll());
            break;
        }
    case YumFormat:
        {
            fromYum(stream.readAll());
            break;
        }
    case PostGetFormat:
        {
            break;
        }
    case StdHTMLTagesFormat:
        {
            fromHtml(stream.readAll());
            break;
        }
    }
    stream.close();
}
void ASettings::SaveSettings()
{
    QFile stream;
    stream.setFileName(file);
    stream.open(QIODevice::WriteOnly);
    if(stream.error()!=QFile::NoError)
        return;
    switch(FileFormat)
    {
    case CfgFormat:
        {
            stream.write(toCfg().toLocal8Bit());
            break;
        }
    case YumFormat:
        {
            stream.write(toYum().toLocal8Bit());
            break;
        }
    case PostGetFormat:
        {
            stream.write(toPost().toLocal8Bit());
            break;
        }
    case StdHTMLTagesFormat:
        {
            stream.write(toHtml().toLocal8Bit());
            break;
        }
    }
}
}
