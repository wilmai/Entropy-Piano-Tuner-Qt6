#ifndef QWINMMMIDIINPUT_H
#define QWINMMMIDIINPUT_H

#include <qtmidi/qmidisystem.h>

#include <QtCore/qvector.h>

#include <windows.h>
#include <mmsystem.h>

QT_BEGIN_NAMESPACE

class QWinMMMidiBackend;

class QWinMMMidiInput : public QAbstractMidiInput
{
public:
    QWinMMMidiInput(const QMidiDeviceInfo &info, const QWinMMMidiBackend &backend);
    virtual ~QWinMMMidiInput();

    virtual QMidi::Error error() const {return mError;}
    virtual QMidi::State state() const {return mState;}

    HMIDIIN mmHandle() const {return mMMHandle;}
    DWORD lastTime() const {return mLastTime;}
    void setLastTime(DWORD lastTime) {mLastTime = lastTime;}
    bool firstMessage() const {return mFirstMessage;}
    void setFirstMessage(bool firstMessage) {mFirstMessage = firstMessage;}
    unsigned char ignoreFlags() const {return mIgnoreFlags;}
    CRITICAL_SECTION &mutex() const {return mMutex;}
    const QVector<LPMIDIHDR> &sysexBuffer() const {return mSysexBuffer;}

private:
    QMidi::Error mError = QMidi::NoError;
    QMidi::State mState = QMidi::DisconnectedState;

    const QWinMMMidiBackend &mBackend;

    unsigned char mIgnoreFlags = 7;
    HMIDIIN mMMHandle;
    DWORD mLastTime = 0;
    bool mFirstMessage = true;
    QVector<LPMIDIHDR> mSysexBuffer;
    mutable CRITICAL_SECTION mMutex;
};

QT_END_NAMESPACE

#endif // QWINMMMIDIINPUT_H
