#include <QtCore/qdebug.h>

#include "qtmidi/qmidi.h"
#include "qtmidi/qmidisystemplugin.h"
#include "qtmidi/qmidiinput.h"
#include "qtmidi/internal/qmidipluginloader.h"
#include "qtmidi/internal/qmididevicefactory.h"
#include "qtmidi/internal/qmidisystemnotifierprivate.h"

QT_BEGIN_NAMESPACE

void QMidiDeviceFactory::instantiate() {
    QMidiPluginLoader::instance().getOrCreate();
}

class QNullInputDevice : public QAbstractMidiInput
{
public:
    QNullInputDevice(const QMidiDeviceInfo &deviceInfo)
        : QAbstractMidiInput(deviceInfo)
    {}

    QMidi::Error error() const override { return QMidi::OpenError; }
    QMidi::State state() const override { return QMidi::DisconnectedState; }
};

class QNullOutputDevice : public QAbstractMidiOutput
{
public:
    QNullOutputDevice(const QMidiDeviceInfo &deviceInfo)
        : QAbstractMidiOutput(deviceInfo)
    {}

    QMidi::Error error() const override { return QMidi::OpenError; }
    QMidi::State state() const override { return QMidi::DisconnectedState; }
    void sendMidiMessage(const QMidiMessage&) const override {}
};

QMidiDeviceInfo QMidiDeviceFactory::createDeviceInfo(const QString& realm, const QByteArray& handle, QMidi::Mode mode)
{
    return QMidiDeviceInfo(realm, handle, mode);
}

QList<QByteArray> QMidiDeviceFactory::availableDeviceHandles(const QString& realm, QMidi::Mode mode)
{
    QMidiSystemFactoryInterface* plugin = QMidiPluginLoader::instance().getOrCreate();

    if (plugin) {
        return plugin->availableDevices(mode);
    }

    return QList<QByteArray>();
}

QList<QMidiDeviceInfo> QMidiDeviceFactory::availableDevices(QMidi::Mode mode)
{
    QList<QMidiDeviceInfo> devices;
    QMidiSystemFactoryInterface* plugin = QMidiPluginLoader::instance().getOrCreate();

    if (plugin) {
        const auto availableDevices = plugin->availableDevices(mode);
        for (const QByteArray& handle : availableDevices)
            devices << QMidiDeviceInfo("default", handle, mode);
    }

    return devices;
}

QMidiDeviceInfo QMidiDeviceFactory::defaultInputDevice()
{
    QMidiSystemFactoryInterface*plugin = QMidiPluginLoader::instance().getOrCreate();
    if (plugin) {
        QList<QByteArray> list = plugin->availableDevices(QMidi::MidiInput);
        if (list.size() > 0)
            return QMidiDeviceInfo("default", list.back(), QMidi::MidiInput);
    }

    return QMidiDeviceInfo();
}

QMidiDeviceInfo QMidiDeviceFactory::defaultOutputDevice()
{
    QMidiSystemFactoryInterface*plugin = QMidiPluginLoader::instance().getOrCreate();
    if (plugin) {
        QList<QByteArray> list = plugin->availableDevices(QMidi::MidiOutput);
        if (list.size() > 0)
            return QMidiDeviceInfo("default", list.at(0), QMidi::MidiOutput);
    }

    return QMidiDeviceInfo();
}

QString QMidiDeviceFactory::midiDeviceName(const QString &realm, const QByteArray &handle, QMidi::Mode mode)
{
    QMidiSystemFactoryInterface*plugin = QMidiPluginLoader::instance().getOrCreate();

    if (plugin)
        return plugin->deviceName(handle, mode);

    return QString();
}

QAbstractMidiInput* QMidiDeviceFactory::createDefaultInputDevice()
{
    return createInputDevice(defaultInputDevice());
}

QAbstractMidiOutput* QMidiDeviceFactory::createDefaultOutputDevice()
{
    return createOutputDevice(defaultOutputDevice());
}

QAbstractMidiInput* QMidiDeviceFactory::createInputDevice(QMidiDeviceInfo const& deviceInfo)
{
    if (deviceInfo.handle().isNull())
        return new QNullInputDevice(deviceInfo);

    QMidiSystemFactoryInterface*plugin = QMidiPluginLoader::instance().getOrCreate();

    QAbstractMidiInput* p = nullptr;
    if (plugin) {
        p = plugin->createInput(deviceInfo);
    }
    if (!p) {
        return new QNullInputDevice(deviceInfo);
    } else {
        return p;
    }
}

QAbstractMidiOutput* QMidiDeviceFactory::createOutputDevice(QMidiDeviceInfo const& device)
{
    if (device.handle().isNull())
        return new QNullOutputDevice(device);

    QMidiSystemFactoryInterface*plugin = QMidiPluginLoader::instance().getOrCreate();

    QAbstractMidiOutput* p = nullptr;
    if (plugin) {
        p = plugin->createOutput(device);
    }

    if (p) {return p;}
    else {return new QNullOutputDevice(device);}
}

QT_END_NAMESPACE
