#ifndef MIDIMESSAGECALLBACK_H
#define MIDIMESSAGECALLBACK_H

#include <QtCore/qglobal.h>

#include <Windows.ApplicationModel.core.h>
#include <collection.h>
#include <ppltasks.h>
#include <wrl.h>
#include "qwinrtnativehelper.h"
#include "qwinrtmididevicewatchercallback.h"
//#include "umidicliwrapper.h"

QT_BEGIN_NAMESPACE

using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Platform;
using namespace Windows::Devices::Enumeration;

using namespace Platform::Collections;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::ApplicationModel::Core;
using namespace Microsoft::WRL;
using namespace concurrency;

using namespace Windows::Devices::Midi;

class QWinRTMidiCLIWrapper;
class QWinRTMidiInput;


ref class QWinRTMidiMessageCallback sealed {
private:
    QWinRTMidiMessageCallback(QWinRTMidiInput* input);

public:
    void onMessageReceived(MidiInPort^ sender, MidiMessageReceivedEventArgs^ args);

private:
    QWinRTMidiInput* mInput;

    friend class QWinRTMidiInput;
};

QT_END_NAMESPACE

#endif // MIDIMESSAGECALLBACK_H
