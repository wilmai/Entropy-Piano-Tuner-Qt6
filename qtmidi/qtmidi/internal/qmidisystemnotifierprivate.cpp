#include "qtmidi/internal/qmidisystemnotifierprivate.h"

#include "qtmidi/qmidideviceinfo.h"
#include "qtmidi/internal/qmididevicefactory.h"

QT_BEGIN_NAMESPACE

Q_GLOBAL_STATIC(QMidiSystemNotifierPrivate, midiSystemNotifier)

QMidiSystemNotifierPrivate *QMidiSystemNotifierPrivate::instance() {
    return midiSystemNotifier();
}

QMidiSystemNotifierPrivate::QMidiSystemNotifierPrivate() {
    // force initialisation of factory
    QMidiDeviceFactory::instantiate();
}

void QMidiSystemNotifierPrivate::sendAttached(const QMidiDeviceInfo& deviceInfo, QMidi::Mode mode) {
    if (mode == QMidi::MidiInput) {
        emit inputDeviceAttached(deviceInfo);
    } else {
        emit outputDeviceAttached(deviceInfo);
    }
}

void QMidiSystemNotifierPrivate::sendDetached(const QMidiDeviceInfo& deviceInfo, QMidi::Mode mode) {
    if (mode == QMidi::MidiInput) {
        emit inputDeviceDetached(deviceInfo);
    } else {
        emit outputDeviceDetached(deviceInfo);
    }
}

QT_END_NAMESPACE
