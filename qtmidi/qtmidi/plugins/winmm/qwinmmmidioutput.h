#ifndef QWINMMMIDIOUTPUT_H
#define QWINMMMIDIOUTPUT_H

#include <qtmidi/qmidisystem.h>

#include <windows.h>
#include <mmsystem.h>

QT_BEGIN_NAMESPACE

class QWinMMMidiOutputBackend;

class QWinMMMidiOutput : public QAbstractMidiOutput
{
public:
    QWinMMMidiOutput(const QMidiDeviceInfo &info, const QWinMMMidiOutputBackend &backend);
    ~QWinMMMidiOutput();

    virtual QMidi::Error error() const {return mError;}
    virtual QMidi::State state() const {return mState;}
    virtual void sendMidiMessage(const QMidiMessage& m) const override;

private:
    mutable QMidi::Error mError = QMidi::NoError;
    QMidi::State mState = QMidi::DisconnectedState;
    const QWinMMMidiOutputBackend &mBackend;

    HMIDIOUT mMMHandle;
};

QT_END_NAMESPACE

#endif // QWINMMMIDIOUTPUT_H
