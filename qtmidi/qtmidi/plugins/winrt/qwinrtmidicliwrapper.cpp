#include "qwinrtmidicliwrapper.h"
#include "qwinrtmididevicewatchercallback.h"
#include "qwinrtnativehelper.h"
#include "qwinrtmidimessagecallback.h"
#include "qwinrtmididevicewatcher.h"

QT_BEGIN_NAMESPACE

using namespace Windows::UI::Core;
using namespace Windows::Devices::Midi;

QWinRTMidiCLIWrapper::QWinRTMidiCLIWrapper(QWinRTMidiWrapper *wrapper) {
    auto currentView = Windows::ApplicationModel::Core::CoreApplication::MainView;
    auto coreWindow = currentView->CoreWindow;
    auto dispatcher = coreWindow->Dispatcher;
    mDispatcher = dispatcher;
    mMidiInDeviceWatcher = ref new QWinRTMidiDeviceWatcher(MidiInPort::GetDeviceSelector(), dispatcher, ref new QWinRTMidiDeviceWatcherCallback(wrapper));
    mMidiOutDeviceWatcher = ref new QWinRTMidiDeviceWatcher(MidiOutPort::GetDeviceSelector(), dispatcher, ref new QWinRTMidiDeviceWatcherCallback(wrapper));
}

QWinRTMidiDeviceWatcher ^ QWinRTMidiCLIWrapper::GetMidiInDeviceWatcher() {
    return mMidiInDeviceWatcher;
}

QWinRTMidiDeviceWatcher ^ QWinRTMidiCLIWrapper::GetMidiOutDeviceWatcher() {
    return mMidiOutDeviceWatcher;
}

Windows::UI::Core::CoreDispatcher ^ QWinRTMidiCLIWrapper::GetDispatcher() {
    return mDispatcher;
}

QT_END_NAMESPACE
