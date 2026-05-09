#include "qtmidi/internal/qmidiautoconnectorprivate.h"

#include <QDataStream>
#include <QIODevice>

#include "qtmidi/qmidisystemnotifier.h"

QT_BEGIN_NAMESPACE

Q_GLOBAL_STATIC(QMidiAutoConnectorPrivate, midiAutoConnector)

QMidiAutoConnectorPrivate* QMidiAutoConnectorPrivate::instance() {
    return midiAutoConnector();
}

QMidiAutoConnectorPrivate::QMidiAutoConnectorPrivate()
{
    mAutoConnect[QMidi::MidiInput] = true;
    mAutoConnect[QMidi::MidiOutput] = true;
    mForceSingleDevice[QMidi::MidiInput] = true;
    mForceSingleDevice[QMidi::MidiOutput] = true;

    mNotifier = new QMidiSystemNotifier();

    QObject::connect(mNotifier, &QMidiSystemNotifier::inputDeviceAttached, [this](const QMidiDeviceInfo info) {
        onAttached(info, QMidi::MidiInput);
    });
    QObject::connect(mNotifier, &QMidiSystemNotifier::inputDeviceDetached, [this](const QMidiDeviceInfo info) {
        onDetached(info, QMidi::MidiInput);
    });
    QObject::connect(mNotifier, &QMidiSystemNotifier::outputDeviceAttached, [this](const QMidiDeviceInfo info) {
        onAttached(info, QMidi::MidiOutput);
    });
    QObject::connect(mNotifier, &QMidiSystemNotifier::outputDeviceDetached, [this](const QMidiDeviceInfo info) {
        onDetached(info, QMidi::MidiOutput);
    });
}

QMidiAutoConnectorPrivate::~QMidiAutoConnectorPrivate()
{
    for (auto mode : {QMidi::MidiInput, QMidi::MidiOutput}) {
        for (auto dev : mCurrentDevices[mode]) {
            delete dev;
        }
    }
    delete mNotifier;
}

void QMidiAutoConnectorPrivate::setAutoConnect(QMidi::Mode mode, bool autoconnect) {
    if (mAutoConnect[mode] != autoconnect) {
        mAutoConnect[mode] = autoconnect;
        emit autoConnectChanged(mode, autoconnect);
        if (mode == QMidi::MidiInput) {
            emit autoConnectToInputChanged(autoconnect);
        } else {
            emit autoConnectToOutputChanged(autoconnect);
        }

        if (autoconnect) {
            // if we are not connected to a device yet, we connect now
            if (mForceSingleDevice[mode]) {
                // only single device => create default device if there is
                // no device present
                if (mCurrentDevices[mode].size() == 0) {
                    createDefaultDevice(mode);
                }
            } else {
                // connect to all devices
                QList<QMidiDeviceInfo> infos = QMidiDeviceInfo::availableDevices(mode);
                for (auto &info : infos) {
                    connectWith(info, mode);
                }
            }
        }
    }
}

void QMidiAutoConnectorPrivate::setForceSingleDevice(QMidi::Mode mode, bool force) {
    if (mForceSingleDevice[mode] != force) {
        mForceSingleDevice[mode] = force;
        emit forceSingleDeviceChanged(mode, force);
        if (mode == QMidi::MidiInput) {
            emit forceSingleInputDeviceChanged(force);
        } else {
            emit forceSingleOutputDeviceChanged(force);
        }

        if (force) {
            // only keep last device
            while (mCurrentDevices[mode].size() > 1) {
                deleteDevice(mCurrentDevices[mode].front(), mode);
            }
        } else {
            // connect to all devices if, auto connect is enabled
            if (mAutoConnect[mode]) {
                QList<QMidiDeviceInfo> infos = QMidiDeviceInfo::availableDevices(mode);
                for (auto &info : infos) {
                    connectWith(info, mode);
                }
            }
        }
    }
}

void QMidiAutoConnectorPrivate::restore(const QByteArray &array, bool notify) {
    QDataStream d(array);
    bool buffer;
    if (notify) {
        for (auto i : {QMidi::MidiInput, QMidi::MidiOutput}) {
            d >> buffer;
            setAutoConnect(i, buffer);
            d >> buffer;
            setForceSingleDevice(i, buffer);
        }
    } else {
        for (auto i : {QMidi::MidiInput, QMidi::MidiOutput}) {
            d >> mAutoConnect[i] >> mForceSingleDevice[i];
        }
    }
}

QByteArray QMidiAutoConnectorPrivate::store() const {
    QByteArray data;
    QDataStream d(&data, QIODevice::WriteOnly);

    for (auto i : {QMidi::MidiInput, QMidi::MidiOutput}) {
        d << mAutoConnect[i] << mForceSingleDevice[i];
    }

    return data;
}

void QMidiAutoConnectorPrivate::connectWith(const QMidiDeviceInfo &info, QMidi::Mode mode)
{
    for (QMidiDevice *dev : mCurrentDevices[mode]) {
        if (dev->deviceInfo() == info) {
            // already connected
            return;
        }
    }

    if (mForceSingleDevice[mode]) {
        deleteAllDevices(mode);
    }
    createDevice(info, mode);
}

void QMidiAutoConnectorPrivate::disconnectFrom(const QMidiDeviceInfo &info, QMidi::Mode mode)
{
    for (auto dev : mCurrentDevices[mode]) {
        if (dev->deviceInfo() == info) {
            emit currentDeviceDetached(dev, mode);
            if (mode == QMidi::MidiInput) {
                emit currentInputDeviceDetached(static_cast<QMidiInput*>(dev));
            } else {
                emit currentOutputDeviceDetached(static_cast<QMidiOutput*>(dev));
            }

            deleteDevice(dev, mode);
            break;
        }
    }

    if (mAutoConnect[mode]) {
        if (mCurrentDevices[mode].size() == 0) {
            // connect to default device
            createDefaultDevice(mode);
        }
    }
}

void QMidiAutoConnectorPrivate::disconnectFromAll(QMidi::Mode mode)
{
    while (mCurrentDevices[mode].size() > 0) {
        auto dev = mCurrentDevices[mode].first();
        emit currentDeviceDetached(dev, mode);
        if (mode == QMidi::MidiInput) {
            emit currentInputDeviceDetached(static_cast<QMidiInput*>(dev));
        } else {
            emit currentOutputDeviceDetached(static_cast<QMidiOutput*>(dev));
        }

        deleteDevice(dev, mode);
    }

    if (mAutoConnect[mode]) {
        createDefaultDevice(mode);
    }
}

void QMidiAutoConnectorPrivate::onAttached(const QMidiDeviceInfo &info, QMidi::Mode mode)
{
    if (mAutoConnect[mode]) {
        connectWith(info, mode);
    }
}

void QMidiAutoConnectorPrivate::onDetached(const QMidiDeviceInfo &info, QMidi::Mode mode)
{
    disconnectFrom(info, mode);
}

void QMidiAutoConnectorPrivate::createDevice(const QMidiDeviceInfo &info, QMidi::Mode mode)
{
    if (info.isNull()) {return;}

    if (mode == QMidi::MidiInput) {
        auto dev = new QMidiInput(info);
        mCurrentDevices[mode].push_back(dev);
        emit inputDeviceCreated(dev);
        emit deviceCreated(dev, mode);
    } else {
        auto dev = new QMidiOutput(info);
        mCurrentDevices[mode].push_back(dev);
        emit outputDeviceCreated(dev);
        emit deviceCreated(dev, mode);
    }
}

void QMidiAutoConnectorPrivate::createDefaultDevice(QMidi::Mode mode)
{
    QMidiDeviceInfo info;
    if (mode == QMidi::MidiInput) {
        info = QMidiDeviceInfo::defaultInputDevice();
    } else {
        info = QMidiDeviceInfo::defaultOutputDevice();
    }

    createDevice(info, mode);
}

void QMidiAutoConnectorPrivate::deleteDevice(QMidiDevice *dev, QMidi::Mode mode)
{
    const QMidiDeviceInfo info = dev->deviceInfo();

    emit deviceDeletedLater(dev, mode);
    emit deviceDeleted(info, mode);
    if (mode == QMidi::MidiInput) {
        emit inputDeviceDeletedLater(static_cast<QMidiInput*>(dev));
        emit inputDeviceDeleted(info);
    } else {
        emit outputDeviceDeletedLater(static_cast<QMidiOutput*>(dev));
        emit outputDeviceDeleted(info);
    }
    mCurrentDevices[mode].removeOne(dev);
    dev->deleteLater();
}

void QMidiAutoConnectorPrivate::deleteAllDevices(QMidi::Mode mode) {
    while (mCurrentDevices[mode].size() > 0) {
        deleteDevice(mCurrentDevices[mode].first(), mode);
    }
}

QT_END_NAMESPACE
