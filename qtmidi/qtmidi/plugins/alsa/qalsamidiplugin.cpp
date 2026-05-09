#include "qalsamidiplugin.h"
#include "qalsamidideviceinfo.h"
#include "qalsamidiinput.h"
#include "qalsamidioutput.h"
#include "qalsamidiinbackend.h"
#include "qalsamidioutbackend.h"

QT_BEGIN_NAMESPACE

QAlsaMidiPlugin::QAlsaMidiPlugin(QObject *parent)
    : QMidiSystemPlugin(parent)
{
    mIn = new QAlsaMidiInBackend(this);
    mOut = new QAlsaMidiOutBackend(this);
}

QAlsaMidiPlugin::~QAlsaMidiPlugin()
{
    delete mIn;
    delete mOut;
}

QList<QByteArray> QAlsaMidiPlugin::availableDevices(QMidi::Mode mode) const
{
    if (mode == QMidi::MidiInput) {
        return mIn->availableDevices();
    } else {
        return mOut->availableDevices();
    }
}

QAbstractMidiInput* QAlsaMidiPlugin::createInput(const QMidiDeviceInfo& info)
{
    return new QAlsaMidiInput(info, mIn);
}

QAbstractMidiOutput* QAlsaMidiPlugin::createOutput(const QMidiDeviceInfo& info)
{
    return new QAlsaMidiOutput(info, mOut);
}

QString QAlsaMidiPlugin::deviceName(const QByteArray& device, QMidi::Mode mode)
{
    if (mode == QMidi::MidiInput) {
        return mIn->deviceName(device);
    } else {
        return mOut->deviceName(device);
    }
}

QT_END_NAMESPACE
