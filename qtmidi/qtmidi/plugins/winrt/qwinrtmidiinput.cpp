#include "qwinrtmidiinput.h"
#include "qwinrtmidimessagecallback.h"

QT_BEGIN_NAMESPACE

QWinRTMidiInput::QWinRTMidiInput(const QMidiDeviceInfo &info, QWinRTMidiWrapper &wrapper)
    : QAbstractMidiInput(info)
    , mWrapper(wrapper)
{
    mMidiMessageCallback = ref new QWinRTMidiMessageCallback(this);

    if (mWrapper.createInputDevice(handle(), mMidiMessageCallback)) {
        mState = QMidi::ConnectedState;
    } else {
        mState = QMidi::DisconnectedState;
    }
}

QWinRTMidiInput::~QWinRTMidiInput()
{
    mState = QMidi::DisconnectedState;
    mMidiMessageCallback = nullptr;
    mWrapper.deleteInputDevice(handle());
}

QT_END_NAMESPACE
