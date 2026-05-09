#include <QtCore/qdebug.h>

#include "qtmidi/internal/qmididevicefactory.h"
#include "qtmidi/qmidideviceinfo.h"
#include "qtmidi/qmidisystem.h"

QT_BEGIN_NAMESPACE

static void qRegisterMidiDeviceInfoMetaTypes()
{
    qRegisterMetaType<QMidiDeviceInfo>();
}

Q_CONSTRUCTOR_FUNCTION(qRegisterMidiDeviceInfoMetaTypes)

class QMidiDeviceInfoPrivate : public QSharedData
{
public:
    QMidiDeviceInfoPrivate()
        : isNull(true)
        , mode(QMidi::MidiOutput)
    {
    }

    QMidiDeviceInfoPrivate(const QString &r,
                           const QByteArray &h,
                           QMidi::Mode m)
        : isNull(false)
        , realm(r)
        , handle(h)
        , mode(m)
        , deviceName(QMidiDeviceFactory::midiDeviceName(r, h, m))
    {
        if (deviceName.isEmpty() || h.isEmpty()) {
            isNull = true;
        }
    }

    QMidiDeviceInfoPrivate(const QMidiDeviceInfoPrivate &other)
        : QSharedData(other)
        , isNull(other.isNull)
        , realm(other.realm)
        , handle(other.handle)
        , mode(other.mode)
        , deviceName(other.deviceName)
    {
    }

    QMidiDeviceInfoPrivate& operator=(const QMidiDeviceInfoPrivate &other)
    {
        isNull = other.isNull;
        realm = other.realm;
        handle = other.handle;
        mode = other.mode;
        deviceName = other.deviceName;
        return *this;
    }

    ~QMidiDeviceInfoPrivate()
    {
    }

    bool isNull;
    QString realm;
    QByteArray handle;
    QMidi::Mode mode;
    QString deviceName;
};

/*!
    \class QMidiDeviceInfo
    \brief The QMidiDeviceInfo class provides an interface to query audio devices and their functionality.
    \inmodule QtMidi
    \ingroup midi

    QMidiDeviceInfo lets you query for midi devices--such as sound cards
    and USB midi connections--that are currently available on the system.
    The midi devices available are dependent on the platform or midi plugins
    installed.

    A QMidiDeviceInfo is used by Qt to construct classes that communicate
    with the device--such as QMidiInput, and QMidiOutput.

    The status functions defaultInputDevice(), defaultOutputDevice(), and
    availableDevices() let you get a list of all available midi devices.
    Devices are fetched according to the value of mode this is specified by
    the \l {QMidi}::Mode enum.
    The QMidiDeviceInfo  returned are only valid for the \l {QMidi}::Mode.

    For instance:

    \snippet midi-snippets/midi.cpp Dumping input devices

    In this code sample, we loop through all devices that are able to receive
    midi events. For each device we find, we simply print the deviceName().

    \sa QMidiOutput, QMidiInput
 */

/*!
    Constructs an empty QMidiDeviceInfo object.
 */
QMidiDeviceInfo::QMidiDeviceInfo()
    : d(new QMidiDeviceInfoPrivate)
{
}

/*!
    Constructs a copy of \a other.
 */
QMidiDeviceInfo::QMidiDeviceInfo(const QMidiDeviceInfo& other)
    : d(other.d)
{
}

/*!
    Destroy this audio device info.
 */
QMidiDeviceInfo::~QMidiDeviceInfo()
{
}

/*!
    Sets the QMidiDeviceInfo object to be equal to \a other.
 */
QMidiDeviceInfo& QMidiDeviceInfo::operator =(const QMidiDeviceInfo& other)
{
    d = other.d;
    return *this;
}

/*!
    Returns true if this QMidiDeviceInfo class represents the same
    midi device as \a other.
 */
bool QMidiDeviceInfo::operator ==(const QMidiDeviceInfo& other) const
{
    /*#if !defined QT_NO_DEBUG
    if (qgetenv("QT_DEBUG_PLUGINS").toInt() > 0) {
        qDebug() << "Comparing:" << *this << "with" << other;
    }
#endif*/

    if (d == other.d) {return true;}
    return d->realm == other.d->realm && d->mode == other.d->mode && d->handle == other.d->handle
           && deviceName() == other.deviceName();
}

/*!
    Returns true if this QMidiDeviceInfo class represents a different
    midi device than \a other
 */
bool QMidiDeviceInfo::operator !=(const QMidiDeviceInfo& other) const
{
    return !operator==(other);
}

#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
/*!
    Unimplemented dummy method to support compare operators in Qt versions before 5.5.0.
    \a other is a dummy parameter
 */
bool QMidiDeviceInfo::operator<(const QMidiDeviceInfo& other) const {
    Q_UNUSED(other);
    Q_UNIMPLEMENTED();
    return false;
}
#endif

/*!
    Returns whether this QMidiDeviceInfo object holds a valid device
    definition.
 */
bool QMidiDeviceInfo::isNull() const {
    return d->isNull;
}

/*!
    Returns the human readable name of the midi device.

    Devices names vary depending on the platform/midi plugin being used.

    They are a unique string identifier for the audio devices.
 */
QString QMidiDeviceInfo::deviceName() const
{
    return d->deviceName;
}

/*!
    Returns the information for the default input midi device.
    If there is no midi device available an invalid QMidiDeviceInfo
    will be returned.
 */
QMidiDeviceInfo QMidiDeviceInfo::defaultInputDevice()
{
    return QMidiDeviceFactory::defaultInputDevice();
}

/*!
    Returns the information for the default output midi device.
    If there is no midi device available an invalid QMidiDeviceInfo
    will be returned.
 */
QMidiDeviceInfo QMidiDeviceInfo::defaultOutputDevice()
{
    return QMidiDeviceFactory::defaultOutputDevice();
}

/*!
    Returns the device info of the given mode and device name if available.
 */
QMidiDeviceInfo QMidiDeviceInfo::deviceInfoByName(QMidi::Mode mode, QString deviceName) {
    auto devices = availableDevices(mode);
    for (auto devInfo : devices) {
        if (devInfo.deviceName() == deviceName) {
            return devInfo;
        }
    }

    return QMidiDeviceInfo();
}

/*!
    Returns a list of midi devices that support \a mode.
 */
QList<QMidiDeviceInfo> QMidiDeviceInfo::availableDevices(QMidi::Mode mode)
{
    return QMidiDeviceFactory::availableDevices(mode);
}

/*!
    \internal
 */
QMidiDeviceInfo::QMidiDeviceInfo(const QString &realm, const QByteArray &handle, QMidi::Mode mode)
    : d(new QMidiDeviceInfoPrivate(realm, handle, mode))
{
}

/*!
    \internal
 */
QString QMidiDeviceInfo::realm() const
{
    return d->realm;
}

/*!
    \internal
 */
QByteArray QMidiDeviceInfo::handle() const
{
    return d->handle;
}

/*!
    \internal
 */
QMidi::Mode QMidiDeviceInfo::mode() const
{
    return d->mode;
}

#ifndef QT_NO_DEBUG_STREAM

/*!
    Prints a QMidiDeviceInfo to the debug output stream.
 */
QDebug operator<<(QDebug dbg, const QMidiDeviceInfo& info) {
    QDebugStateSaver saver(dbg);
    dbg.nospace();
    return dbg << "{QMidiDeviceInfo {realm: " << info.realm() << ", mode: " << info.mode() << ", handle: " << QString::fromUtf8(info.handle()) << ", name: " << info.deviceName() << "}";
}
#endif

QT_END_NAMESPACE
