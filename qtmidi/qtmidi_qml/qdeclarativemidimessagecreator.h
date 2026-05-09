#ifndef QDECLARATIVEMIDIMESSAGECREATOR_H
#define QDECLARATIVEMIDIMESSAGECREATOR_H

#include <QObject>

#include "qtmidi/internal/qmidimessagecreator.h"

class QDeclarativeMidiMessageCreator : public QObject
{
    Q_OBJECT
public:
    explicit QDeclarativeMidiMessageCreator(QObject *parent = nullptr);

    Q_INVOKABLE QMidiMessage empty() const {
        return QMidiMessageCreator::empty();
    }

    Q_INVOKABLE QMidiMessage noteOn(quint8 channel, quint8 key, quint8 velocity) {
        return QMidiMessageCreator::noteOn(channel, key, velocity);
    }

    Q_INVOKABLE QMidiMessage noteOn(quint8 key, quint8 velocity) {
        return QMidiMessageCreator::noteOn(key, velocity);
    }

    Q_INVOKABLE QMidiMessage noteOff(quint8 channel, quint8 key, quint8 velocity) {
        return QMidiMessageCreator::noteOff(channel, key, velocity);
    }

    Q_INVOKABLE QMidiMessage noteOff(quint8 key, quint8 velocity) {
        return QMidiMessageCreator::noteOff(key, velocity);
    }

    Q_INVOKABLE QMidiMessage aftertouch(quint8 channel, quint8 key, quint8 velocity) {
        return QMidiMessageCreator::aftertouch(channel, key, velocity);
    }

    Q_INVOKABLE QMidiMessage aftertouch(quint8 key, quint8 velocity) {
        return QMidiMessageCreator::aftertouch(key, velocity);
    }

    Q_INVOKABLE QMidiMessage controlChange(quint8 channel, quint8 control, quint8 value) {
        return QMidiMessageCreator::controlChange(channel, control, value);
    }

    Q_INVOKABLE QMidiMessage controlChange(quint8 control, quint8 value) {
        return QMidiMessageCreator::controlChange(control, value);
    }

    Q_INVOKABLE QMidiMessage programChange(quint8 channel, quint8 program) {
        return QMidiMessageCreator::programChange(channel, program);
    }

    Q_INVOKABLE QMidiMessage programChange(quint8 program) {
        return QMidiMessageCreator::programChange(program);
    }

    Q_INVOKABLE QMidiMessage pitchBend(quint8 channel, quint8 l, quint8 m) {
        return QMidiMessageCreator::pitchBend(channel, l, m);
    }

    Q_INVOKABLE QMidiMessage pitchBend(quint8 channel, int value) {
        return QMidiMessageCreator::pitchBend(channel, value);
    }

    Q_INVOKABLE QMidiMessage resetAllControllers(quint8 channel = 0) {
        return QMidiMessageCreator::resetAllControllers(channel);
    }

    Q_INVOKABLE QMidiMessage allNotesOff(quint8 channel = 0) {
        return QMidiMessageCreator::allNotesOff(channel);
    }

    Q_INVOKABLE QMidiMessage disableLocalControl(quint8 channel = 0) {
        return QMidiMessageCreator::disableLocalControl(channel);
    }

    Q_INVOKABLE QMidiMessage enableLocalControl(quint8 channel = 0) {
        return QMidiMessageCreator::enableLocalControl(channel);
    }
};

#endif // QDECLARATIVEMIDIMESSAGECREATOR_H
