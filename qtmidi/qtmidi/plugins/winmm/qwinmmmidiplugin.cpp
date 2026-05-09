#include "qwinmmmidiplugin.h"
#include "qwinmmmidiinput.h"
#include "qwinmmmidioutput.h"
#include "qtmidi/internal/qmidipluginloader.h"

QT_BEGIN_NAMESPACE

QWinMMMidiPlugin::QWinMMMidiPlugin(QObject* parent)
    : QMidiSystemPlugin(true, parent)
    , mInputBackend(*this)
    , mOutputBackend(*this)
{}

QWinMMMidiPlugin::~QWinMMMidiPlugin()
{}

QList<QByteArray> QWinMMMidiPlugin::availableDevices(QMidi::Mode mode) const {
    if (mode == QMidi::MidiInput) {
        return mInputBackend.listAvailableDevices();
    } else {
        return mOutputBackend.listAvailableDevices();
    }
}

QAbstractMidiInput* QWinMMMidiPlugin::createInput(const QMidiDeviceInfo& info) {
    return new QWinMMMidiInput(info, mInputBackend);
}

QAbstractMidiOutput* QWinMMMidiPlugin::createOutput(const QMidiDeviceInfo& info) {
    return new QWinMMMidiOutput(info, mOutputBackend);
}

QString QWinMMMidiPlugin::deviceName(const QByteArray& device, QMidi::Mode mode) {
    if (mode == QMidi::MidiInput) {
        return mInputBackend.handleToName(device);
    } else {
        return mOutputBackend.handleToName(device);
    }
}

QT_END_NAMESPACE
