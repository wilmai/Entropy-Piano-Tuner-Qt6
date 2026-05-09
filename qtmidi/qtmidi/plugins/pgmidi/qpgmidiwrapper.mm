#include "qpgmidiwrapper.h"
#import "qpgmidimonitor.h"
#include "qpgmidiplugin.h"

#include <qdebug.h>

QT_BEGIN_NAMESPACE

QPGMidiWrapper::QPGMidiWrapper(QPGMidiPlugin &plugin)
    : mPlugin(plugin)
    , mMonitor(nil)
{
    qDebug() << "Initializing PGMidi";
    auto monitor = [[QPGMidiMonitor alloc] initMonitor:this];
    mMonitor = monitor;
}

QPGMidiWrapper::~QPGMidiWrapper()
{
    if (mMonitor != nil) {
        [(QPGMidiMonitor*)mMonitor release];
        mMonitor = nil;
    }
}

void QPGMidiWrapper::start() {
    auto monitor = (QPGMidiMonitor*)mMonitor;
    [monitor start];
}

QList<QByteArray> QPGMidiWrapper::availableDevices(QMidi::Mode mode) const
{
    auto m = (QPGMidiMonitor*)mMonitor;
    if (mode == QMidi::MidiInput) {
        return [m availableInputDevices];
    } else {
        return [m availableOutputDevices];
    }
}

QT_END_NAMESPACE
