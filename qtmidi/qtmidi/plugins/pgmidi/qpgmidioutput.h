#ifndef QPGMIDIOUTPUT_H
#define QPGMIDIOUTPUT_H

#include <qtmidi/qmidisystem.h>

QT_BEGIN_NAMESPACE

class QPGMidiWrapper;

class QPGMidiOutput : public QAbstractMidiOutput
{
public:
    QPGMidiOutput(const QMidiDeviceInfo &info, QPGMidiWrapper &wrapper);
    ~QPGMidiOutput();

    virtual QMidi::Error error() const override {return mError;}
    virtual QMidi::State state() const override {return mState;}

private:
    virtual void sendMidiMessage(const QMidiMessage& m) const override;

private:
    QMidi::Error mError = QMidi::NoError;
    QMidi::State mState = QMidi::DisconnectedState;

    QPGMidiWrapper &mWrapper;
    void *mOutputDevice;
};

QT_END_NAMESPACE

#endif // QPGMIDIOUTPUT_H
