#include "qpgmidioutput.h"

#include <QDateTime>

#import "qpgmidimonitor.h"
#include "qpgmidiwrapper.h"


QT_BEGIN_NAMESPACE

QPGMidiOutput::QPGMidiOutput(const QMidiDeviceInfo &info, QPGMidiWrapper &wrapper)
    : QAbstractMidiOutput(info)
    , mWrapper(wrapper)
    , mOutputDevice(nil)
{
    auto monitor = (QPGMidiMonitor*)mWrapper.monitor();
    mOutputDevice = [monitor destination:handle()];
    if (mOutputDevice != nil) {
        mState = QMidi::ConnectedState;
    }
}

QPGMidiOutput::~QPGMidiOutput() {
    mOutputDevice = nil;
}

void QPGMidiOutput::sendMidiMessage(const QMidiMessage& m) const {
    if (mOutputDevice == nil) {return;}
    auto dest = (PGMidiDestination*)mOutputDevice;
    // convert timestamp in ns relative to the uptime in s to ms since epoch
    NSTimeInterval uptime = [[NSProcessInfo processInfo] systemUptime];
    const quint64 bootTime = QDateTime::currentMSecsSinceEpoch() - static_cast<quint64>(uptime * 1000);
    quint64 timestamp = 0;
    if (m.timestamp() > bootTime) {
        timestamp = (m.timestamp() - bootTime) * 1000000;
    }
    [dest sendBytes:m.message().data() size:m.size() timestamp:timestamp];
}

QT_END_NAMESPACE
