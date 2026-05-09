#include "qtmidi/qmidisystemnotifier.h"

#include <QTimer>

#include "qtmidi/internal/qmidisystemnotifierprivate.h"

QT_BEGIN_NAMESPACE

QMidiSystemNotifier::QMidiSystemNotifier(QObject *parent)
    : QMidiSystemNotifier(true, parent)
{}

QMidiSystemNotifier::QMidiSystemNotifier(bool notifyOnCurrentDevices, QObject *parent)
    : QObject(parent)
{
    d = QMidiSystemNotifierPrivate::instance();

    connect(d, &QMidiSystemNotifierPrivate::inputDeviceAttached, [this](const QMidiDeviceInfo &info) {
        emit deviceAttached(QMidi::MidiInput, info);
    });
    connect(d, &QMidiSystemNotifierPrivate::outputDeviceAttached, [this](const QMidiDeviceInfo &info) {
        emit deviceAttached(QMidi::MidiOutput, info);
    });
    connect(d, &QMidiSystemNotifierPrivate::inputDeviceDetached, [this](const QMidiDeviceInfo &info) {
        emit deviceDetached(QMidi::MidiInput, info);
    });
    connect(d, &QMidiSystemNotifierPrivate::outputDeviceDetached, [this](const QMidiDeviceInfo &info) {
        emit deviceDetached(QMidi::MidiOutput, info);
    });

    connect(d, &QMidiSystemNotifierPrivate::inputDeviceAttached, this, &QMidiSystemNotifier::inputDeviceAttached);
    connect(d, &QMidiSystemNotifierPrivate::outputDeviceAttached, this, &QMidiSystemNotifier::outputDeviceAttached);
    connect(d, &QMidiSystemNotifierPrivate::inputDeviceDetached, this, &QMidiSystemNotifier::inputDeviceDetached);
    connect(d, &QMidiSystemNotifierPrivate::outputDeviceDetached, this, &QMidiSystemNotifier::outputDeviceDetached);

    connect(d, &QMidiSystemNotifierPrivate::inputDeviceCreated, this, &QMidiSystemNotifier::inputDeviceCreated);
    connect(d, &QMidiSystemNotifierPrivate::outputDeviceCreated, this, &QMidiSystemNotifier::outputDeviceCreated);
    connect(d, &QMidiSystemNotifierPrivate::inputDeviceDeleted, this, &QMidiSystemNotifier::inputDeviceDeleted);
    connect(d, &QMidiSystemNotifierPrivate::outputDeviceDeleted, this, &QMidiSystemNotifier::outputDeviceDeleted);

    if (notifyOnCurrentDevices) {
        // list all current input and output devices, to force emitting of signals if devices are attached
        // This needs to be done on the next frame, because clients subscribe after the constructor ran.
        QTimer::singleShot(10, [this]() {
            for (const QMidiDeviceInfo &dev : QMidiDeviceInfo::availableDevices(QMidi::MidiInput)) {
                emit inputDeviceAttached(dev);
            }
            
            for (const QMidiDeviceInfo &dev : QMidiDeviceInfo::availableDevices(QMidi::MidiOutput)) {
                emit outputDeviceAttached(dev);
            }
        });
    }
}

QMidiSystemNotifier::~QMidiSystemNotifier()
{
    d = nullptr;
}

QT_END_NAMESPACE
