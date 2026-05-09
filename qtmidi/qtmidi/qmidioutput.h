#ifndef QMIDIOUTPUT_H
#define QMIDIOUTPUT_H

#include <qtmidi/qmidi.h>
#include <qtmidi/qmidideviceinfo.h>
#include <qtmidi/qmididevice.h>
#include <qtmidi/qmidisystem.h>

QT_BEGIN_NAMESPACE

class QAbstractMidiOutput;
class QMidiDeviceManager;

class Q_MIDI_EXPORT QMidiOutput : public QMidiDevice
{
    friend class QMidiDeviceManager;

    Q_OBJECT

public:
    static Q_CONSTEXPR auto Mode = QMidi::MidiOutput;

public:
    explicit QMidiOutput(const QMidiDeviceInfo& midiDeviceInfo, QObject *parent = nullptr);
    explicit QMidiOutput(QObject *parent = nullptr);
    virtual ~QMidiOutput();

public Q_SLOTS:
    void receiveMidiMessage(const QMidiMessage& m);
    void setForceTargetChannel(bool force, quint8 channel);

private:
    Q_DISABLE_COPY(QMidiOutput)
};

QT_END_NAMESPACE

#endif // QMIDIOUTPUT_H
