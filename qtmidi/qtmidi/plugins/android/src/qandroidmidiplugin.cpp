#include "qandroidmidiplugin.h"
#include "qandroidmidinativewrapper.h"
#include <qtmidi/qmidideviceinfo.h>
#include "qandroidmidiinput.h"
#include "qandroidmidioutput.h"

QT_BEGIN_NAMESPACE

QAndroidMidiPlugin *QAndroidMidiPlugin::thiz = nullptr;

QAndroidMidiPlugin::QAndroidMidiPlugin(QObject* parent)
    : QMidiSystemPlugin(parent)
{
    thiz = this;
    android_registerNatives();
    initAndroidManagerJNI(&mUsbManager);
}

QAndroidMidiPlugin::~QAndroidMidiPlugin()
{
    thiz = nullptr;
    releaseAndroidManagerJNI(mUsbManager);
    mUsbManager = QJniObject();
}

QList<QByteArray> QAndroidMidiPlugin::availableDevices(QMidi::Mode mode) const {
    QStringList deviceNames;
    if (mode == QMidi::MidiInput) {
        deviceNames = android_listAvailableInputDevices(mUsbManager);
    } else {
        deviceNames = android_listAvailableOutputDevices(mUsbManager);
    }

    QList<QByteArray> out;
    for (QString name : deviceNames) {
        out.append(toHandle(name, mode));
    }
    return out;
}

QAbstractMidiInput* QAndroidMidiPlugin::createInput(const QMidiDeviceInfo& info) {
    return new QAndroidMidiInput(info, mUsbManager);
}

QAbstractMidiOutput* QAndroidMidiPlugin::createOutput(const QMidiDeviceInfo& info) {
    return new QAndroidMidiOutput(info, mUsbManager);
}

QString QAndroidMidiPlugin::deviceName(const QByteArray& device, QMidi::Mode mode) {
    return toDeviceName(device) + QString::fromUtf8(" [%1]").arg(mode);
}

QString QAndroidMidiPlugin::toDeviceName(const QByteArray &handle) {
    if (handle.isEmpty()) {return QString();}
    return handle.left(handle.size() - 1);
}

QByteArray QAndroidMidiPlugin::toHandle(const QString &deviceName, QMidi::Mode mode) {
    if (deviceName.isEmpty()) {return QByteArray();}
    return (deviceName + QString::number(mode)).toUtf8();
}

void QAndroidMidiPlugin::fromNativeHandleInputMessage(const QString &deviceName, const QMidiMessage &m) {
    QMutexLocker lock(&thiz->mConnectedDevicesMutex);

    auto handle = toHandle(deviceName, QMidi::MidiInput);
    for (auto pair : thiz->mConnectedDevices) {
        if (pair.first == handle) {
            pair.second->handleMidiMessage(m);
        }
    }
}

void QAndroidMidiPlugin::deviceAttached(const QString &deviceName, QMidi::Mode mode) {
    QMidiDeviceInfo info = thiz->deviceInfo("android", toHandle(deviceName, mode), mode);
    thiz->notifyDeviceAttached(info, mode);
}

void QAndroidMidiPlugin::deviceDetached(const QString &deviceName, QMidi::Mode mode) {
    QMidiDeviceInfo info = thiz->deviceInfo("android", toHandle(deviceName, mode), mode);
    thiz->notifyDeviceDetached(info, mode);
}

void QAndroidMidiPlugin::registerDevice(const QByteArray &handle, const QAndroidMidiInput *input) {
    QMutexLocker lock(&mConnectedDevicesMutex);

    mConnectedDevices.append(qMakePair(handle, input));
}

void QAndroidMidiPlugin::removeDevice(const QAndroidMidiInput *input) {
    QMutexLocker lock(&mConnectedDevicesMutex);

    for (auto it = mConnectedDevices.begin(); it != mConnectedDevices.end();) {
        if (it->second == input) {
            it = mConnectedDevices.erase(it);
        } else {
            ++it;
        }
    }
}


QT_END_NAMESPACE
