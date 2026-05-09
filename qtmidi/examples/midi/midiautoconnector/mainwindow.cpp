#include "mainwindow.h"
#include <qtmidi/qmidiinput.h>
#include <qtmidi/qmidisystemnotifier.h>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <qtmidi/qmidiautoconnector.h>
#include <QCheckBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    QWidget* mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);

    // listwidgets
    QGridLayout *conLayout = new QGridLayout();
    mainLayout->addLayout(conLayout);


    QCheckBox *autoConnectInput = new QCheckBox("Auto connect", mainWidget);
    QCheckBox *autoConnectOutput = new QCheckBox("Auto connect", mainWidget);

    QCheckBox *forceSingleInput = new QCheckBox("Single device", mainWidget);
    QCheckBox *forceSingleOutput = new QCheckBox("Single device", mainWidget);

    mInputDevices = new QListWidget(this);
    mOutputDevices = new QListWidget(this);

    conLayout->addWidget(new QLabel("Input devices"), 0, 0);
    conLayout->addWidget(new QLabel("Output devices"), 0, 1);

    conLayout->addWidget(autoConnectInput, 1, 0);
    conLayout->addWidget(autoConnectOutput, 1, 1);

    conLayout->addWidget(forceSingleInput, 2, 0);
    conLayout->addWidget(forceSingleOutput, 2, 1);

    conLayout->addWidget(mInputDevices, 3, 0);
    conLayout->addWidget(mOutputDevices, 3, 1);

    // monitor
    mConsoleOutput = new QPlainTextEdit(this);
    mainLayout->addWidget(new QLabel("Monitor"));
    mainLayout->addWidget(mConsoleOutput);

    mConsoleOutput->setReadOnly(true);
    QFont font("Courier New");
    mConsoleOutput->setFont(font);

    // device notifier
    auto notifier = new QMidiSystemNotifier(this);
    connect(notifier, &QMidiSystemNotifier::inputDeviceAttached, this, &MainWindow::inputDeviceAttached);
    connect(notifier, &QMidiSystemNotifier::outputDeviceAttached, this, &MainWindow::outputDeviceAttached);
    connect(notifier, &QMidiSystemNotifier::inputDeviceDetached, this, &MainWindow::inputDeviceDetached);
    connect(notifier, &QMidiSystemNotifier::outputDeviceDetached, this, &MainWindow::outputDeviceDetached);

    connect(notifier, &QMidiSystemNotifier::inputDeviceCreated, this, &MainWindow::inputDeviceCreated);
    connect(notifier, &QMidiSystemNotifier::outputDeviceCreated, this, &MainWindow::outputDeviceCreated);
    connect(notifier, &QMidiSystemNotifier::inputDeviceDeleted, this, &MainWindow::inputDeviceDeleted);
    connect(notifier, &QMidiSystemNotifier::outputDeviceDeleted, this, &MainWindow::outputDeviceDeleted);

    // autoconnector
    auto connector = new QMidiAutoConnector(this);
    connect(connector, &QMidiAutoConnector::deviceCreated, this, &MainWindow::acDeviceCreated);
    connect(this, &MainWindow::connectWith, connector, &QMidiAutoConnector::connectWith);

    // list widget
    connect(mInputDevices, &QListWidget::itemDoubleClicked, this, &MainWindow::onItemDoubleClicked);
    connect(mOutputDevices, &QListWidget::itemDoubleClicked, this, &MainWindow::onItemDoubleClicked);

    // check boxes
    autoConnectInput->setChecked(connector->isAutoConnectEnabled(QMidi::MidiInput));
    autoConnectOutput->setChecked(connector->isAutoConnectEnabled(QMidi::MidiOutput));
    connect(autoConnectInput, &QCheckBox::toggled, connector, &QMidiAutoConnector::setAutoConnectToInput);
    connect(autoConnectOutput, &QCheckBox::toggled, connector, &QMidiAutoConnector::setAutoConnectToOutput);

    forceSingleInput->setChecked(connector->isSingleDeviceForced(QMidi::MidiInput));
    forceSingleOutput->setChecked(connector->isSingleDeviceForced(QMidi::MidiOutput));
    connect(forceSingleInput, &QCheckBox::toggled, connector, &QMidiAutoConnector::setForceSingleInputDevice);
    connect(forceSingleOutput, &QCheckBox::toggled, connector, &QMidiAutoConnector::setForceSingleOutputDevice);

    // monitor
    connect(this, &MainWindow::addLine, mConsoleOutput, &QPlainTextEdit::appendPlainText);


    // startup
    addLine("MIDI Device Monitor");
    addLine("===================");
    addLine("");
}

MainWindow::~MainWindow() {
}

QListWidgetItem *MainWindow::itemByInfo(const QMidiDeviceInfo &info, QMidi::Mode mode) {
    QList<QListWidgetItem*> candidates;
    if (mode == QMidi::MidiInput) {
        candidates = mInputDevices->findItems(info.deviceName(), Qt::MatchExactly);
    } else {
        candidates = mOutputDevices->findItems(info.deviceName(), Qt::MatchExactly);
    }

    for (QListWidgetItem *item : candidates) {
        if (item->data(Qt::UserRole).value<QMidiDeviceInfo>() == info) {
            return item;
        }
    }

    return nullptr;
}

void MainWindow::inputDeviceAttached(const QMidiDeviceInfo info) {
    addLine(QString("Input device attached: %1").arg(info.deviceName()));
    auto data = new QListWidgetItem(info.deviceName(), mInputDevices);
    data->setData(Qt::UserRole, QVariant::fromValue(info));
}

void MainWindow::outputDeviceAttached(const QMidiDeviceInfo info) {
    addLine(QString("Output device attached: %1").arg(info.deviceName()));
    auto data = new QListWidgetItem(info.deviceName(), mOutputDevices);
    data->setData(Qt::UserRole, QVariant::fromValue(info));
}

void MainWindow::inputDeviceDetached(const QMidiDeviceInfo info) {
    addLine(QString("Input device detached: %1").arg(info.deviceName()));

    auto item = itemByInfo(info, QMidi::MidiInput);
    if (item) {delete item;}
}

void MainWindow::outputDeviceDetached(const QMidiDeviceInfo info) {
    addLine(QString("Output device detached: %1").arg(info.deviceName()));

    auto item = itemByInfo(info, QMidi::MidiOutput);
    if (item) {delete item;}
}

void MainWindow::inputDeviceCreated(const QMidiInput* device) {
    addLine(QString("Input device created: %1").arg(device->deviceInfo().deviceName()));

    auto item = itemByInfo(device->deviceInfo(), QMidi::MidiInput);
    if (item) {item->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));}

    connect(device, &QMidiInput::notify, this, &MainWindow::receiveMessage);
}

void MainWindow::outputDeviceCreated(const QMidiOutput* device) {
    addLine(QString("Output device created: %1").arg(device->deviceInfo().deviceName()));

    auto item = itemByInfo(device->deviceInfo(), QMidi::MidiOutput);
    if (item) {item->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));}
}

void MainWindow::inputDeviceDeleted(QMidiDeviceInfo info) {
    addLine(QString("Input device deleted: %1").arg(info.deviceName()));

    auto item = itemByInfo(info, QMidi::MidiInput);
    if (item) {item->setIcon(QIcon());}
}

void MainWindow::outputDeviceDeleted(QMidiDeviceInfo info) {
    addLine(QString("Output device deleted: %1").arg(info.deviceName()));

    auto item = itemByInfo(info, QMidi::MidiOutput);
    if (item) {item->setIcon(QIcon());}
}

void MainWindow::receiveMessage(const QMidiMessage &m) {
    addLine(QString::fromLatin1("QMidiMessage received: ") + m.toJson());
}

void MainWindow::acDeviceCreated(const QMidiDevice* dev, QMidi::Mode mode) {
    auto ac = qobject_cast<QMidiAutoConnector*>(sender());
    if (mode == QMidi::MidiInput) {
        QList<const QMidiOutput *> os = ac->devices<QMidiOutput>();
        for (auto o : os) {
            connect(qobject_cast<const QMidiInput*>(dev), &QMidiInput::notify, o, &QMidiOutput::receiveMidiMessage);
        }
    } else {
        QList<const QMidiInput *> is = ac->devices<QMidiInput>();
        for (auto i : is) {
            connect(i, &QMidiInput::notify, qobject_cast<const QMidiOutput*>(dev), &QMidiOutput::receiveMidiMessage);
        }
    }
}

void MainWindow::onItemDoubleClicked(QListWidgetItem *item) {
    QMidi::Mode mode = QMidi::MidiInput;
    if (item->listWidget() == mOutputDevices) {mode = QMidi::MidiOutput;}

    QMidiDeviceInfo info = item->data(Qt::UserRole).value<QMidiDeviceInfo>();
    emit connectWith(info, mode);
}
