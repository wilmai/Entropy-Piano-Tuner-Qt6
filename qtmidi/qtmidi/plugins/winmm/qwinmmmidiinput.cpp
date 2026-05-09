#include "qwinmmmidiinput.h"
#include "qwinmmmidiinputbackend.h"
#include <qdebug.h>

QT_BEGIN_NAMESPACE

#define  QT_WINMM_SYSEX_BUFFER_SIZE 1024
#define  QT_WINMM_SYSEX_BUFFER_COUNT 4

static void CALLBACK midiInputCallback(
    HMIDIIN hmin, UINT inputStatus, DWORD_PTR instancePtr, DWORD_PTR midiMessage, DWORD timestamp);

QWinMMMidiInput::QWinMMMidiInput(const QMidiDeviceInfo &info, const QWinMMMidiBackend &backend)
    : QAbstractMidiInput(info)
    , mBackend(backend)
{
    mError = QMidi::OpenError;

    if (!InitializeCriticalSectionAndSpinCount(&mMutex, 0x00000400)) {
        qCritical() << "InitializeCriticalSectionAnSpinCount failed.";
        return;
    }

    int portId = mBackend.getPortId(handle());
    if (portId < 0) {
        qCritical() << "Invalid port number";
        return;
    }

    MMRESULT result = midiInOpen(&mMMHandle,
                                 portId,
                                 (DWORD_PTR)&midiInputCallback,
                                 (DWORD_PTR)this,
                                 CALLBACK_FUNCTION);

    if (result != MMSYSERR_NOERROR) {
        qCritical() << "midiInOpen error with code:" << result;
        return;
    }

    // Allocate and init the sysex buffers.
    mSysexBuffer.resize(QT_WINMM_SYSEX_BUFFER_COUNT);
    for (int i = 0; i < mSysexBuffer.size(); ++i) {
        mSysexBuffer[i] = (MIDIHDR*) new char[ sizeof(MIDIHDR) ];
        mSysexBuffer[i]->lpData = new char[ QT_WINMM_SYSEX_BUFFER_SIZE ];
        mSysexBuffer[i]->dwBufferLength = QT_WINMM_SYSEX_BUFFER_SIZE;
        mSysexBuffer[i]->dwUser = i; // We use the dwUser parameter as buffer indicator
        mSysexBuffer[i]->dwFlags = 0;

        result = midiInPrepareHeader( mMMHandle, mSysexBuffer[i], sizeof(MIDIHDR) );
        if ( result != MMSYSERR_NOERROR ) {
            midiInClose( mMMHandle );
            qCritical() << "Error starting Windows MM MIDI input port (PrepareHeader).";
            return;
        }

        // Register the buffer.
        result = midiInAddBuffer( mMMHandle, mSysexBuffer[i], sizeof(MIDIHDR) );
        if ( result != MMSYSERR_NOERROR ) {
            midiInClose( mMMHandle );
            qCritical() << "Error starting Windows MM MIDI input port (AddBuffer).";
            return;
        }
    }

    result = midiInStart( mMMHandle );
    if ( result != MMSYSERR_NOERROR ) {
        midiInClose( mMMHandle );
        qCritical() << "Error starting Windows MM MIDI input port.";
        return;
    }

    mError = QMidi::NoError;
    mState = QMidi::ConnectedState;
}

QWinMMMidiInput::~QWinMMMidiInput() {
    if (mState == QMidi::DisconnectedState) {return;}
    mState = QMidi::DisconnectedState;

    EnterCriticalSection( &mMutex );
    midiInReset( mMMHandle );
    midiInStop( mMMHandle );

    for ( int i = 0; i < mSysexBuffer.size(); ++i ) {
        int result = midiInUnprepareHeader(mMMHandle, mSysexBuffer[i], sizeof(MIDIHDR));
        delete [] mSysexBuffer[i]->lpData;
        delete [] mSysexBuffer[i];
        if ( result != MMSYSERR_NOERROR ) {
            midiInClose( mMMHandle );
            qWarning() << "Error closing Windows MM MIDI input port (midiInUnprepareHeader).";
            mError = QMidi::FatalError;
            return;
        }
    }

    midiInClose( mMMHandle );
    LeaveCriticalSection( &mMutex );
}

static void CALLBACK midiInputCallback(
    HMIDIIN hmin, UINT inputStatus, DWORD_PTR instancePtr, DWORD_PTR midiMessage, DWORD timestamp)
{
    if ( inputStatus != MIM_DATA && inputStatus != MIM_LONGDATA && inputStatus != MIM_LONGERROR ) return;

    QWinMMMidiInput *thiz = (QWinMMMidiInput*)instancePtr;
    QMidiMessage message;

    if ( thiz->mmHandle() != hmin) {
        qWarning() << "Handle-Instance mismatch";
        return;
    }

    // Calculate time stamp.
    if ( thiz->firstMessage() ) {
        message.setTimestamp(0.0);
        thiz->setFirstMessage(false);
    } else {
        message.setTimestamp((double) ( timestamp - thiz->lastTime() ) * 0.001);
    }

    thiz->setLastTime(timestamp);

    if ( inputStatus == MIM_DATA ) { // Channel or system message

        // Make sure the first byte is a status byte.
        unsigned char status = (unsigned char) (midiMessage & 0x000000FF);
        if ( !(status & 0x80) ) return;

        // Determine the number of bytes in the MIDI message.
        unsigned short nBytes = 1;
        if ( status < 0xC0 ) nBytes = 3;
        else if ( status < 0xE0 ) nBytes = 2;
        else if ( status < 0xF0 ) nBytes = 3;
        else if ( status == 0xF1 ) {
            if ( thiz->ignoreFlags() & 0x02 ) return;
            else nBytes = 2;
        }
        else if ( status == 0xF2 ) nBytes = 3;
        else if ( status == 0xF3 ) nBytes = 2;
        else if ( status == 0xF8 && (thiz->ignoreFlags() & 0x02) ) {
            // A MIDI timing tick message and we're ignoring it.
            return;
        }
        else if ( status == 0xFE && (thiz->ignoreFlags() & 0x04) ) {
            // A MIDI active sensing message and we're ignoring it.
            return;
        }

        // Copy bytes to our MIDI message.
        unsigned char *ptr = (unsigned char *) &midiMessage;
        for ( int i=0; i<nBytes; ++i ) message.appendByte( *ptr++ );
    }
    else { // Sysex message ( MIM_LONGDATA or MIM_LONGERROR )
        MIDIHDR *sysex = ( MIDIHDR *) midiMessage;
        if ( !( thiz->ignoreFlags() & 0x01 ) && inputStatus != MIM_LONGERROR ) {
            // Sysex message and we're not ignoring it
            for ( int i=0; i<(int)sysex->dwBytesRecorded; ++i ) {
                message.appendByte( sysex->lpData[i] );
            }
        }

        // The WinMM API requires that the sysex buffer be requeued after
        // input of each sysex message.  Even if we are ignoring sysex
        // messages, we still need to requeue the buffer in case the user
        // decides to not ignore sysex messages in the future.  However,
        // it seems that WinMM calls this function with an empty sysex
        // buffer when an application closes and in this case, we should
        // avoid requeueing it, else the computer suddenly reboots after
        // one or two minutes.
        if ( thiz->sysexBuffer()[sysex->dwUser]->dwBytesRecorded > 0 ) {
            //if ( sysex->dwBytesRecorded > 0 ) {
            EnterCriticalSection( &(thiz->mutex()) );
            MMRESULT result = midiInAddBuffer( thiz->mmHandle(), thiz->sysexBuffer()[sysex->dwUser], sizeof(MIDIHDR) );
            LeaveCriticalSection( &(thiz->mutex()) );
            if ( result != MMSYSERR_NOERROR ) {
                qWarning() << "RtMidiIn::midiInputCallback: error sending sysex to Midi device!";
            }

            if ( thiz->ignoreFlags() & 0x01 ) return;
        }
        else return;
    }

    thiz->handleMidiMessage(message);
}

QT_END_NAMESPACE
