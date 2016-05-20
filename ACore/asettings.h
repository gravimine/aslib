#ifndef ASETTINGS_H
#define ASETTINGS_H
#include "recursionarray.h"
namespace ACore
{
class ASettings : public RecursionArray
{
public:
    ASettings(QString patch,ArrayFormates format);
    ASettings();
    ~ASettings();
    void setPatch(QString patch,ArrayFormates format);
    void LoadSettings();
    void SaveSettings();
    void setAutoSave(bool on);
protected:
    bool isAutoSave;
    QString file;
    ArrayFormates FileFormat;
};
}
#endif // ASETTINGS_H
