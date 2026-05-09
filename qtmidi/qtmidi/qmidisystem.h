#ifndef QMIDISYSTEM_H
#define QMIDISYSTEM_H

#include <QtCore/qobject.h>

#include "qtmidi/internal/qtmididefs.h"
#include "qtmidi/qmidi.h"
#include "qtmidi/qmidimessage.h"


QT_BEGIN_NAMESPACE

class Q_MIDI_EXPORT QAbstractMidiDeviceInfo : public QObject
{
    Q_OBJECT

public:
    virtual QString deviceName() const = 0;
};

class QMidiDeviceInfo;
class QAbstractMidiDevicePrivate;
class Q_MIDI_EXPORT QAbstractMidiDevice : public QObject
{
    Q_OBJECT

public:
    QAbstractMidiDevice(const QMidiDeviceInfo& deviceInfo);
    virtual ~QAbstractMidiDevice();

    const QMidiDeviceInfo &deviceInfo() const;
    const QByteArray handle() const;
    virtual QMidi::Error error() const = 0;
    virtual QMidi::State state() const = 0;

    virtual void handleMidiMessage(const QMidiMessage &m) const = 0;

Q_SIGNALS:
    void requestDelete();

protected:
    QAbstractMidiDevicePrivate *d;
};

class Q_MIDI_EXPORT QAbstractMidiOutput : public QAbstractMidiDevice
{
    Q_OBJECT

public:
    QAbstractMidiOutput(const QMidiDeviceInfo& deviceInfo);

public Q_SLOTS:
    virtual void handleMidiMessage(const QMidiMessage &m) const override;

    void setForceTargetChannel(bool force, quint8 channel);

Q_SIGNALS:
    void errorChanged(QMidi::Error);

private:
    virtual void sendMidiMessage(const QMidiMessage& m) const = 0;

private:
    bool mForceTargetChannel = false;
    quint8 mForcedChannel = 0;
};

class Q_MIDI_EXPORT QAbstractMidiInput : public QAbstractMidiDevice
{
    Q_OBJECT

public:
    QAbstractMidiInput(const QMidiDeviceInfo& deviceInfo);

    void handleMidiMessage(const QMidiMessage &m) const override;
    void setIgnoreSystemMessages(bool ignore);

Q_SIGNALS:
    void errorChanged(QMidi::Error);

    void notify(const QMidiMessage& m) const;
    // more to come

private:
    bool mIgnoreSystemMessages = true;
};

QT_END_NAMESPACE

#endif // QMIDISYSTEM_H
