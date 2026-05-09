#ifndef QMIDIINPUT_H
#define QMIDIINPUT_H

#include <qtmidi/qmidi.h>
#include <qtmidi/qmidideviceinfo.h>
#include <qtmidi/qmididevice.h>
#include <qtmidi/qmidisystem.h>
#include <qtmidi/qmidimessage.h>

QT_BEGIN_NAMESPACE

class QAbstractMidiInput;
class QMidiDeviceManager;

class Q_MIDI_EXPORT QMidiInput : public QMidiDevice
{
    friend class QMidiDeviceManager;

    Q_OBJECT

public:
    static Q_CONSTEXPR auto Mode = QMidi::MidiInput;

public:
    explicit QMidiInput(const QMidiDeviceInfo& midiDeviceInfo, QObject *parent = nullptr);
    explicit QMidiInput(QObject *parent = nullptr);

    virtual ~QMidiInput();

Q_SIGNALS:
    void notify(const QMidiMessage &);

public Q_SLOTS:
    void setIgnoreSystemMessages(bool ignore = true);

private:
    Q_DISABLE_COPY(QMidiInput)
};

QT_END_NAMESPACE

#endif // QMIDIINPUT_H
