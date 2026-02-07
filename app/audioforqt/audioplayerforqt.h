#ifndef AUDIOPLAYERFORQT_H
#define AUDIOPLAYERFORQT_H

#include <QAudioSink>

#include "prerequisites.h"

#include "audiointerfaceforqt.h"

class AudioPlayerThreadForQt;

///////////////////////////////////////////////////////////////////////////////
/// \brief The AudioPlayerForQt class
///
/// This class implements the audio player for Qt. Its main purpose is to
/// start an indpendent Qt-compatible thread because the Qt audio player must
/// be operated from a single Qt-thread only.
///////////////////////////////////////////////////////////////////////////////

class AudioPlayerForQt : public AudioInterfaceForQt {
  Q_OBJECT

public:
  AudioPlayerForQt(QObject *parent);
  ~AudioPlayerForQt() {}

  void exit() override final;

  void start() override final;
  void stop() override final;

  virtual void suspendChanged(bool s) override final;

  virtual void setGain(double gain) override final;
  virtual double getGain() const override final;

protected:
  virtual QAudio::Error createDevice(const QAudioFormat &format,
                                     const QAudioDevice &info,
                                     int bufferSizeMS) override final;

private slots:
  void errorString(QString);
  void stateChanged(QAudio::State state);

private:
  QAudioSink *mAudioSink; ///< Audio sink to which the data is sent
};

#endif // AUDIOPLAYERFORQT_H
