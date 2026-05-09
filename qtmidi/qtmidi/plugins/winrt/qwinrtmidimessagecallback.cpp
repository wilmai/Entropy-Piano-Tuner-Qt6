#include "qwinrtmidimessagecallback.h"
#include "qwinrtnativehelper.h"
#include "qwinrtmidiinput.h"

#include <qtmidi/qmidimessage.h>
#include <QDebug>

QT_BEGIN_NAMESPACE

QWinRTMidiMessageCallback::QWinRTMidiMessageCallback(QWinRTMidiInput *input)
    : mInput(input) {
}


void QWinRTMidiMessageCallback::onMessageReceived(MidiInPort^ sender, MidiMessageReceivedEventArgs^ args) {
    IMidiMessage^ midiMessage = args->Message;
    String ^ deviceID = sender->DeviceId;

    // Duration unit is 100 ns (1 s == 10000 * 100ns)
    double timestamp = midiMessage->Timestamp.Duration / 10000.0;

    // read midi message bytes into bytes vector
    auto reader = Windows::Storage::Streams::DataReader::FromBuffer(midiMessage->RawData);
    QVector<quint8> bytes(reader->UnconsumedBufferLength);
    if (!bytes.empty()) {
        reader->ReadBytes(Platform::ArrayReference<quint8>(&bytes[0], bytes.size()));
    }

    mInput->handleMidiMessage(QMidiMessage(bytes, timestamp));
}

QT_END_NAMESPACE
