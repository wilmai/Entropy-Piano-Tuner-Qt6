#ifndef QMIDI_H
#define QMIDI_H

#include <QtCore/qmetatype.h>

#include "qtmidi/internal/qtmididefs.h"

QT_BEGIN_NAMESPACE

namespace QMidi
{
enum Error { NoError, OpenError, IOError, FatalError };
enum Mode { MidiInput = 0, MidiOutput = 1 };
enum State { ConnectedState, DisconnectedState };
}

#ifndef QT_NO_DEBUG_STREAM
Q_MIDI_EXPORT QDebug operator<<(QDebug dbg, QMidi::Error error);
Q_MIDI_EXPORT QDebug operator<<(QDebug dbg, QMidi::Mode mode);
#endif

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QMidi::Error)
Q_DECLARE_METATYPE(QMidi::Mode)
Q_DECLARE_METATYPE(QMidi::State)


#endif // QMDI_H
