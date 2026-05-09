#include "qwinmmmidioutput.h"
#include <qdebug.h>

#include "qwinmmmidioutputbackend.h"

QT_BEGIN_NAMESPACE

QWinMMMidiOutput::QWinMMMidiOutput(const QMidiDeviceInfo &info, const QWinMMMidiOutputBackend &backend)
    : QAbstractMidiOutput(info)
    , mBackend(backend)
{
    mError = QMidi::OpenError;
    mState = QMidi::DisconnectedState;

    int portId = mBackend.getPortId(handle());
    MMRESULT result = midiOutOpen(&mMMHandle,
                                  portId,
                                  (DWORD)NULL,
                                  (DWORD)NULL,
                                  CALLBACK_NULL);

    if (result != MMSYSERR_NOERROR) {
        qCritical() << "Error creating Windows MM MIDI output port.";
        return;
    }

    mError = QMidi::NoError;
    mState = QMidi::ConnectedState;
}

QWinMMMidiOutput::~QWinMMMidiOutput() {
    if (mState == QMidi::DisconnectedState) {return;}

    midiOutReset(mMMHandle);
    midiOutClose(mMMHandle);

    mState = QMidi::DisconnectedState;
}

void QWinMMMidiOutput::sendMidiMessage(const QMidiMessage& m) const {
    if (mState == QMidi::DisconnectedState) {return;}

    unsigned int nBytes = static_cast<unsigned int>(m.size());
    if ( nBytes == 0 ) {
        qDebug() << "MidiOutWinMM::sendMessage: message argument is empty!";
        mError = QMidi::IOError;
        return;
    }

    MMRESULT result;
    if ( m.command() == 0xF0 ) { // Sysex message
        // Create and prepare MIDIHDR structure.
        MIDIHDR sysex;
        sysex.lpData = (LPSTR) m.message().data();
        sysex.dwBufferLength = nBytes;
        sysex.dwFlags = 0;
        result = midiOutPrepareHeader( mMMHandle,  &sysex, sizeof(MIDIHDR) );
        if ( result != MMSYSERR_NOERROR ) {
            qDebug() << "Error preparing sysex header";
            return;
        }

        // Send the message.
        result = midiOutLongMsg( mMMHandle, &sysex, sizeof(MIDIHDR) );
        if ( result != MMSYSERR_NOERROR ) {
            qDebug() << "Error sending sysex message";
            return;
        }

        // Unprepare the buffer and MIDIHDR.
        while ( MIDIERR_STILLPLAYING == midiOutUnprepareHeader( mMMHandle, &sysex, sizeof (MIDIHDR) ) ) Sleep( 1 );
    }
    else { // Channel or system message.

        // Make sure the message size isn't too big.
        if ( nBytes > 3 ) {
            qDebug() << "Message size is greater than 3 bytes (and not sysex)";
            return;
        }

        // Send the message immediately.
        result = midiOutShortMsg( mMMHandle, m.toUInt32() );
        if ( result != MMSYSERR_NOERROR ) {
            qDebug() << "Error sending MIDI message";
            return;
        }
    }
}

QT_END_NAMESPACE
