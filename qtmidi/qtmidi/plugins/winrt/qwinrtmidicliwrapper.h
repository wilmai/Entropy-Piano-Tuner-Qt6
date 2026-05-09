#ifndef UMIDICLIWRAPPER_H
#define UMIDICLIWRAPPER_H

#include <QtCore/qglobal.h>

QT_BEGIN_NAMESPACE

class QWinRTMidiWrapper;
ref class QWinRTMidiDeviceWatcher;
ref class QWinRTMidiMessageCallback;

class QWinRTMidiCLIWrapper
{
public:
    QWinRTMidiCLIWrapper(QWinRTMidiWrapper *wrapper);

    QWinRTMidiDeviceWatcher ^ GetMidiInDeviceWatcher();
    QWinRTMidiDeviceWatcher ^ GetMidiOutDeviceWatcher();
    Windows::UI::Core::CoreDispatcher ^ GetDispatcher();

private:
    QWinRTMidiDeviceWatcher ^ mMidiInDeviceWatcher;
    QWinRTMidiDeviceWatcher ^ mMidiOutDeviceWatcher;

    Windows::UI::Core::CoreDispatcher ^ mDispatcher;
};

QT_END_NAMESPACE

#endif // UMIDICLIWRAPPER_H
