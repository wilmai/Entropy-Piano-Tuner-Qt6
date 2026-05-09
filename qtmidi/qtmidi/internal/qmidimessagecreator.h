#ifndef QMIDIMESSAGECREATOR_H
#define QMIDIMESSAGECREATOR_H

#include "QtCore/qobject.h"
#include "qtmidi/qmidimessage.h"
#include <QDateTime>

QT_BEGIN_NAMESPACE

class QMidiMessageCreator
{
public:
    static QMidiMessage empty() {
        return QMidiMessage();
    }

    static QMidiMessage noteOn(quint8 channel, quint8 key, quint8 velocity) {
        return QMidiMessage::fromCommandChannel(QMidiMessage::NOTE_ON, channel, key, velocity, QDateTime::currentMSecsSinceEpoch());
    }

    static QMidiMessage noteOn(quint8 key, quint8 velocity) {
        return noteOn(0, key, velocity);
    }

    static QMidiMessage noteOff(quint8 channel, quint8 key, quint8 velocity) {
        return QMidiMessage::fromCommandChannel(QMidiMessage::NOTE_OFF, channel, key, velocity, QDateTime::currentMSecsSinceEpoch());
    }

    static QMidiMessage noteOff(quint8 key, quint8 velocity) {
        return noteOff(0, key, velocity);
    }

    static QMidiMessage aftertouch(quint8 channel, quint8 key, quint8 pressure) {
        return QMidiMessage::fromCommandChannel(QMidiMessage::POLYPHONIC_KEY_PRESSURE, channel, key, pressure, QDateTime::currentMSecsSinceEpoch());
    }

    static QMidiMessage aftertouch(quint8 key, quint8 pressure) {
        return aftertouch(0, key, pressure);
    }

    static QMidiMessage controlChange(quint8 channel, quint8 control, quint8 value) {
        return QMidiMessage::fromCommandChannel(QMidiMessage::CONTROL_CHANGE, channel, control, value, QDateTime::currentMSecsSinceEpoch());
    }

    static QMidiMessage controlChange(quint8 control, quint8 value) {
        return controlChange(0, control, value);
    }

    static QMidiMessage programChange(quint8 channel, quint8 program) {
        return QMidiMessage::fromCommandChannel(QMidiMessage::PROGRAM_CHANGE, channel, program, QDateTime::currentMSecsSinceEpoch());
    }

    static QMidiMessage programChange(quint8 program) {
        return QMidiMessage::fromCommandChannel(QMidiMessage::PROGRAM_CHANGE, 0, program, QDateTime::currentMSecsSinceEpoch());
    }

    static QMidiMessage pitchBend(quint8 channel, quint8 l, quint8 m) {
        return QMidiMessage::fromCommandChannel(QMidiMessage::PITCH_BEND, channel, l, m, QDateTime::currentMSecsSinceEpoch());
    }

    static QMidiMessage pitchBend(quint8 channel, int value) {
        return pitchBend(channel, value & 0x7f, (value >> 7) & 0x7f);
    }

    static QMidiMessage resetAllControllers(quint8 channel = 0) {
        return QMidiMessage::fromCommandChannel(QMidiMessage::CHANNEL_MODE_MESSAGE, channel, 121, 0, QDateTime::currentMSecsSinceEpoch());
    }

    static QMidiMessage allNotesOff(quint8 channel = 0) {
        return QMidiMessage::fromCommandChannel(QMidiMessage::CHANNEL_MODE_MESSAGE, channel, 123, 0, QDateTime::currentMSecsSinceEpoch());
    }

    static QMidiMessage disableLocalControl(quint8 channel = 0) {
        return QMidiMessage::fromCommandChannel(QMidiMessage::CHANNEL_MODE_MESSAGE, channel, 0x7A, 0x00, QDateTime::currentMSecsSinceEpoch());
    }

    static QMidiMessage enableLocalControl(quint8 channel = 0) {
        return QMidiMessage::fromCommandChannel(QMidiMessage::CHANNEL_MODE_MESSAGE, channel, 0x7A, 0x7F, QDateTime::currentMSecsSinceEpoch());
    }
};

QT_END_NAMESPACE

#endif // QMIDIMESSAGECREATOR_H
