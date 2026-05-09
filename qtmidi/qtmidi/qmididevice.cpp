#include "qmididevice.h"
#include "qmidisystem.h"

QT_BEGIN_MOC_NAMESPACE



/*!
    \class QMidiDevice
    \brief Abstract QMidiDevice class that is inherited by QMidiInput and QMidiOutput devices.

    \inmodule QtMidi
    \ingroup midi
    \ingroup midi_audio

    \sa QMidiInput, QMidiOutput
 */

/*!
    \internal
 */
QMidiDevice::QMidiDevice(QAbstractMidiDevice *ad, QObject *parent)
    : QObject(parent)
    , d(ad)
{
    connect(d, &QAbstractMidiDevice::requestDelete, this, &QMidiDevice::deleteLater);
}

/*!
    \internal
 */
QMidiDevice::~QMidiDevice()
{
    delete d;
}

/*!
    Returns the error state.
 */
QMidi::Error QMidiDevice::error() const
{
    return d->error();
}

/*!
    Returns the device state.
 */
QMidi::State QMidiDevice::state() const
{
    return d->state();
}

/*!
    Returns the device info.
 */
const QMidiDeviceInfo& QMidiDevice::deviceInfo() const
{
    return d->deviceInfo();
}

/*!
    Returns the device name.
    \sa deviceInfo()
 */
QString QMidiDevice::name() {
    return d->deviceInfo().deviceName();
}

/*!
    Handle a midi message \a m. If the underlying implementation is
    a QMidiInput device, this will call QMidiInput::notify, if it is
    a QMidiOutput device, this will forward the message to the midi
    output device.
 */
void QMidiDevice::handleMidiMessage(const QMidiMessage &m) const {
    d->handleMidiMessage(m);
}

QT_END_NAMESPACE
