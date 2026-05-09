#include "qwinrtmididevicewatchercallback.h"
#include "qwinrtmidiwrapper.h"
#include "qwinrtnativehelper.h"

QT_BEGIN_NAMESPACE

using namespace Windows::Devices::Midi;

QWinRTMidiDeviceWatcherCallback::QWinRTMidiDeviceWatcherCallback(QWinRTMidiWrapper* wrapper)
    : mWrapper(wrapper) {
    
}

void QWinRTMidiDeviceWatcherCallback::deviceAttached(String^ midiSelector, String^ deviceID, String^ humanReadable) {
    QWinRTMidiWrapperCallbackLockGuard(mWrapper);
    if (!mWrapper->getCallback()) {return;}
    if (MidiInPort::GetDeviceSelector() == midiSelector) {
        mWrapper->getCallback()->inputDeviceAttached(convert(deviceID), convert(humanReadable));
    } else {
        mWrapper->getCallback()->outputDeviceAttached(convert(deviceID), convert(humanReadable));
    }
}

void QWinRTMidiDeviceWatcherCallback::deviceDetached(String^ midiSelector, String^ deviceID, String^ humanReadable) {
    QWinRTMidiWrapperCallbackLockGuard(mWrapper);
    if (!mWrapper->getCallback()) {return;}
    if (MidiInPort::GetDeviceSelector() == midiSelector) {
        mWrapper->getCallback()->inputDeviceDetached(convert(deviceID), convert(humanReadable));
    } else {
        mWrapper->getCallback()->outputDeviceDetached(convert(deviceID), convert(humanReadable));
    }
}

QT_END_NAMESPACE
