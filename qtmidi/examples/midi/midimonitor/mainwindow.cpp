#include "mainwindow.h"
#include <qtmidi/qmidiinput.h>
#include <qtmidi/qmidisystemnotifier.h>
#include <qtmidi/qmidiautoconnector.h>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {

    mConsoleOutput = new QPlainTextEdit(this);
    setCentralWidget(mConsoleOutput);

    mConsoleOutput->setReadOnly(true);
    QFont font("Courier New");
    mConsoleOutput->setFont(font);

    connect(this, SIGNAL(addLine(QString)), mConsoleOutput, SLOT(appendPlainText(QString)));

    addLine("MIDI Device Monitor");
    addLine("===================");
    addLine("");

    auto notifier = new QMidiSystemNotifier(this);
    connect(notifier, &QMidiSystemNotifier::inputDeviceAttached, this, &MainWindow::inputDeviceAttached);
    connect(notifier, &QMidiSystemNotifier::outputDeviceAttached, this, &MainWindow::outputDeviceAttached);
    connect(notifier, &QMidiSystemNotifier::inputDeviceDetached, this, &MainWindow::inputDeviceDetached);
    connect(notifier, &QMidiSystemNotifier::outputDeviceDetached, this, &MainWindow::outputDeviceDetached);
    connect(notifier, &QMidiSystemNotifier::inputDeviceCreated, this, &MainWindow::inputDeviceCreated);

    printInputDevices();
    printOutputDevices();
    
    new QMidiAutoConnector(this);
}

MainWindow::~MainWindow() {
}

void MainWindow::printInputDevices() {
    addLine("Available input devices:");
    for (QMidiDeviceInfo &info : QMidiDeviceInfo::availableDevices(QMidi::MidiInput)) {
        addLine(QString("\t- %1").arg(info.deviceName()));
    }
    addLine(QString());
}

void MainWindow::printOutputDevices() {
    addLine("Available output devices:");
    for (QMidiDeviceInfo &info : QMidiDeviceInfo::availableDevices(QMidi::MidiOutput)) {
        addLine(QString("\t- %1").arg(info.deviceName()));
    }
    addLine(QString());
}

void MainWindow::inputDeviceAttached(const QMidiDeviceInfo info) {
    addLine(QString("Input device attached: %1").arg(info.deviceName()));
}

void MainWindow::outputDeviceAttached(const QMidiDeviceInfo info) {
    addLine(QString("Output device attached: %1").arg(info.deviceName()));
}

void MainWindow::inputDeviceDetached(const QMidiDeviceInfo info) {
    addLine(QString("Input device detached: %1").arg(info.deviceName()));
}

void MainWindow::outputDeviceDetached(const QMidiDeviceInfo info) {
    addLine(QString("Output device detached: %1").arg(info.deviceName()));
}

void MainWindow::inputDeviceCreated(const QMidiInput* device) {
    addLine(QString("Input device created: %1").arg(device->deviceInfo().deviceName()));
    connect(device, &QMidiInput::notify, this, &MainWindow::receiveMessage);
}

void MainWindow::outputDeviceCreated(const QMidiOutput* device) {
    addLine(QString("Output device created: %1").arg(device->deviceInfo().deviceName()));
}

void MainWindow::inputDeviceDeleted(QMidiDeviceInfo info) {
    addLine(QString("Input device deleted: %1").arg(info.deviceName()));
}

void MainWindow::outputDeviceDeleted(QMidiDeviceInfo info) {
    addLine(QString("Output device deleted: %1").arg(info.deviceName()));
}

void MainWindow::receiveMessage(const QMidiMessage &m) {
    addLine(QString::fromLatin1("QMidiMessage received: ") + m.toJson());
}
