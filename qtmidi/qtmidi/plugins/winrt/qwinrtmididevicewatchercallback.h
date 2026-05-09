#ifndef MIDIDEVICEWATCHERCALLBACK_H
#define MIDIDEVICEWATCHERCALLBACK_H

#include <QtCore/qglobal.h>

#include "qwinrtnativehelper.h"

QT_BEGIN_NAMESPACE

class QWinRTMidiWrapper;
class QWinRTMidiCLIWrapper;

using Platform::String;

ref class QWinRTMidiDeviceWatcherCallback sealed {
private:
    QWinRTMidiDeviceWatcherCallback(QWinRTMidiWrapper* wrapper);

public:
    void deviceAttached(String^ midiSelector, String^ deviceID, String^ humanReadable);
    void deviceDetached(String^ midiSelector, String^ deviceID, String^ humanReadable);

private:
    QWinRTMidiWrapper* mWrapper = nullptr;

    friend class QWinRTMidiCLIWrapper;
};

QT_END_NAMESPACE

#endif // MIDIDEVICEWATCHERCALLBACK_H
