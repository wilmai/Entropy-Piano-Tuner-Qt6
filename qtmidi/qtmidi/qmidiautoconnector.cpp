#include "qtmidi/qmidiautoconnector.h"

#include <QtCore/qlist.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QDebug>

#include "qtmidi/qmidisystemnotifier.h"
#include "qtmidi/internal/qmidiautoconnectorprivate.h"

QT_BEGIN_NAMESPACE

/*!
    \class QMidiAutoConnector
    \brief The QMidiAutoConnector class provides an optional tool to automatically connect to midi input or output device.

    \inmodule QtMidi
    \ingroup midi
    \ingroup midi_audio


    The QMidiAutoConnector can be used to automatically connect to available input and output midi devices.
    You can specify if there may only be one device of the given QMidi::Mode connected at the same time via
    \l setAllowMultiDevices.

    The following snippet shows how you can create a QMidiAutoConnector and listen to deviceCreated and deviceDeleted events.

    \snippet midi-snippets/midi.cpp Auto connector

    In you custom implementation you should listen to the deviceCreated signal and connect the \l QMidiInput::notify method to
    you \l QMidiMessage listener.

    The \l setForceSingleDevicem and \l setAllowMultiDevices are inverse functions.
 */

/*!
    Construct a QMidiAutoConnector and attach it to \a parent.
 */
QMidiAutoConnector::QMidiAutoConnector(QObject *parent)
    : QObject(parent)
{
    d = QMidiAutoConnectorPrivate::instance();

    connect(d, &QMidiAutoConnectorPrivate::autoConnectChanged, this, &QMidiAutoConnector::autoConnectChanged);
    connect(d, &QMidiAutoConnectorPrivate::autoConnectToInputChanged, this, &QMidiAutoConnector::autoConnectToInputChanged);
    connect(d, &QMidiAutoConnectorPrivate::autoConnectToOutputChanged, this, &QMidiAutoConnector::autoConnectToOutputChanged);

    connect(d, &QMidiAutoConnectorPrivate::forceSingleDeviceChanged, this, &QMidiAutoConnector::forceSingleDeviceChanged);
    connect(d, &QMidiAutoConnectorPrivate::forceSingleInputDeviceChanged, this, &QMidiAutoConnector::forceSingleInputDeviceChanged);
    connect(d, &QMidiAutoConnectorPrivate::forceSingleOutputDeviceChanged, this, &QMidiAutoConnector::forceSingleOutputDeviceChanged);

    connect(d, &QMidiAutoConnectorPrivate::deviceCreated, this, &QMidiAutoConnector::deviceCreated);
    connect(d, &QMidiAutoConnectorPrivate::inputDeviceCreated, this, &QMidiAutoConnector::inputDeviceCreated);
    connect(d, &QMidiAutoConnectorPrivate::outputDeviceCreated, this, &QMidiAutoConnector::outputDeviceCreated);

    connect(d, &QMidiAutoConnectorPrivate::deviceDeletedLater, this, &QMidiAutoConnector::deviceDeletedLater);
    connect(d, &QMidiAutoConnectorPrivate::inputDeviceDeletedLater, this, &QMidiAutoConnector::inputDeviceDeletedLater);
    connect(d, &QMidiAutoConnectorPrivate::outputDeviceDeletedLater, this, &QMidiAutoConnector::outputDeviceDeletedLater);

    connect(d, &QMidiAutoConnectorPrivate::deviceDeleted, this, &QMidiAutoConnector::deviceDeleted);
    connect(d, &QMidiAutoConnectorPrivate::inputDeviceDeleted, this, &QMidiAutoConnector::inputDeviceDeleted);
    connect(d, &QMidiAutoConnectorPrivate::outputDeviceDeleted, this, &QMidiAutoConnector::outputDeviceDeleted);

    connect(d, &QMidiAutoConnectorPrivate::currentDeviceDetached, this, &QMidiAutoConnector::currentDeviceDetached);
    connect(d, &QMidiAutoConnectorPrivate::currentInputDeviceDetached, this, &QMidiAutoConnector::currentInputDeviceDetached);
    connect(d, &QMidiAutoConnectorPrivate::currentOutputDeviceDetached, this, &QMidiAutoConnector::currentOutputDeviceDetached);
}

/*!
    Destroy this auto connector.
 */
QMidiAutoConnector::~QMidiAutoConnector()
{
    d = nullptr;
}

/*!
    Returns if auto connect is enabled for the given \a mode.
 */
bool QMidiAutoConnector::isAutoConnectEnabled(QMidi::Mode mode) const
{
    return d->isAutoConnectEnabled(mode);
}

/*!
    Returns if only a single device may be connected at the same time for the given \a mode.

    \sa allowMultiDevices
 */
bool QMidiAutoConnector::isSingleDeviceForced(QMidi::Mode mode) const
{
    return d->isSingleDeviceForced(mode);
}

/*!
    Returns if the auto connector is allowed to connect to multiple devices at the same time
    for a given \a mode.

    \sa isSingleDeviceForced
 */
bool QMidiAutoConnector::allowMultiDevices(QMidi::Mode mode) const
{
    return !d->isSingleDeviceForced(mode);
}

QMidiDevice *QMidiAutoConnector::device(const QMidi::Mode mode) const {
    if (d->mCurrentDevices[mode].size() > 0) {
        return d->mCurrentDevices[mode].last();
    }
    return nullptr;
}

/*!
    \fn QList<const DeviceType*> QMidiAutoConnector::devices() const
    Returns all of the current connected devices.

    \sa allowMultiDevices
 */
template <class DeviceType>
QList<const DeviceType*> QMidiAutoConnector::devices() const
{
    QList<const DeviceType*> devices;
    for (auto dev : d->mCurrentDevices[DeviceType::Mode]) {
        devices.push_back(static_cast<const DeviceType*>(dev));
    }
    return devices;
}

template Q_MIDI_EXPORT QList<const QMidiInput*> QMidiAutoConnector::devices() const;

template Q_MIDI_EXPORT QList<const QMidiOutput*> QMidiAutoConnector::devices() const;

/*!
    \fn const DeviceType* QMidiAutoConnector::device() const
    Returns the last connected device.

    \sa isSingleDeviceForced
 */
template <class DeviceType>
const DeviceType* QMidiAutoConnector::device() const {
    if (d->mCurrentDevices[DeviceType::Mode].size() > 0) {
        return static_cast<const DeviceType*>(d->mCurrentDevices[DeviceType::Mode].last());
    }
    return nullptr;
}

template Q_MIDI_EXPORT const QMidiInput* QMidiAutoConnector::device() const;

template Q_MIDI_EXPORT const QMidiOutput* QMidiAutoConnector::device() const;

template <class DeviceType>
const DeviceType* QMidiAutoConnector::device(const QMidiDeviceInfo &info) const {
    for (auto dev : d->mCurrentDevices[DeviceType::Mode]) {
        if (dev->deviceInfo() == info) {
            return static_cast<const DeviceType*>(dev);
        }
    }
    return nullptr;
}

template Q_MIDI_EXPORT const QMidiInput* QMidiAutoConnector::device(const QMidiDeviceInfo &) const;

template Q_MIDI_EXPORT const QMidiOutput* QMidiAutoConnector::device(const QMidiDeviceInfo &) const;

template <class DeviceType>
DeviceType* QMidiAutoConnector::device(const QMidiDeviceInfo &info) {
    for (auto dev : d->mCurrentDevices[DeviceType::Mode]) {
        if (dev->deviceInfo() == info) {
            return static_cast<DeviceType*>(dev);
        }
    }
    return nullptr;
}

template Q_MIDI_EXPORT QMidiInput* QMidiAutoConnector::device(const QMidiDeviceInfo &);

template Q_MIDI_EXPORT QMidiOutput* QMidiAutoConnector::device(const QMidiDeviceInfo &);

/*!
    Restores the internal configuration from the given \a array.
    If \a notify is set to true signals for changed parameters will
    be emitted.

    Usually the \a array should be created by \l store.

    \sa store
 */
void QMidiAutoConnector::restore(const QByteArray &array, bool notify) {
    d->restore(array, notify);
}

/*!
    Returns a QByteArray representing the configuration of the auto connector.
    This can be used to store the current settings.

    \sa restore
 */
QByteArray QMidiAutoConnector::store() const {
    return d->store();
}

/*!
    Set whether to \a autoconnect to devices of \a mode.
    If \a autoconnect is set to true it will instantly check if it shall
    connect to new devices depending on the \l isAutoConnectEnabled state.

    \sa isAutoConnectEnabled, setAutoConnectToInput, setAutoConnectToOutput
 */
void QMidiAutoConnector::setAutoConnect(QMidi::Mode mode, bool autoconnect)
{
    d->setAutoConnect(mode, autoconnect);
}

/*!
    Set whether to \a autoconnect to input devices.

    \sa setAutoConnect
 */
void QMidiAutoConnector::setAutoConnectToInput(bool autoconnect)
{
    setAutoConnect(QMidi::MidiInput, autoconnect);
}

/*!
    Set whether to \a autoconnect to output devices.

    \sa setAutoConnect
 */
void QMidiAutoConnector::setAutoConnectToOutput(bool autoconnect)
{
    setAutoConnect(QMidi::MidiOutput, autoconnect);
}

/*!
    If \a force is true maximal one device of \a mode will be connected
    at the same time.

    If set to true it will disconnect all devices and connect to
    the \l{QMidiInput::QMidiInput()}{default input device}.
 */
void QMidiAutoConnector::setForceSingleDevice(QMidi::Mode mode, bool force)
{
    d->setForceSingleDevice(mode, force);
}

/*!
    If \a force is true maximal one input device will be connected
    at the same time.

    \sa setForceSingleDevice
 */
void QMidiAutoConnector::setForceSingleInputDevice(bool force)
{
    setForceSingleDevice(QMidi::MidiInput, force);
}
/*!
    If \a force is true maximal one input device will be connected
    at the same time.

    \sa setForceSingleDevice
 */
void QMidiAutoConnector::setForceSingleOutputDevice(bool force)
{
    setForceSingleDevice(QMidi::MidiOutput, force);
}

/*!
    If \a allow is set to true the auto connector is allowed to connect to
    multiple devices of type \a mode.
 */
void QMidiAutoConnector::setAllowMultiDevices(QMidi::Mode mode, bool allow)
{
    d->setForceSingleDevice(mode, !allow);
}

/*!
    If \a allow is set to true the auto connector is allowed to
    connect to multiple input devices.

    \sa setAllowMultiDevices
 */
void QMidiAutoConnector::setAllowMultiInputDevices(bool allow)
{
    setAllowMultiDevices(QMidi::MidiInput, allow);
}

/*!
    It \a allow is set to true the auto connector is allowed to
    connect to multiple output devices.

    \sa setAllowMultiDevices
 */
void QMidiAutoConnector::setAllowMultiOutputDevices(bool allow)
{
    setAllowMultiDevices(QMidi::MidiOutput, allow);
}

/*!
    Force the auto connector to connect with the device of type
    \a mode with \a info.

    If \l isSingleDeviceForced is set the auto connector will
    disconnect from the currrent device.
 */
void QMidiAutoConnector::connectWith(const QMidiDeviceInfo &info, QMidi::Mode mode)
{
    d->connectWith(info, mode);
}

/*!
    Force the auto connect to disconnect from the device of type
    \a mode with \a info.

    If \l isAutoConnectEnabled is true ts will connect to a new device
    instantly.
 */
void QMidiAutoConnector::disconnectFrom(const QMidiDeviceInfo &info, QMidi::Mode mode)
{
    d->disconnectFrom(info, mode);
}

/*!
    Force the auto connect to disconnect from all devices of type \a mode.

    If \l isAutoConnectEnabled is true it will connect to the default device
    instantly.
 */
void QMidiAutoConnector::disconnectFromAll(QMidi::Mode mode)
{
    d->disconnectFromAll(mode);
}

/*!
    \fn QMidiAutoConnector::autoConnectChanged(QMidi::Mode mode, bool autoconnect)
    This signal is emitted if the auto connect state of \a mode changed to \a autoconnect.

    \sa setAutoConnect, isAutoConnectEnabled
 */

/*!
    \fn QMidiAutoConnector::forceSingleDeviceChanged(QMidi::Mode mode, bool force)
    This signal is emitted if the state of forcing a single \a mode device changed to \a force.

    \sa setForceSingleDevice, isSingleDeviceForced
 */

/*!
    \fn QMidiAutoConnector::deviceCreated(const QMidiDevice* device, QMidi::Mode mode)
    This signal is emitted if a \a device of type \a mode was created by
    the auto connector.

    \sa inputDeviceCreated, outputDeviceCreated, deviceDeleted
 */

/*!
    \fn QMidiAutoConnector::inputDeviceCreated(const QMidiInput *device)
    This signal is emitted if a input \a device was created.

    \sa deviceCreated, outputDeviceCreated, inputDeviceDeleted
 */

/*!
    \fn QMidiAutoConnector::outputDeviceCreated(const QMidiOutput *device)
    This signal is emitted if a output \a device was created.

    \sa deviceCreated, inputDeviceCreated, outputDeviceDeleted
 */

/*!
    \fn QMidiAutoConnector::deviceDeleted(const QMidiDeviceInfo info, QMidi::Mode mode)
    This signal is emitted if a device with given \a info of \a mode was destroyed
    by the auto connector.

    \sa inputDeviceDeleted, outputDeviceDeleted, deviceCreated
 */

/*!
    \fn QMidiAutoConnector::inputDeviceDeleted(const QMidiDeviceInfo info)
    This signal is emitted if a input device with given \a \info was destroyed.

    \sa deviceDeleted, outputDeviceDeleted, inputDeviceCreated
 */

/*!
    \fn QMidiAutoConnector::outputDeviceDeleted(const QMidiDeviceInfo info)
    This signal is emitted if a output device with given \a info was destroyed.

    \sa deviceDeleted, outputDeviceDeleted, outputDeviceCreated
 */

QT_END_NAMESPACE
