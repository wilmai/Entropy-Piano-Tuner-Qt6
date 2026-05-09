#ifndef QPGMIDIINPUT_H
#define QPGMIDIINPUT_H

#include <qtmidi/qmidisystem.h>

QT_BEGIN_NAMESPACE

class QPGMidiWrapper;

class QPGMidiInput : public QAbstractMidiInput
{
public:
    QPGMidiInput(const QMidiDeviceInfo &info, QPGMidiWrapper &wrapper);
    virtual ~QPGMidiInput();

    virtual QMidi::Error error() const {return mError;}
    virtual QMidi::State state() const {return mState;}

private:
    QMidi::Error mError = QMidi::NoError;
    QMidi::State mState = QMidi::DisconnectedState;

    QPGMidiWrapper &mWrapper;
    void *mInterface;
};

QT_END_NAMESPACE

#endif // QPGMIDIINPUT_H
