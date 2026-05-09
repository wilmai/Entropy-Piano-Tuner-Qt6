#ifndef MIDIDEVICEWATCHER_H
#define MIDIDEVICEWATCHER_H

#include <Windows.h>
#include <Windows.ApplicationModel.core.h>
#include <collection.h>
#include <ppltasks.h>
#include <wrl.h>
#include "qwinrtmididevicewatchercallback.h"

using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Platform;
using namespace Windows::Devices::Enumeration;

using namespace Platform::Collections;
using namespace Microsoft::WRL;
using namespace concurrency;
using namespace Windows::Devices::Midi;

ref class QWinRTMidiDeviceConnection sealed {
public:
    QWinRTMidiDeviceConnection(DeviceInformation^ device)
    {
        Id = device->Id;
        Name = device->Name;
        Device = device;
        ToDelete = false;
        MidiIn = nullptr;
        MidiOut = nullptr;
    }

    property bool ToDelete;
    property String^ Id;
    property String^ Name;
    property DeviceInformation^ Device;
    property MidiInPort ^ MidiIn;
    property IMidiOutPort ^ MidiOut;
    property Windows::Foundation::EventRegistrationToken Connection;
};

ref class QWinRTMidiDeviceWatcher sealed
{
public:
    QWinRTMidiDeviceWatcher(String ^ midiSelector, Windows::UI::Core::CoreDispatcher ^ dispatcher, QWinRTMidiDeviceWatcherCallback^ callback);
    virtual ~QWinRTMidiDeviceWatcher();
    void Start();
    void Stop();
    IVector<QWinRTMidiDeviceConnection ^>
        ^ portList() { return _portList; } QWinRTMidiDeviceConnection ^ getPort(String ^ s);

private:
    void OnPortAdded(DeviceWatcher^ deviceWatcher, DeviceInformation^ devInfo);
    void OnPortRemoved(DeviceWatcher^ deviceWatcher, DeviceInformationUpdate^ devInfoUpdate);
    void OnPortUpdated(DeviceWatcher^ deviceWatcher, DeviceInformationUpdate^ devInfoUpdate);
    void OnPortEnumCompleted(DeviceWatcher^ deviceWatcher, Object^ obj);

    Windows::Foundation::EventRegistrationToken _portAddedToken;
    Windows::Foundation::EventRegistrationToken _portRemovedToken;
    Windows::Foundation::EventRegistrationToken _portUpdatedToken;
    Windows::Foundation::EventRegistrationToken _portEnumCompleteToken;

    String ^ _midiSelectorString;
    Windows::UI::Core::CoreDispatcher^ _coreDispatcher;
    QWinRTMidiDeviceWatcherCallback^ _callback;
    IVector<QWinRTMidiDeviceConnection ^> ^ _portList;
    DeviceWatcher ^ _deviceWatcher;
    bool _enumCompleted;
};

#endif // MIDIDEVICEWATCHER_H
