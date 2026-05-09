#include "qwinrtmidioutput.h"

QT_BEGIN_NAMESPACE

QWinRTMidiOutput::QWinRTMidiOutput(const QMidiDeviceInfo &info, QWinRTMidiWrapper &wrapper)
    : QAbstractMidiOutput(info)
    , mWrapper(wrapper) {
    if (wrapper.createOutputDevice(handle())) {
        mState = QMidi::ConnectedState;
    } else {
        mState = QMidi::DisconnectedState;
    }
}

QWinRTMidiOutput::~QWinRTMidiOutput()
{
    mState = QMidi::DisconnectedState;
    mWrapper.deleteOutputDevice(handle());
}

void QWinRTMidiOutput::sendMidiMessage(const QMidiMessage& m) const {
    mWrapper.sendMidiMessage(handle(), m);
}

QT_END_NAMESPACE
