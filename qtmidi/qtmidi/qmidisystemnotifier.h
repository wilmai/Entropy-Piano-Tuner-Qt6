#ifndef QMIDISYSTEMNOTIFIER_H
#define QMIDISYSTEMNOTIFIER_H

#include <qtmidi/qmidideviceinfo.h>
#include <qtmidi/qmidiinput.h>
#include <qtmidi/qmidioutput.h>

QT_BEGIN_NAMESPACE

class QMidiSystemNotifierPrivate;

class Q_MIDI_EXPORT QMidiSystemNotifier : public QObject
{
    Q_OBJECT

public:
    explicit QMidiSystemNotifier(QObject *parent);
    explicit QMidiSystemNotifier(bool notifyOnCurrentDevices = true, QObject *parent = nullptr);
    virtual ~QMidiSystemNotifier();

Q_SIGNALS:
    void deviceAttached(QMidi::Mode mode, const QMidiDeviceInfo& deviceInfo);
    void deviceDetached(QMidi::Mode mode, const QMidiDeviceInfo& deviceInfo);

    void inputDeviceAttached(const QMidiDeviceInfo& deviceInfo);
    void outputDeviceAttached(const QMidiDeviceInfo& deviceInfo);
    void inputDeviceDetached(const QMidiDeviceInfo& deviceInfo);
    void outputDeviceDetached(const QMidiDeviceInfo& deviceInfo);

    void deviceCreated(QMidi::Mode mode, const QMidiDevice* device);
    void deviceDeleted(QMidi::Mode mode, const QMidiDeviceInfo& deviceInfo);

    void inputDeviceCreated(const QMidiInput* device);
    void outputDeviceCreated(const QMidiOutput* device);
    void inputDeviceDeleted(QMidiDeviceInfo info);
    void outputDeviceDeleted(QMidiDeviceInfo info);

private:
    QMidiSystemNotifierPrivate *d;
};

QT_END_NAMESPACE

#endif // QMIDISYSTEMNOTIFIER_H
