#include "qtmidi/qmidioutput.h"

#include "qtmidi/qmidi.h"
#include "qtmidi/qmidideviceinfo.h"
#include "qtmidi/qmidisystem.h"
#include "qtmidi/internal/qmididevicefactory.h"
#include "qtmidi/internal/qmidisystemnotifierprivate.h"

QT_BEGIN_NAMESPACE

/*!
    \class QMidiOutput
    \brief The QMidiOutput class provides interface for sending midi events to a midi output device.

    \inmodule QtMidi
    \ingroup midi
    \ingroup midi_audio

    You can construct a midi output with a
    \l{QMidiDeviceInfo::defaultOutputDevice()}{default midi output device}.
    It is also possible to create QMidiOutput with a specific QMidiDeviceInfo.

    QMidiOutput lets you send midi signals to a midi output device.
    The default constructor of this class will use the last connected
    midi device, but you can also specify a QMidiDeviceInfo for a specific device.

 */

/*!
    Construct a new midi output and attach it to \a parent.
    The device referenced by \a midiDeviceInfo is used.
    Upon construction \l{QMidiSystemNotifier::outputDeviceCreated} will be emitted.
 */
QMidiOutput::QMidiOutput(const QMidiDeviceInfo &midiDeviceInfo, QObject *parent)
    : QMidiDevice(QMidiDeviceFactory::createOutputDevice(midiDeviceInfo), parent)
{
    emit QMidiSystemNotifierPrivate::instance()->outputDeviceCreated(this);
}

/*!
    Construct a new midi output and attach it to \a parent.
    The last available midi output device is used.
    Upon construction \l{QMidiSystemNotifier::outputDeviceCreated} will be emitted.
 */
QMidiOutput::QMidiOutput(QObject *parent)
    : QMidiOutput(QMidiDeviceFactory::defaultOutputDevice(), parent)
{
    
}

/*!
    Destroy this audio output.
    This will emit \l{QMidiSystemNotifier::outputDeviceDeleted}
 */
QMidiOutput::~QMidiOutput()
{
    emit QMidiSystemNotifierPrivate::instance()->outputDeviceDeleted(deviceInfo());
}

/*!
    Receives a midi message \a m and forwards it to the midi output device.
 */
void QMidiOutput::receiveMidiMessage(const QMidiMessage &m) {
    d->handleMidiMessage(m);
}

/*!
    If you set \a force to true, all midi suitable messages will be sent to
    the output device to the defined \a channel.
    Suitable midi messages are e.g. midi press or release.
 */
void QMidiOutput::setForceTargetChannel(bool force, quint8 channel) {
    QAbstractMidiOutput *amo = static_cast<QAbstractMidiOutput*>(d);
    amo->setForceTargetChannel(force, channel);
}

QT_END_NAMESPACE
