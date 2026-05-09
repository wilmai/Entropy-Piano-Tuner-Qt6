#include "mainwindow.h"
#include <qtmidi/qmidiinput.h>
#include <qtmidi/qmidisystemnotifier.h>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QHeaderView>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
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

    QPushButton *addButton = new QPushButton(QString::fromLatin1("Add"), this);
    conLayout->addSpacing(20);
    conLayout->addWidget(addButton);

    connect(mInputDevices, SIGNAL(currentIndexChanged(int)), this, SLOT(inputIndexChanged(int)));
    connect(mOutputDevices, SIGNAL(currentIndexChanged(int)), this, SLOT(outputIndexChanged(int)));
    connect(mForcedOutputChannel, SIGNAL(currentIndexChanged(int)), this, SLOT(forcedChannelChanged(int)));
    connect(addButton, SIGNAL(pressed()), this, SLOT(onAddConnection()));

    // connection list
    mConnections = new QTableWidget(this);
    mConnections->setColumnCount(3);
    mConnections->setHorizontalHeaderLabels(QStringList() << "Input" << "Output" << "Delete");
    mainLayout->addWidget(mConnections);

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

void MainWindow::onAddConnection() {
    auto inInfo= mInputDevices->currentData().value<QMidiDeviceInfo>();
    const QMidiInput *input = accessInput(inInfo);

    auto outInfo = mOutputDevices->currentData().value<QMidiDeviceInfo>();
    const QMidiOutput *output = accessOutput(outInfo);

    if (input && output) {
        QTableWidgetItem *inItem = new QTableWidgetItem(inInfo.deviceName());
        inItem->setData(Qt::UserRole, QVariant::fromValue(inInfo));
        QTableWidgetItem *outItem = new QTableWidgetItem(outInfo.deviceName());
        outItem->setData(Qt::UserRole, QVariant::fromValue(outInfo));

        mConnections->setRowCount(mConnections->rowCount() + 1);
        mConnections->setItem(mConnections->rowCount() - 1, 0, inItem);
        mConnections->setItem(mConnections->rowCount() - 1, 1, outItem);
        QPushButton *button = new QPushButton(QString::fromLatin1("Delete"));
        connect(button, SIGNAL(pressed()), this, SLOT(onDeleteConnection()));
        mConnections->setCellWidget(mConnections->rowCount() - 1, 2, button);
        connect(input, &QMidiInput::notify, output, &QMidiOutput::receiveMidiMessage);
    }
}

void MainWindow::onDeleteConnection() {
    for (int i = 0; i < mConnections->rowCount(); ++i) {
        if (mConnections->cellWidget(i, 2) == sender()) {
            deleteRow(i);
            break;
        }
    }
}

void MainWindow::deleteRow(int index) {
    if (index < 0 || index >= mConnections->rowCount()) {return;}

    auto inInfo = mConnections->item(index, 0)->data(Qt::UserRole).value<QMidiDeviceInfo>();
    auto outInfo = mConnections->item(index, 1)->data(Qt::UserRole).value<QMidiDeviceInfo>();

    const QMidiInput *input = getInput(inInfo);
    const QMidiOutput *output = getOutput(outInfo);

    if (input && output) {
        disconnect(input, &QMidiInput::notify, output, &QMidiOutput::receiveMidiMessage);
    }

    release(inInfo, QMidi::MidiInput);
    release(outInfo, QMidi::MidiOutput);

    mConnections->removeRow(index);
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
    for (int i = 0 ; i < mConnections->rowCount(); ) {
        auto inInfo = mConnections->item(i, 0)->data(Qt::UserRole).value<QMidiDeviceInfo>();
        if (info == inInfo) {
            deleteRow(i);
        } else {
            ++i;
        }
    }
}

void MainWindow::outputDeviceDetached(const QMidiDeviceInfo info) {
    addLine(QString("Output device detached: %1").arg(info.deviceName()));
    mOutputDevices->removeItem(mOutputDevices->findData(QVariant::fromValue(info)));
    for (int i = 0 ; i < mConnections->rowCount(); ) {
        auto outInfo = mConnections->item(i, 1)->data(Qt::UserRole).value<QMidiDeviceInfo>();
        if (info == outInfo) {
            deleteRow(i);
        } else {
            ++i;
        }
    }
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

void MainWindow::inputIndexChanged(int index) {
}

void MainWindow::outputIndexChanged(int index) {
}

void MainWindow::forcedChannelChanged(int index) {
    bool forcePort = true;
    quint8 portId = 0;
    if (index == 0) {
        forcePort = false;
    } else {
        portId = static_cast<quint8>(mForcedOutputChannel->itemData(index).toInt());
    }

    auto outInfo = mOutputDevices->currentData().value<QMidiDeviceInfo>();
    QMidiOutput *output = getOutput(outInfo);
    if (output) {
        output->setForceTargetChannel(forcePort, portId);
    }
}

QMidiInput* MainWindow::accessInput(const QMidiDeviceInfo &info) {
    return qobject_cast<QMidiInput*>(access(info, QMidi::MidiInput));
}

QMidiOutput* MainWindow::accessOutput(const QMidiDeviceInfo &info) {
    return qobject_cast<QMidiOutput*>(access(info, QMidi::MidiOutput));
}

QMidiDevice* MainWindow::access(const QMidiDeviceInfo &info, QMidi::Mode mode) {
    for (auto p : mCurrentDevices[mode]) {
        if (p.second->deviceInfo() == info) {
            ++p.first;
            return p.second;
        }
    }

    QMidiDevice *dev;
    if (mode == QMidi::MidiInput) {
        dev = new QMidiInput(info, this);
    } else {
        dev = new QMidiOutput(info, this);
    }

    mCurrentDevices[mode].append(qMakePair(1, dev));
    return dev;
}

void MainWindow::release(const QMidiDeviceInfo &info, QMidi::Mode mode) {
    for (auto it = mCurrentDevices[mode].begin(); it != mCurrentDevices[mode].end(); ++it) {
        if (it->second->deviceInfo() == info) {
            --it->first;
            if (it->first <= 0) {
                it->second->deleteLater();
                mCurrentDevices[mode].erase(it);
                return;
            }
        }
    }
}

QMidiInput* MainWindow::getInput(const QMidiDeviceInfo &info) {
    return qobject_cast<QMidiInput*>(get(info, QMidi::MidiInput));
}

QMidiOutput* MainWindow::getOutput(const QMidiDeviceInfo &info) {
    return qobject_cast<QMidiOutput*>(get(info, QMidi::MidiOutput));
}

QMidiDevice* MainWindow::get(const QMidiDeviceInfo &info, QMidi::Mode mode) {
    for (auto p : mCurrentDevices[mode]) {
        if (p.second->deviceInfo() == info) {
            return p.second;
        }
    }

    return nullptr;
}
