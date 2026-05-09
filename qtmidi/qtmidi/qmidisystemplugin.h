#ifndef QMIDISYSTEMPLUGIN_H
#define QMIDISYSTEMPLUGIN_H

#include <QtCore/qstring.h>
#include <QtCore/qplugin.h>

#include "qtmidi/internal/qtmididefs.h"
#include "qtmidi/qmidi.h"
#include "qtmidi/qmidisystem.h"

QT_BEGIN_NAMESPACE

class Q_MIDI_EXPORT QMidiSystemFactoryInterface
{
public:
    virtual QList<QByteArray> availableDevices(QMidi::Mode) const = 0;
    virtual QAbstractMidiInput* createInput(const QMidiDeviceInfo& device) = 0;
    virtual QAbstractMidiOutput* createOutput(const QMidiDeviceInfo& device) = 0;
    virtual QString deviceName(const QByteArray& device, QMidi::Mode mode) = 0;
    virtual ~QMidiSystemFactoryInterface();
};

class Q_MIDI_EXPORT QMidiSystemRegistry
{
private:
    static QMidiSystemFactoryInterface* test();
};

class Q_MIDI_EXPORT QMidiSystemPlugin : public QObject, public QMidiSystemFactoryInterface
{
    Q_OBJECT

public:
    explicit QMidiSystemPlugin(bool supportsDeviceWatcher, QObject* parent = Q_NULLPTR);
    ~QMidiSystemPlugin();

    virtual QList<QByteArray> availableDevices(QMidi::Mode) const = 0;
    virtual QAbstractMidiInput* createInput(const QMidiDeviceInfo& device) = 0;
    virtual QAbstractMidiOutput* createOutput(const QMidiDeviceInfo& device) = 0;
    virtual QString deviceName(const QByteArray& device, QMidi::Mode mode) = 0;

    QMidiDeviceInfo deviceInfo(const QString &realm, const QByteArray &handle, QMidi::Mode mode);

    void notifyInputDeviceAttached(const QMidiDeviceInfo& deviceInfo);
    void notifyOutputDeviceAttached(const QMidiDeviceInfo& deviceInfo);
    void notifyInputDeviceDetached(const QMidiDeviceInfo& deviceInfo);
    void notifyOutputDeviceDetached(const QMidiDeviceInfo& deviceInfo);

    void notifyDeviceAttached(const QMidiDeviceInfo& deviceInfo, QMidi::Mode mode);
    void notifyDeviceDetached(const QMidiDeviceInfo& deviceInfo, QMidi::Mode mode);

private:
    bool mSupportsDeviceWatcher;
};

QT_END_NAMESPACE

#endif // QMIDISYSTEMPLUGIN_H
