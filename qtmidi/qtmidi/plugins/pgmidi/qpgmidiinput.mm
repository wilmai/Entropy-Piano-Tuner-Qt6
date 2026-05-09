#include "qpgmidiinput.h"
#include "qpgmidiwrapper.h"
#import "qpgmidimonitor.h"

QT_BEGIN_NAMESPACE

QPGMidiInput::QPGMidiInput(const QMidiDeviceInfo &info, QPGMidiWrapper &wrapper)
    : QAbstractMidiInput(info)
    , mWrapper(wrapper)
    , mInterface(nil)
{
    auto interface = [[QPGMidiInputInterface alloc] initInput:this];
    mInterface = interface;
    auto monitor = (QPGMidiMonitor*)mWrapper.monitor();
    [monitor connectSource:handle() input:interface];

    mState = QMidi::ConnectedState;
}

QPGMidiInput::~QPGMidiInput() {
    auto interface = (QPGMidiInputInterface*)mInterface;
    auto monitor = (QPGMidiMonitor*)mWrapper.monitor();
    [monitor disconnectSource:handle() input:interface];
    [interface release];
}

QT_END_NAMESPACE
