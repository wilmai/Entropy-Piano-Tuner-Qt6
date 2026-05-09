#include "qwinmmmidiinputbackend.h"
#include <QDebug>

QT_BEGIN_NAMESPACE

QWinMMMidiInputBackend::QWinMMMidiInputBackend(QWinMMMidiPlugin &midiPlugin)
    : QWinMMMidiBackend(QMidi::MidiInput, midiPlugin)
{
    
}

unsigned int QWinMMMidiInputBackend::getPortCount() const {
    return midiInGetNumDevs();
}

QString QWinMMMidiInputBackend::getPortName(unsigned int portNumber) const {
    if (portNumber >= getPortCount()) {
        qWarning() << "The port number" << portNumber << "is invalid. Only" << getPortCount() << "devices available.";
        return QString();
    }

    MIDIINCAPS deviceCaps;
    midiInGetDevCaps(portNumber, &deviceCaps, sizeof(MIDIINCAPS));
    return QString::fromWCharArray(deviceCaps.szPname);
}

QT_END_NAMESPACE
