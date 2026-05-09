#include "qrtmidiplugin.h"

QT_BEGIN_NAMESPACE

QRtMidiPlugin::QRtMidiPlugin(QObject *parent)
    : QMidiSystemPlugin(parent)
{
}

QList<QByteArray> QRtMidiPlugin::availableDevices(QMidi::Mode mode) const
{
    return QRtMidiDeviceInfo::availableDevices(mode);
}

QAbstractMidiInput* QRtMidiPlugin::createInput(const QByteArray& device)
{
    return QRtMidiInput(device);
}

QAbstractMidiOutput* QRtMidiPlugin::createOutput(const QByteArray& device)
{
    return QRtMidiOutput(device);
}

QAbstractMidiDeviceInfo* QRtMidiPlugin::createDeviceInfo(const QByteArray& device, QMidi::Mode mode)
{
    return QRtMidiDeviceInfo(device, mode);
}

QT_END_NAMESPACE
