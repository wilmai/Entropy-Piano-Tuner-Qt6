#include <QtQml/qqmlinfo.h>

#include "qdeclarativemididevice_p.h"
#include "qtmidi/qmidiautoconnector.h"
#include "qtmidi/qmidisystemnotifier.h"
#include "qtimer.h"

QT_BEGIN_NAMESPACE

QDeclarativeMidiDevice::QDeclarativeMidiDevice(Mode mode, QObject *parent)
    : QObject(parent)
    , m_currentIndex(-1)
    , m_mode(mode)
    , m_autoConnector(nullptr)
    , m_midiSystemNotifier(nullptr)
{
}

QDeclarativeMidiDevice::~QDeclarativeMidiDevice()
{
    delete m_autoConnector;
    delete m_midiSystemNotifier;
}

const QStringList &QDeclarativeMidiDevice::availableDevices() const {
    return m_availableDevices;
}

int QDeclarativeMidiDevice::currentIndex() const {
    return m_currentIndex;
}

QMidiDeviceInfo QDeclarativeMidiDevice::currentDeviceInfo() const {
    if (m_currentIndex >= 0 && m_currentIndex < m_availableDevicesInfo.size()) {
        return m_availableDevicesInfo.at(m_currentIndex);
    }
    return QMidiDeviceInfo();
}

void QDeclarativeMidiDevice::setAvailableDevices(const QStringList &availableDevices) {
    if (m_availableDevices == availableDevices) {
        return;
    }

    m_availableDevices = availableDevices;
    emit availableDevicesChanged();
}

void QDeclarativeMidiDevice::setCurrentIndex(int index) {
    if (m_currentIndex != index) {
        m_currentIndex = index;

        if (index >= 0 && index < m_availableDevicesInfo.size()) {
            emit connectWith(m_availableDevicesInfo[index], static_cast<QMidi::Mode>(m_mode));
        }

        emit currentIndexChanged();
    }
}

void QDeclarativeMidiDevice::classBegin() {
    m_autoConnector = new QMidiAutoConnector(this);
    m_midiSystemNotifier = new QMidiSystemNotifier(this);

    // connect to new devices
    connect(m_midiSystemNotifier, SIGNAL(deviceAttached(QMidi::Mode, QMidiDeviceInfo)), this, SLOT(deviceAttached(QMidi::Mode, QMidiDeviceInfo)));
    connect(m_midiSystemNotifier, SIGNAL(deviceDetached(QMidi::Mode, QMidiDeviceInfo)), this, SLOT(deviceDetached(QMidi::Mode, QMidiDeviceInfo)));
    connect(m_autoConnector, SIGNAL(deviceCreated(const QMidiDevice*,QMidi::Mode)), this, SLOT(deviceCreated(const QMidiDevice*,QMidi::Mode)));
    connect(m_autoConnector, SIGNAL(deviceDeleted(QMidiDeviceInfo,QMidi::Mode)), this, SLOT(deviceDeleted(QMidiDeviceInfo,QMidi::Mode)));
    connect(this, SIGNAL(connectWith(QMidiDeviceInfo,QMidi::Mode)), m_autoConnector, SLOT(connectWith(QMidiDeviceInfo,QMidi::Mode)));

    if (m_mode == MidiInput) {
        connect(m_autoConnector, SIGNAL(currentInputDeviceDetached(const QMidiInput*)), this, SIGNAL(currentDeviceDetached()));
        connect(m_autoConnector, SIGNAL(inputDeviceDeletedLater(const QMidiInput*)), this, SIGNAL(currentDeviceToBeDeleted()));
    } else {
        connect(m_autoConnector, SIGNAL(currentOutputDeviceDetached(const QMidiOutput*)), this, SIGNAL(currentDeviceDetached()));
        connect(m_autoConnector, SIGNAL(outputDeviceDeletedLater(const QMidiOutput*)), this, SIGNAL(currentDeviceToBeDeleted()));
    }

}

void QDeclarativeMidiDevice::componentComplete() {
    // current state of devices
    QMidiDevice *currentDevice = m_autoConnector->device(static_cast<QMidi::Mode>(m_mode));
    QMidiDeviceInfo currentInfo = (currentDevice) ? currentDevice->deviceInfo() : QMidiDeviceInfo();
    int newIndex = -1;
    for (QMidiDeviceInfo info : QMidiDeviceInfo::availableDevices(static_cast<QMidi::Mode>(m_mode))) {
        if (!m_availableDevices.contains(info.deviceName())) {
            m_availableDevicesInfo.append(info);
            m_availableDevices.push_back(info.deviceName());
            if (currentInfo == info) {
                newIndex = m_availableDevices.size() - 1;
            }
        }
    }


    if (m_availableDevices.size() > 0) {
        // shot the changed signales on next run, QMLDesinger has problems elsewise
        QTimer::singleShot(20, [this, newIndex](){
            emit availableDevicesChanged();
            if (newIndex >= 0) {
                setCurrentIndex(newIndex);
            }
        });
    }
}

void QDeclarativeMidiDevice::deviceAttached(QMidi::Mode mode, const QMidiDeviceInfo& info) {
    if (static_cast<Mode>(mode) != m_mode) {return;}
    if (!m_availableDevices.contains(info.deviceName())) {
        m_availableDevices.append(info.deviceName());
        m_availableDevicesInfo.append(info);
        emit availableDevicesChanged();
    }

}

void QDeclarativeMidiDevice::deviceDetached(QMidi::Mode mode, const QMidiDeviceInfo& info) {
    if (static_cast<Mode>(mode) != m_mode) {return;}
    auto pos = m_availableDevicesInfo.indexOf(info);
    if (pos > 0) {
        m_availableDevices.removeAt(pos);
        m_availableDevicesInfo.removeAt(pos);
        emit availableDevicesChanged();
    }
}

void QDeclarativeMidiDevice::deviceCreated(const QMidiDevice *device, QMidi::Mode mode) {
    if (static_cast<Mode>(mode) != m_mode) {return;}

    int index = m_availableDevicesInfo.indexOf(device->deviceInfo());
    if (index < 0) {
        deviceAttached(mode, device->deviceInfo());
        index = m_availableDevicesInfo.indexOf(device->deviceInfo());
    }
    setCurrentIndex(index);
}

void QDeclarativeMidiDevice::deviceDeleted(const QMidiDeviceInfo &info, QMidi::Mode mode) {
    Q_UNUSED(info);

    if (static_cast<Mode>(mode) != m_mode) {return;}
}

QT_END_NAMESPACE
