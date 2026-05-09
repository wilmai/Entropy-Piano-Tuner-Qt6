#ifndef QMIDIDEVICEINFO_H
#define QMIDIDEVICEINFO_H

#include <QtCore/qobject.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qlist.h>
#include <QtCore/qshareddata.h>

#include "qtmidi/internal/qtmididefs.h"
#include "qtmidi/qmidi.h"

QT_BEGIN_NAMESPACE

class QMidiDeviceFactory;
class QAbstractMidiDevice;
class QMidiDeviceWatcher;
class QMidiDeviceManager;

class QMidiDeviceInfoPrivate;
class Q_MIDI_EXPORT QMidiDeviceInfo
{
    friend class QMidiDeviceFactory;
    friend class QAbstractMidiDevice;
    friend class QMidiDeviceWatcher;
    friend class QMidiDeviceManager;

public:
    QMidiDeviceInfo();
    QMidiDeviceInfo(const QMidiDeviceInfo& other);
    ~QMidiDeviceInfo();

    QMidiDeviceInfo& operator=(const QMidiDeviceInfo& other);

    bool operator==(const QMidiDeviceInfo& other) const;
    bool operator!=(const QMidiDeviceInfo& other) const;

#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
    // unimplemented less operator required for qt < 5.5.0
    bool operator<(const QMidiDeviceInfo& other) const;
#endif

    bool isNull() const;
    QString deviceName() const;

    static QMidiDeviceInfo defaultInputDevice();
    static QMidiDeviceInfo defaultOutputDevice();

    static QMidiDeviceInfo deviceInfoByName(QMidi::Mode mode, QString deviceName);

    static QList<QMidiDeviceInfo> availableDevices(QMidi::Mode mode);

#ifndef QT_NO_DEBUG_STREAM
    friend Q_MIDI_EXPORT QDebug operator<<(QDebug dbg, const QMidiDeviceInfo& info);
#endif
private:
    QMidiDeviceInfo(const QString &realm, const QByteArray &handle, QMidi::Mode mode);
    QString realm() const;
    QByteArray handle() const;
    QMidi::Mode mode() const;

    QSharedDataPointer<QMidiDeviceInfoPrivate> d;
};


QT_END_NAMESPACE

Q_DECLARE_METATYPE(QMidiDeviceInfo)

#endif // QMIDIDEVICEINFO_H
