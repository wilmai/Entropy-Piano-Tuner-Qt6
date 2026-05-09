#ifndef QMIDIAUTOCONNECTOR_H
#define QMIDIAUTOCONNECTOR_H

#include <qtmidi/qmidi.h>
#include <qtmidi/qmidiinput.h>
#include <qtmidi/qmidioutput.h>

QT_BEGIN_NAMESPACE

class QMidiAutoConnectorPrivate;

class Q_MIDI_EXPORT QMidiAutoConnector : public QObject
{
    Q_OBJECT
public:
    QMidiAutoConnector(QObject *parent = nullptr);
    virtual ~QMidiAutoConnector();

    bool isAutoConnectEnabled(QMidi::Mode mode) const;
    bool isSingleDeviceForced(QMidi::Mode mode) const;
    bool allowMultiDevices(QMidi::Mode mode) const;

    QMidiDevice *device(const QMidi::Mode mode) const;

    template <class DeviceType>
    QList<const DeviceType*> devices() const;

    template <class DeviceType>
    const DeviceType* device() const;

    template <class DeviceType>
    const DeviceType* device(const QMidiDeviceInfo &info) const;

    template <class DeviceType>
    DeviceType* device(const QMidiDeviceInfo &info);

Q_SIGNALS:
    void autoConnectChanged(QMidi::Mode mode, bool autoconnect);
    void autoConnectToInputChanged(bool autoconnect);
    void autoConnectToOutputChanged(bool autoconnect);

    void forceSingleDeviceChanged(QMidi::Mode mode, bool force);
    void forceSingleInputDeviceChanged(bool force);
    void forceSingleOutputDeviceChanged(bool force);

    void deviceCreated(const QMidiDevice* dev, QMidi::Mode mode);
    void inputDeviceCreated(const QMidiInput *dev);
    void outputDeviceCreated(const QMidiOutput *dev);

    void deviceDeletedLater(const QMidiDevice* dev, QMidi::Mode mode);
    void inputDeviceDeletedLater(const QMidiInput *dev);
    void outputDeviceDeletedLater(const QMidiOutput *dev);

    void deviceDeleted(const QMidiDeviceInfo info, QMidi::Mode mode);
    void inputDeviceDeleted(const QMidiDeviceInfo info);
    void outputDeviceDeleted(const QMidiDeviceInfo info);

    void currentDeviceDetached(const QMidiDevice *dev, QMidi::Mode mode);
    void currentInputDeviceDetached(const QMidiInput*dev);
    void currentOutputDeviceDetached(const QMidiOutput*dev);

public slots:
    void setAutoConnect(QMidi::Mode mode, bool autoconnect = true);
    void setAutoConnectToInput(bool autoconnect = true);
    void setAutoConnectToOutput(bool autoconnect = true);

    void setForceSingleDevice(QMidi::Mode mode, bool force = true);
    void setForceSingleInputDevice(bool force = true);
    void setForceSingleOutputDevice(bool force = true);

    void setAllowMultiDevices(QMidi::Mode mode, bool allow = true);
    void setAllowMultiInputDevices(bool allow = true);
    void setAllowMultiOutputDevices(bool allow = true);

    void restore(const QByteArray &array, bool notify = true);
    QByteArray store() const;

    void connectWith(const QMidiDeviceInfo &info, QMidi::Mode mode);
    void disconnectFrom(const QMidiDeviceInfo &info, QMidi::Mode mode);
    void disconnectFromAll(QMidi::Mode mode);

private:
    QMidiAutoConnectorPrivate *d;
    friend class QMidiAutoConnectorPrivate;
};

QT_END_NAMESPACE

#endif // QMIDIAUTOCONNECTOR_H
