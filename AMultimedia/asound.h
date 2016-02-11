#ifndef ASOUND_H
#define ASOUND_H
#include <QtMultimedia/QAudioOutput>
#include <QFile>
#include <QString>

namespace AMultimedia
{
enum SoundAPI
{
    classQAudioOutput,
    classQSound
};

class ASound
{
protected:
  QString patch;
  QFile inputFile;
  QAudioFormat audioformat;
  QAudioDeviceInfo info;
  QAudioOutput* audiotest;
  bool isOpen;
public:

  ASound();
  ~ASound();
  void SetPatch(QString file);
  void start(SoundAPI soundapi);
  void open();
  void ForDevice(QString devicename);
  void SetFormat(int SampleRate,int bitterit,int canals,QString codec,QAudioFormat::Endian ByteOrder,QAudioFormat::SampleType SampleType);
};
}

#endif // ASOUND_H
