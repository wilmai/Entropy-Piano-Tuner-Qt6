#ifndef OPTIONSPAGEAUDIOINPUTOUTPUTPAGE_H
#define OPTIONSPAGEAUDIOINPUTOUTPUTPAGE_H

#include <QAudioDevice> // Added for Qt 6
#include <QMediaDevices>
#include <QSpinBox>
#include <QThread>

#include "prerequisites.h"

#include "../optionscontentswidgetinterface.h"

#include "audioforqt/audiointerfaceforqt.h"

namespace options {

///
/// \brief Page of the audio options that displays content for input or output
/// device
///
/// If this is for the input device, the user can set the device and its sample
/// rate.
///
/// If this is for the output device, the user can set the device and its sample
/// rate. Moreover the user can set the number of output channels and the buffer
/// size for the synthesizer.
///
class PageAudioInputOutput : public QWidget, public ContentsWidgetInterface {
  Q_OBJECT
public:
  ///
  /// \brief Create a input or output options audio page
  /// \param optionsDialog The options dialog
  /// \param mode The mode of the audio settings page
  ///
  PageAudioInputOutput(OptionsDialog *optionsDialog,
                       AudioInterfaceForQt::AudioMode mode);

  virtual ~PageAudioInputOutput();

  ///
  /// \brief Stops the audio, changes the device and restarts the audio
  ///
  void apply() override final;

signals:
  void updateProgress(int percentage);

private slots:
  ///
  /// \brief Called if the device (input/output) changed
  ///
  void onDeviceSelectionChanged(int);

  ///
  /// \brief Call if the user wants to select the default device
  ///
  void onDefaultDevice();

  ///
  /// \brief Call to select the default sampling rate of the current selected
  /// device
  ///
  void onDefaultSamplingRate();

  void addDevice(QAudioDevice info);

  // special audio output
  // ====================================================================

  ///
  /// \brief Call to select the default channels (max = 2)
  ///
  void onDefaultChannel();

  ///
  /// \brief Call to set the buffer size to its default
  ///
  void onDefaultBufferSize();

private:
  OptionsDialog *mOptionsDialog; ///< Pointer to the options dialog
  AudioInterfaceForQt
      *mAudioInterface; ///< Pointer to the audio base (input/output)
  AudioInterfaceForQt::AudioMode mMode; ///< Mode of this page

  QComboBox *mDeviceSelection; ///< Item to select the device
  QComboBox *mSamplingRates;   ///< Item to select the sampling rate

  QThread *mDeviceLoader; ///< Thread to load devices

  // special audio output
  // =====================================================================
  QComboBox *mChannelsSelect; ///< Item to select the number of channels
  QSpinBox *mBufferSizeEdit;  ///< Item to select the buffer size for output
};

class DeviceLoaderThread : public QThread {
  Q_OBJECT
public:
  DeviceLoaderThread(QObject *parent, AudioInterfaceForQt::AudioMode mode);

private:
  virtual void run() override final;

signals:
  void updateProgress(int percentageCompleted);
  void deviceReady(QAudioDevice device);

private:
  AudioInterfaceForQt::AudioMode mMode;
};

} // namespace options

#endif // OPTIONSPAGEAUDIOINPUTOUTPUTPAGE_H
