#include "audiointerfaceforqt.h"

#include <QMediaDevices>
#include <QSettings>

#include "core/audio/pcmdevice.h"

const int AudioInterfaceForQt::DEFAULT_BUFFER_SIZE_MS(100);

AudioInterfaceForQt::AudioInterfaceForQt(AudioMode mode, QObject *parent)
    : QObject(parent), mMode(mode),
      mSettingsPrefix(mode == AudioMode::Input ? "audio/input/"
                                               : "audio/output/"),
      mPCMDevice(this) {
  // these settings are fixed
  // mFormat.setCodec("audio/pcm"); // Default in Qt 6
  mFormat.setSampleFormat(QAudioFormat::Int16);

  // default values, these can be changed
  mFormat.setSampleRate(mode == AudioMode::Input ? 44100 : 22050);
  mFormat.setChannelCount(mode == AudioMode::Input ? 1 : 2);
}

void AudioInterfaceForQt::reinitialize(int samplingRate, int channelCount,
                                       QAudioDevice deviceInfo,
                                       int bufferSizeMS) {
  exit();

  mDeviceInfo = deviceInfo;
  mFormat.setSampleRate(samplingRate);
  mFormat.setChannelCount(channelCount);

  // only necessary if default settings
  if (!deviceInfo.isFormatSupported(mFormat)) {
    LogW("Raw audio format not supported by backend, trying to proceed anyway "
         "or use preferred.");
    // mFormat = deviceInfo.nearestFormat(mFormat); // Not available in Qt 6
    // In Qt 6 we can try to use preferred format if specific one fails, but
    // usually Int16 PCM is supported.

    if (!deviceInfo.isFormatSupported(mFormat)) {
      // Try preferred format as fallback
      QAudioFormat preferred = deviceInfo.preferredFormat();
      if (preferred.isValid()) {
        LogW("Using preferred format: rate=%d, channels=%d, format=%d",
             preferred.sampleRate(), preferred.channelCount(),
             preferred.sampleFormat());
        // We only accept if it matches our critical requirements (Int16)
        if (preferred.sampleFormat() == QAudioFormat::Int16) {
          mFormat = preferred;
        }
      }
    }

    if (mFormat.sampleFormat() != QAudioFormat::Int16) {
      LogW("Sample format not supported (requires Int16)");
      // return; // Try to proceed, QAudioSink might convert
    }
  }

  QAudio::Error err = createDevice(mFormat, deviceInfo, bufferSizeMS);
  if (err != QAudio::NoError) {
    LogE("Error creating audio device with error %d", err);
    return;
  }

  LogI("Initialized audio using device: %s", getDeviceName().c_str());

  QSettings s;
  s.setValue(mSettingsPrefix + "samplerate",
             QVariant::fromValue(mFormat.sampleRate()));
  s.setValue(mSettingsPrefix + "channels",
             QVariant::fromValue(mFormat.channelCount()));
  s.setValue(mSettingsPrefix + "devicename",
             QVariant::fromValue(mDeviceInfo.description()));
  s.setValue(mSettingsPrefix + "buffersize", QVariant::fromValue(bufferSizeMS));
}

void AudioInterfaceForQt::init() {
  QSettings s;

  // get device info
  mDeviceInfo = QAudioDevice(); // Null device
  QString deviceName = s.value(mSettingsPrefix + "devicename").toString();

  // Helper to find device
  auto findDevice = [&](const QList<QAudioDevice> &devices) {
    if (deviceName.isEmpty())
      return QAudioDevice();
    for (const auto &d : devices) {
      if (d.description() == deviceName)
        return d;
    }
    return QAudioDevice();
  };

  if (mMode == AudioMode::Input) {
    QList<QAudioDevice> devices = QMediaDevices::audioInputs();
    mDeviceInfo = findDevice(devices);
    if (mDeviceInfo.isNull())
      mDeviceInfo = QMediaDevices::defaultAudioInput();
  } else {
    QList<QAudioDevice> devices = QMediaDevices::audioOutputs();
    mDeviceInfo = findDevice(devices);
    if (mDeviceInfo.isNull())
      mDeviceInfo = QMediaDevices::defaultAudioOutput();
  }

  // get format
  mFormat.setSampleRate(s.value(mSettingsPrefix + "samplerate",
                                QVariant::fromValue(mFormat.sampleRate()))
                            .toInt());
  mFormat.setChannelCount(s.value(mSettingsPrefix + "channels",
                                  QVariant::fromValue(mFormat.channelCount()))
                              .toInt());

  // these settings are required
  if (mMode == AudioMode::Input) {
    mFormat.setChannelCount(1);
  }

  // buffer size
  int bufferSize = s.value(mSettingsPrefix + "buffersize",
                           QVariant::fromValue(DEFAULT_BUFFER_SIZE_MS))
                       .toInt();

  // initialize device
  reinitialize(mFormat.sampleRate(), mFormat.channelCount(), mDeviceInfo,
               bufferSize);
}

const std::string AudioInterfaceForQt::getDeviceName() const {
  return mDeviceInfo.description().toStdString();
}

int AudioInterfaceForQt::getSamplingRate() const {
  return mFormat.sampleRate();
}

int AudioInterfaceForQt::getChannelCount() const {
  return mFormat.channelCount();
}

PCMDevice *AudioInterfaceForQt::getDevice() const {
  return mPCMDevice.getDevice();
}

void AudioInterfaceForQt::setDevice(PCMDevice *device) {
  mPCMDevice.setDevice(device);
}

int AudioInterfaceForQt::getBufferSizeMS() const {
  QSettings s;
  return s
      .value(mSettingsPrefix + "buffersize",
             QVariant::fromValue(DEFAULT_BUFFER_SIZE_MS))
      .toInt();
}
