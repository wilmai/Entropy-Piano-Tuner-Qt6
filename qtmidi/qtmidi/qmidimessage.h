#ifndef QMIDIMESSAGE_H
#define QMIDIMESSAGE_H

#include <QtCore/QVector>

#include <qtmidi/qmidi.h>

QT_BEGIN_NAMESPACE

class Q_MIDI_EXPORT QMidiMessage
{
    Q_GADGET
    Q_PROPERTY(quint8 command READ command WRITE setCommand)
    Q_PROPERTY(quint8 channel READ channel WRITE setChannel)
    Q_PROPERTY(quint8 byte0 READ byte0 WRITE setByte0)
    Q_PROPERTY(quint8 byte1 READ byte1)
    Q_PROPERTY(quint8 byte2 READ byte2)
    Q_PROPERTY(QVector<quint8> message READ message)
    Q_PROPERTY(quint64 timestamp READ timestamp WRITE setTimestamp)

public:
    enum Commands : quint8 {
        NOTE_OFF = 0b1000,
        NOTE_ON = 0b1001,
        POLYPHONIC_KEY_PRESSURE = 0b1010,
        CONTROL_CHANGE = 0b1011,
        PROGRAM_CHANGE = 0b1100,
        CHANNEL_PRESSURE = 0b1101,
        PITCH_BEND = 0b1110,
        CHANNEL_MODE_MESSAGE = 0b1011,
    };

    // custom aliases for frequent commands
    // channel voice messages
    Q_INVOKABLE bool isNoteOff() const { return command() == NOTE_OFF; }
    Q_INVOKABLE bool isNoteOn() const { return command() == NOTE_ON; }
    Q_INVOKABLE bool isPolyphonicKeyPressure() const { return command() == POLYPHONIC_KEY_PRESSURE; }
    Q_INVOKABLE bool isControlChange() const { return command() == CONTROL_CHANGE; }
    Q_INVOKABLE bool isProgramChange() const { return command() == PROGRAM_CHANGE; }
    Q_INVOKABLE bool isChannelPressure() const { return command() == CHANNEL_PRESSURE; }
    Q_INVOKABLE bool isPitchBend() const { return command() == PITCH_BEND; }
    Q_INVOKABLE bool isChannelModeMessage() const { return command() == CHANNEL_MODE_MESSAGE; }

    Q_INVOKABLE quint8 key() const {return byte1();}
    Q_INVOKABLE quint8 controller() const {return byte1();}
    Q_INVOKABLE quint8 programNumber() const {return byte1();}

    Q_INVOKABLE quint8 pressure() const {return isChannelPressure() ? byte1() : byte2();}

    Q_INVOKABLE quint8 volume() const {return byte2();}
    Q_INVOKABLE quint8 velocity() const {return byte2();}
    Q_INVOKABLE quint8 controllerValue() const {return byte2();}

public:
    QMidiMessage();
    explicit QMidiMessage(const QVector<quint8> &m, quint64 timestamp);
    explicit QMidiMessage(quint8 byte0, quint8 byte1, quint64 timestamp);
    explicit QMidiMessage(quint8 byte0, quint8 byte1, quint8 byte2, quint64 timestamp);
    explicit QMidiMessage(quint8 byte0, quint8 byte1, quint8 byte2, quint8 byte3, quint64 timestamp);

    static QMidiMessage fromCommand(int command, int byte1, int byte2, quint64 timestamp);
    static QMidiMessage fromCommandChannel(quint8 command, quint8 channel, quint8 byte1, quint64 timestamp);
    static QMidiMessage fromCommandChannel(quint8 command, quint8 channel, quint8 byte1, quint8 byte2, quint64 timestamp);

    quint8 command() const;
    void setCommand(quint8 c);
    quint8 channel() const;
    void setChannel(quint8 c);
    quint8 byte0() const;
    void setByte0(quint8 b0);
    quint8 byte1() const;
    quint8 byte2() const;

    quint32 toUInt32() const;

    Q_INVOKABLE void appendByte(quint8 byte);

    void setTimestamp(const quint64 d);
    quint64 timestamp() const;

    const QVector<quint8> message() const;
    int size() const;

    const quint8 &operator[](int i) const;
    quint8 &operator[](int i);

    bool equalBytes(const QMidiMessage &other) const;

    bool operator==(const QMidiMessage &other) const;
    bool operator!=(const QMidiMessage &other) const;

    Q_INVOKABLE QString toJson() const;
#ifndef QT_NO_DEBUG_STREAM
    friend Q_MIDI_EXPORT QDebug operator<<(QDebug dbg, const QMidiMessage& m);
#endif
private:
    QVector<quint8> mMessage;
    quint64 mTimestamp = 0.0;
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QMidiMessage)

#endif // QMIDIMESSAGE_H
