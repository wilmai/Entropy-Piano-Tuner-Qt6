#ifndef QMIDIDEVICE_H
#define QMIDIDEVICE_H

#include <QtCore/qobject.h>
#include <qtmidi/qmidideviceinfo.h>
#include <qtmidi/qmidimessage.h>

QT_BEGIN_NAMESPACE

class QAbstractMidiDevice;

class Q_MIDI_EXPORT QMidiDevice : public QObject
{
    Q_OBJECT
public:
    QMidiDevice(QAbstractMidiDevice* ad, QObject *parent);
    virtual ~QMidiDevice();

    QMidi::Error error() const;
    QMidi::State state() const;
    const QMidiDeviceInfo& deviceInfo() const;

    QString name();

public slots:
    void handleMidiMessage(const QMidiMessage &m) const;

protected:
    QAbstractMidiDevice* d;
};

QT_END_NAMESPACE

#endif // QMIDIDEVICE_H
