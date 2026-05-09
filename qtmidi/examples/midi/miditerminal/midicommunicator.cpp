#include "midicommunicator.h"
#include <QTextStream>
#include <iostream>
#include <qtmidi/qmidisystemnotifier.h>
#include <qtmidi/qmidiautoconnector.h>

MidiCommunicator::MidiCommunicator()
{
    auto notifier = new QMidiSystemNotifier(true, this);
    connect(notifier, &QMidiSystemNotifier::inputDeviceAttached, this, &MidiCommunicator::inputDeviceAttached);
    connect(notifier, &QMidiSystemNotifier::outputDeviceAttached, this, &MidiCommunicator::outputDeviceAttached);
    connect(notifier, &QMidiSystemNotifier::inputDeviceDetached, this, &MidiCommunicator::inputDeviceDetached);
    connect(notifier, &QMidiSystemNotifier::outputDeviceDetached, this, &MidiCommunicator::outputDeviceDetached);

    connect(notifier, &QMidiSystemNotifier::inputDeviceCreated, this, &MidiCommunicator::inputDeviceCreated);
    connect(notifier, &QMidiSystemNotifier::outputDeviceCreated, this, &MidiCommunicator::outputDeviceCreated);
    connect(notifier, &QMidiSystemNotifier::inputDeviceDeleted, this, &MidiCommunicator::inputDeviceDeleted);
    connect(notifier, &QMidiSystemNotifier::outputDeviceDeleted, this, &MidiCommunicator::outputDeviceDeleted);

    new QMidiAutoConnector(this);
}

void MidiCommunicator::addLine(const QString &s) {
    QTextStream str(stdout);
    str << s << Qt::endl;
}

void MidiCommunicator::inputDeviceAttached(const QMidiDeviceInfo info) {
    addLine(QString("Input device attached: %1").arg(info.deviceName()));
}

void MidiCommunicator::outputDeviceAttached(const QMidiDeviceInfo info) {
    addLine(QString("Output device attached: %1").arg(info.deviceName()));
}

void MidiCommunicator::inputDeviceDetached(const QMidiDeviceInfo info) {
    addLine(QString("Input device detached: %1").arg(info.deviceName()));
}

void MidiCommunicator::outputDeviceDetached(const QMidiDeviceInfo info) {
    addLine(QString("Output device detached: %1").arg(info.deviceName()));
}

void MidiCommunicator::inputDeviceCreated(const QMidiInput* device) {
    addLine(QString("Input device created: %1").arg(device->deviceInfo().deviceName()));
    connect(device, &QMidiInput::notify, this, &MidiCommunicator::receiveMessage);
}

void MidiCommunicator::outputDeviceCreated(const QMidiOutput* device) {
    addLine(QString("Output device created: %1").arg(device->deviceInfo().deviceName()));
}

void MidiCommunicator::inputDeviceDeleted(QMidiDeviceInfo info) {
    addLine(QString("Input device deleted: %1").arg(info.deviceName()));
}

void MidiCommunicator::outputDeviceDeleted(QMidiDeviceInfo info) {
    addLine(QString("Output device deleted: %1").arg(info.deviceName()));
}

void MidiCommunicator::receiveMessage(const QMidiMessage &m) {
    addLine(QString::fromLatin1("QMidiMessage received: ") + m.toJson());
}
