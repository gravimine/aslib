#include "asound.h"
#include <QString>
#include <QFile>
#include <QtMultimedia/QSound>
using ACore::ASound;
ASound::ASound()
{
    info = QAudioDeviceInfo::defaultOutputDevice();
}
ASound::~ASound()
{
    if(audiotest) delete audiotest;
}
void ASound::SetPatch(QString file)
{
  patch=file;
}
void ASound::start(ACore::SoundAPI soundapi)
{
    if(soundapi==ACore::classQAudioOutput){
    inputFile.seek(0);
    audiotest->start(&inputFile);}
    else if(ACore::classQSound)
    {
        QSound::play(patch);
    }
}
void ASound::SetFormat(int SampleRate,int bitterit,int canals,QString codec,QAudioFormat::Endian ByteOrder,QAudioFormat::SampleType SampleType)
{
    audioformat.setChannelCount(canals);
    audioformat.setCodec(codec);
    audioformat.setSampleRate(SampleRate);
    audioformat.setSampleSize(bitterit);
    audioformat.setByteOrder(ByteOrder);
    audioformat.setSampleType(SampleType);
}
void ASound::open()
{
    if(!isOpen){
    inputFile.setFileName(patch);
    inputFile.open(QIODevice::ReadOnly);
    audiotest=new QAudioOutput(info,info.nearestFormat(audioformat));
    isOpen=true;}
}
void ASound::ForDevice(QString devicename)
{
    QList<QAudioDeviceInfo> ach = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    for(int i=0;i<ach.size();i++)
    {
        if(ach.value(i).deviceName()==devicename) info=ach.value(i);
    }
}
