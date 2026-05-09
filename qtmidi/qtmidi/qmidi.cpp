#include "qmidi.h"
#include <QDebug>

QT_BEGIN_NAMESPACE

static void qRegisterAutoMetaTypes()
{
    qRegisterMetaType<QMidi::Error>();
    qRegisterMetaType<QMidi::Mode>();
    qRegisterMetaType<QMidi::State>();
}

Q_CONSTRUCTOR_FUNCTION(qRegisterAutoMetaTypes)

/*!
    \namespace QMidi
    \ingroup midi-namespaces
    \brief The QMidi namespace contains enums used by the midi classes.
    \inmodule QtMidi
    \ingroup midi
    \ingroup midi_audio
*/

/*!
    \enum QMidi::Error

    \value NoError              No errors have occurred
    \value OpenError            An error occurred opening the midi device
    \value IOError              An error occured during sending/receiving midi events
    \value FatalError           A non-recoverable error has occurred, the audio device is not usable at this time.
*/

/*!
    \enum QMidi::Mode

    \value MidiOutput           midi output device
    \value MidiInput            midi input device
*/

/*!
    \enum QMidi::State

    \value ConnectedState       midi device is connected and can be used to receive or send messages
    \value DisconnectedState    midi device is disconnected, is will not send or receive messages

    \sa QMidiDevice::state()
*/

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, QMidi::Error error)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace();
    switch (error) {
    case QMidi::NoError:
        dbg << "NoError";
        break;
    case QMidi::OpenError:
        dbg << "OpenError";
        break;
    case QMidi::IOError:
        dbg << "IOError";
        break;
    case QMidi::FatalError:
        dbg << "FatalError";
        break;
    }
    return dbg;
}

QDebug operator<<(QDebug dbg, QMidi::Mode mode)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace();
    switch (mode) {
    case QMidi::MidiOutput:
        dbg << "MidiOutput";
        break;
    case QMidi::MidiInput:
        dbg << "MidiInput";
        break;
    }
    return dbg;
}

QDebug operator<<(QDebug dbg, QMidi::State state)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace();
    switch (state) {
    case QMidi::ConnectedState:
        dbg << "ConnectedState";
        break;
    case QMidi::DisconnectedState:
        dbg << "DisconnectedState";
        break;
    }
    return dbg;
}

#endif

QT_END_NAMESPACE
