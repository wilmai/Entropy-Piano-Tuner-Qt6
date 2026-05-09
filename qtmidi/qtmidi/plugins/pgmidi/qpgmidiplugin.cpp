#include "qpgmidiplugin.h"
#include "qpgmidiinput.h"
#include "qpgmidioutput.h"
#include <qtmidi/qmidideviceinfo.h>
#include <QtCore/qtimer.h>

QT_BEGIN_NAMESPACE

QPGMidiPlugin::QPGMidiPlugin(QObject *parent)
    : QMidiSystemPlugin(parent)
    , mWrapper(*this)
{
    // call start later, this will notify on default attached devices
    // dont call now, then the plugin is not registered yet and will be recreated
    // infinitely
    QTimer::singleShot(10, this, SLOT(start()));
}

void QPGMidiPlugin::start() {
    mWrapper.start();
}

QList<QByteArray> QPGMidiPlugin::availableDevices(QMidi::Mode mode) const {
    return mWrapper.availableDevices(mode);
}

QAbstractMidiInput* QPGMidiPlugin::createInput(const QMidiDeviceInfo& info) {
    return new QPGMidiInput(info, mWrapper);
}

QAbstractMidiOutput* QPGMidiPlugin::createOutput(const QMidiDeviceInfo &info) {
    return new QPGMidiOutput(info, mWrapper);
}

QString QPGMidiPlugin::deviceName(const QByteArray &device, QMidi::Mode mode) {
    Q_UNUSED(mode);
    return QString::fromUtf8(device);
}

void QPGMidiPlugin::notifyAttached(const QByteArray &handle, QMidi::Mode mode) {
    QMidiDeviceInfo info = deviceInfo("pgmidi", handle, mode);
    notifyDeviceAttached(info, mode);
}

void QPGMidiPlugin::notifyDetached(const QByteArray &handle, QMidi::Mode mode) {
    QMidiDeviceInfo info = deviceInfo("pgmidi", handle, mode);
    notifyDeviceDetached(info, mode);
}

QT_END_NAMESPACE
