#include "qtmidi/qmidi.h"
#include "qtmidi/qmidiinput.h"
#include "qtmidi/qmidideviceinfo.h"
#include "qtmidi/qmidisystem.h"

#include "qtmidi/internal/qmididevicefactory.h"
#include "qtmidi/internal/qmidisystemnotifierprivate.h"

QT_BEGIN_NAMESPACE

/*!
    \class QMidiInput
    \brief The QMidiInput class provides interface for receiving midi events from a midi input device.

    \inmodule QtMidi
    \ingroup midi
    \ingroup midi_audio

    You can construct a midi input with a
    \l{QMidiDeviceInfo::defaultInputDevice()}{default midi input device}.
    It is also possible to create QMidiInput with a specific QMidiDeviceInfo.

    QMidiInput lets you reveive midi signals from a midi input device.
    The default constructor of this class will use the last connected
    midi device, but you can also specify a QMidiDeviceInfo for a specific device.

    As soon as you create a QMidiInput it will listen to all of the events of the
    midi device and sent the message as QMidiMessage emitting the notify() signal.

 */

/*!
    Construct a new midi input and attach it to \a parent.
    The device referenced by \a midiDeviceInfo is used.
    Upon construction \l{QMidiSystemNotifier::inputDeviceCreated} will be emitted.
 */
QMidiInput::QMidiInput(const QMidiDeviceInfo &midiDeviceInfo, QObject *parent)
    : QMidiDevice(QMidiDeviceFactory::createInputDevice(midiDeviceInfo), parent)
{
    QAbstractMidiInput *ami = static_cast<QAbstractMidiInput*>(d);
    connect(ami, &QAbstractMidiInput::notify, this, &QMidiInput::notify);

    emit QMidiSystemNotifierPrivate::instance()->inputDeviceCreated(this);
}

/*!
    Construct a new midi input and attach it to \a parent.
    The last available midi input device is used.
    Upon construction \l{QMidiSystemNotifier::inputDeviceCreated} will be emitted.
 */
QMidiInput::QMidiInput(QObject *parent)
    : QMidiInput(QMidiDeviceFactory::defaultInputDevice(), parent)
{
}

/*!
    Destroy this audio input.
    This will emit \l{QMidiSystemNotifier::inputDeviceDeleted}
 */
QMidiInput::~QMidiInput()
{
    emit QMidiSystemNotifierPrivate::instance()->inputDeviceDeleted(deviceInfo());
}

/*!
     Sets whether to ignore system messages.
     If \a ignore is set to true notify will stop emitting midi system messages.
 */
void QMidiInput::setIgnoreSystemMessages(bool ignore) {
    QAbstractMidiInput *ami = static_cast<QAbstractMidiInput*>(d);
    ami->setIgnoreSystemMessages(ignore);
}

/*!
    \fn QMidiInput::notify(const QMidiMessage &m)
    This signal is emitted when the device sent a midi message \a m.
    Connect to this signal to listen to midi events.
*/

QT_END_NAMESPACE
