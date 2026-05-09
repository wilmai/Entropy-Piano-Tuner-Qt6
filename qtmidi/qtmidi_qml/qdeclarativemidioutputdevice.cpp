#include "qdeclarativemidioutputdevice_p.h"
#include "qtmidi/qmidioutput.h"
#include "qtmidi/qmidiautoconnector.h"

QT_BEGIN_NAMESPACE

QDeclarativeMidiOutputDevice::QDeclarativeMidiOutputDevice(QObject *parent)
    : QDeclarativeMidiDevice(MidiOutput, parent) {

}

void QDeclarativeMidiOutputDevice::sendMessage(const QMidiMessage &message) {
    const QMidiOutput *device = m_autoConnector->device<QMidiOutput>();
    if (device) {
        device->handleMidiMessage(message);
    }
}
QT_END_NAMESPACE
