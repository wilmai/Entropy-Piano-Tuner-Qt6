#include "qtmidi/qmidisystemplugin.h"

#include "qtmidi/internal/qmidisystemnotifierprivate.h"
#include "qtmidi/internal/qmididevicefactory.h"

QT_BEGIN_NAMESPACE

QMidiSystemFactoryInterface::~QMidiSystemFactoryInterface()
{}

// TODO: doc

QMidiSystemPlugin::QMidiSystemPlugin(bool supportsDeviceWatcher, QObject *parent)
    : QObject(parent),
    mSupportsDeviceWatcher(supportsDeviceWatcher)
{
    if (mSupportsDeviceWatcher)
    {
        new QMidiDeviceWatcher(QMidi::MidiInput, 1000, parent);
        new QMidiDeviceWatcher(QMidi::MidiOutput, 1000, parent);
    }
}

QMidiSystemPlugin::~QMidiSystemPlugin()
{}

QMidiDeviceInfo QMidiSystemPlugin::deviceInfo(const QString &realm, const QByteArray &handle, QMidi::Mode mode) {
    return QMidiDeviceFactory::createDeviceInfo(realm, handle, mode);
}

void QMidiSystemPlugin::notifyInputDeviceAttached(const QMidiDeviceInfo& deviceInfo) {
    QMidiSystemNotifierPrivate::instance()->inputDeviceAttached(deviceInfo);
}

void QMidiSystemPlugin::notifyOutputDeviceAttached(const QMidiDeviceInfo& deviceInfo) {
    QMidiSystemNotifierPrivate::instance()->outputDeviceAttached(deviceInfo);
}

void QMidiSystemPlugin::notifyInputDeviceDetached(const QMidiDeviceInfo& deviceInfo) {
    QMidiSystemNotifierPrivate::instance()->inputDeviceDetached(deviceInfo);
}

void QMidiSystemPlugin::notifyOutputDeviceDetached(const QMidiDeviceInfo& deviceInfo) {
    QMidiSystemNotifierPrivate::instance()->outputDeviceDetached(deviceInfo);
}

void QMidiSystemPlugin::notifyDeviceAttached(const QMidiDeviceInfo& deviceInfo, QMidi::Mode mode) {
    if (mode == QMidi::MidiInput) {
        notifyInputDeviceAttached(deviceInfo);
    } else {
        notifyOutputDeviceAttached(deviceInfo);
    }
}

void QMidiSystemPlugin::notifyDeviceDetached(const QMidiDeviceInfo& deviceInfo, QMidi::Mode mode) {
    if (mode == QMidi::MidiInput) {
        notifyInputDeviceDetached(deviceInfo);
    } else {
        notifyOutputDeviceDetached(deviceInfo);
    }
}

QT_END_NAMESPACE

#include "moc_qmidisystemplugin.cpp"
