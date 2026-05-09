#include "qdeclarativemidiinputdevice_p.h"
#include "qtmidi/qmidiautoconnector.h"

QT_BEGIN_NAMESPACE

QDeclarativeMidiInputDevice::QDeclarativeMidiInputDevice(QObject *parent)
    : QDeclarativeMidiDevice(MidiInput, parent) {
    QObject::connect(this, &QDeclarativeMidiDevice::currentIndexChanged, this, &QDeclarativeMidiInputDevice::currentInputDeviceChanged);
}

void QDeclarativeMidiInputDevice::currentInputDeviceChanged() {
    QObject::disconnect(m_connection);
    QMidiInput *input = m_autoConnector->device<QMidiInput>(currentDeviceInfo());
    if (input) {
        m_connection = QObject::connect(input, SIGNAL(notify(QMidiMessage)), this, SIGNAL(midiMessageReceived(QMidiMessage)));
    }
}

QT_END_NAMESPACE
