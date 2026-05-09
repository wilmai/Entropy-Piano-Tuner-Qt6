#include "qwinrtmididevicewatcher.h"
#include "qwinrtnativehelper.h"
#include "qwinrtmidimessagecallback.h"
#include <QDebug>

using namespace Platform::Collections;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml::Navigation;
using namespace concurrency;
using namespace Windows::UI::Popups;

QWinRTMidiDeviceWatcher::QWinRTMidiDeviceWatcher(String ^ midiSelector,
                                                 CoreDispatcher ^ dispatcher,
                                                 QWinRTMidiDeviceWatcherCallback ^ callback)
    : _midiSelectorString(midiSelector)
    , _coreDispatcher(dispatcher)
    , _callback(callback)
    , _deviceWatcher(nullptr)
    , _portList(ref new Vector<QWinRTMidiDeviceConnection ^>())
    , _enumCompleted(false)
{
    _deviceWatcher = Windows::Devices::Enumeration::DeviceInformation::CreateWatcher(midiSelector);

    _portAddedToken = _deviceWatcher->Added += ref new TypedEventHandler<DeviceWatcher ^, DeviceInformation ^>(this, &QWinRTMidiDeviceWatcher::OnPortAdded);
    _portRemovedToken = _deviceWatcher->Removed += ref new TypedEventHandler<DeviceWatcher ^, DeviceInformationUpdate ^>(this, &QWinRTMidiDeviceWatcher::OnPortRemoved);
    _portUpdatedToken = _deviceWatcher->Updated += ref new TypedEventHandler<DeviceWatcher ^, DeviceInformationUpdate ^>(this, &QWinRTMidiDeviceWatcher::OnPortUpdated);
    _portEnumCompleteToken = _deviceWatcher->EnumerationCompleted += ref new TypedEventHandler<DeviceWatcher ^, Object ^>(this, &QWinRTMidiDeviceWatcher::OnPortEnumCompleted);
}

QWinRTMidiDeviceWatcher::~QWinRTMidiDeviceWatcher()
{
    _deviceWatcher->Added -= _portAddedToken;
    _deviceWatcher->Removed -= _portRemovedToken;
    _deviceWatcher->Updated -= _portUpdatedToken;
    _deviceWatcher->EnumerationCompleted -= _portEnumCompleteToken;
}

void QWinRTMidiDeviceWatcher::Start()
{
    _deviceWatcher->Start();
}

void QWinRTMidiDeviceWatcher::Stop()
{
    _deviceWatcher->Stop();
}

QWinRTMidiDeviceConnection^ QWinRTMidiDeviceWatcher::getPort(String^ s) {
    if (!s) {return nullptr;}

    for (unsigned int i = 0; i < _portList->Size; ++i) {
        if (s->Equals(_portList->GetAt(i)->Id)) {
            return _portList->GetAt(i);
        }
    }

    return nullptr;
}

void QWinRTMidiDeviceWatcher::OnPortAdded(DeviceWatcher^ deviceWatcher, DeviceInformation^ devInfo)
{
    auto task = create_task(
        _coreDispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::High,
                                  ref new Windows::UI::Core::DispatchedHandler([this, devInfo]() {
                                      _portList->Append(ref new QWinRTMidiDeviceConnection(devInfo));
                                      _callback->deviceAttached(_midiSelectorString,
                                                                devInfo->Id,
                                                                devInfo->Name);
                                  })));
}

void QWinRTMidiDeviceWatcher::OnPortRemoved(DeviceWatcher^ deviceWatcher, DeviceInformationUpdate^ devInfoUpdate)
{
    auto task = create_task(
        _coreDispatcher
            ->RunAsync(Windows::UI::Core::CoreDispatcherPriority::High,
                       ref new Windows::UI::Core::DispatchedHandler([this, devInfoUpdate]() {
                           for (unsigned int i = 0; i < _portList->Size; ++i) {
                               if (_portList->GetAt(i)->Id->Equals(devInfoUpdate->Id)) {
                                   String ^ id = _portList->GetAt(i)->Id;
                                   String ^ name = _portList->GetAt(i)->Name;
                                   _portList->GetAt(i)->ToDelete = true;
                                   _callback->deviceDetached(_midiSelectorString, id, name);
                                   _portList->RemoveAt(i);
                                   break;
                               }
                           }
                       })));
}

void QWinRTMidiDeviceWatcher::OnPortUpdated(DeviceWatcher^ deviceWatcher, DeviceInformationUpdate^ devInfoUpdate)
{
}

void QWinRTMidiDeviceWatcher::OnPortEnumCompleted(DeviceWatcher^ deviceWatcher, Object^ obj)
{
    _enumCompleted = true;
}
