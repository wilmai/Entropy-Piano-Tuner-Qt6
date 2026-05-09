#include "mainwindow.h"
#include <qtmidi/qmidiinput.h>
#include <qtmidi/qmidisystemnotifier.h>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , mCurrentInputDevice(nullptr)
    , mCurrentOutputDevice(nullptr) {
    QWidget* mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);

    // comboboxes
    QHBoxLayout *conLayout = new QHBoxLayout();
    mainLayout->addLayout(conLayout);

    mInputDevices = new QComboBox(this);
    mOutputDevices = new QComboBox(this);
    QLabel *forwardText = new QLabel(QString::fromLatin1("-->"), this);
    conLayout->addWidget(mInputDevices, 10);
    conLayout->addWidget(forwardText, 1);
    conLayout->addWidget(mOutputDevices, 10);

    mForcedOutputChannel = new QComboBox(this);
    mForcedOutputChannel->addItem(QString::fromLatin1("Disable"), QVariant::fromValue((int)-1));
    for (int i = 0; i < 16; ++i) {
        mForcedOutputChannel->addItem(QString::number(i), QVariant::fromValue(i));
    }
    conLayout->addWidget(mForcedOutputChannel);

    connect(mInputDevices, SIGNAL(currentIndexChanged(int)), this, SLOT(inputIndexChanged(int)));
    connect(mOutputDevices, SIGNAL(currentIndexChanged(int)), this, SLOT(outputIndexChanged(int)));
    connect(mForcedOutputChannel, SIGNAL(currentIndexChanged(int)), this, SLOT(forcedChannelChanged(int)));

    // monitor
    mConsoleOutput = new QPlainTextEdit(this);
    mainLayout->addWidget(mConsoleOutput);

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
    connect(notifier, &QMidiSystemNotifier::outputDeviceCreated, this, &MainWindow::outputDeviceCreated);
    connect(notifier, &QMidiSystemNotifier::inputDeviceDeleted, this, &MainWindow::inputDeviceDeleted);
    connect(notifier, &QMidiSystemNotifier::outputDeviceDeleted, this, &MainWindow::outputDeviceDeleted);
}

MainWindow::~MainWindow() {
}

void MainWindow::inputDeviceAttached(const QMidiDeviceInfo info) {
    addLine(QString("Input device attached: %1").arg(info.deviceName()));
    mInputDevices->addItem(info.deviceName(), QVariant::fromValue(info));
}

void MainWindow::outputDeviceAttached(const QMidiDeviceInfo info) {
    addLine(QString("Output device attached: %1").arg(info.deviceName()));
    mOutputDevices->addItem(info.deviceName(), QVariant::fromValue(info));
}

void MainWindow::inputDeviceDetached(const QMidiDeviceInfo info) {
    addLine(QString("Input device detached: %1").arg(info.deviceName()));
    mInputDevices->removeItem(mInputDevices->findData(QVariant::fromValue(info)));
}

void MainWindow::outputDeviceDetached(const QMidiDeviceInfo info) {
    addLine(QString("Output device detached: %1").arg(info.deviceName()));
    mOutputDevices->removeItem(mOutputDevices->findData(QVariant::fromValue(info)));
}

void MainWindow::inputDeviceCreated(const QMidiInput* device) {
    addLine(QString("Input device created: %1").arg(device->deviceInfo().deviceName()));
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

void MainWindow::inputIndexChanged(int index) {
    if (mCurrentInputDevice) {
        mCurrentInputDevice->deleteLater();
        mCurrentInputDevice = nullptr;
    }

    if (index >= 0) {
        const QMidiDeviceInfo info = mInputDevices->itemData(index).value<QMidiDeviceInfo>();
        if (info.isNull()) {
            return;
        }
        mCurrentInputDevice = new QMidiInput(info, this);
        connect(mCurrentInputDevice, &QMidiInput::notify, this, &MainWindow::receiveMessage);
        if (mCurrentOutputDevice) {
            connect(mCurrentInputDevice, &QMidiInput::notify, mCurrentOutputDevice, &QMidiOutput::receiveMidiMessage);
        }
    }
}

void MainWindow::outputIndexChanged(int index) {
    if (mCurrentOutputDevice) {
        mCurrentOutputDevice->deleteLater();
    }

    if (index >= 0) {
        const QMidiDeviceInfo info = mOutputDevices->itemData(index).value<QMidiDeviceInfo>();
        if (info.isNull()) {
            return;
        }
        mCurrentOutputDevice = new QMidiOutput(info, this);
        if (mCurrentInputDevice) {
            connect(mCurrentInputDevice, &QMidiInput::notify, mCurrentOutputDevice, &QMidiOutput::receiveMidiMessage);
        }
    }
}

void MainWindow::forcedChannelChanged(int index) {
    bool forcePort = true;
    quint8 portId = 0;
    if (index == 0) {
        forcePort = false;
    } else {
        portId = static_cast<quint8>(mForcedOutputChannel->itemData(index).toInt());
    }

    if (mCurrentOutputDevice) {
        mCurrentOutputDevice->setForceTargetChannel(forcePort, portId);
    }
}

