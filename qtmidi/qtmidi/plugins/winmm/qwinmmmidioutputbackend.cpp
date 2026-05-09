#include "qwinmmmidioutputbackend.h"
#include <qdebug.h>

QT_BEGIN_NAMESPACE

QWinMMMidiOutputBackend::QWinMMMidiOutputBackend(QWinMMMidiPlugin &midiPlugin)
    : QWinMMMidiBackend(QMidi::MidiOutput, midiPlugin)
{
    
}

unsigned int QWinMMMidiOutputBackend::getPortCount() const {
    return midiOutGetNumDevs();
}

QString QWinMMMidiOutputBackend::getPortName(unsigned int portNumber) const {
    if (portNumber >= getPortCount()) {
        qWarning() << "The port number" << portNumber << "is invalid. Only" << getPortCount() << "devices available.";
        return QString();
    }

    MIDIOUTCAPS deviceCaps;
    midiOutGetDevCaps(portNumber, &deviceCaps, sizeof(MIDIOUTCAPS));
    return QString::fromWCharArray(deviceCaps.szPname);
}

QT_END_NAMESPACE
