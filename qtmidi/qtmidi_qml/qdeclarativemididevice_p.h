#ifndef QDECLARATIVEMIDIDEVICE_P_H
#define QDECLARATIVEMIDIDEVICE_P_H

#include <QtQml/qqmlparserstatus.h>
#include <QtQml/qqml.h>
#include <QtQml/qjsvalue.h>

#include "qtmidi/qmidiinput.h"

class QMidiAutoConnector;
class QMidiSystemNotifier;

QT_BEGIN_NAMESPACE

class QDeclarativeMidiDevice : public QObject, public QQmlParserStatus
{
    Q_OBJECT

    Q_PROPERTY(QStringList availableDevices READ availableDevices WRITE setAvailableDevices NOTIFY availableDevicesChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(Mode mode READ mode)
    Q_ENUMS(Mode)
    Q_INTERFACES(QQmlParserStatus)
public:
    enum Mode {
        MidiInput = QMidi::MidiInput,
        MidiOutput = QMidi::MidiOutput,
    };

    QDeclarativeMidiDevice(Mode mode, QObject *parent = 0);
    ~QDeclarativeMidiDevice();

    void classBegin() override;
    void componentComplete() override;

    Mode mode() const {return m_mode;}

    const QStringList &availableDevices() const;
    int currentIndex() const;

    QMidiDeviceInfo currentDeviceInfo() const;

public Q_SLOTS:
    void setAvailableDevices(const QStringList &availableDevices);
    void setCurrentIndex(int index);

signals:
    void currentDeviceDetached();
    void currentDeviceToBeDeleted();

Q_SIGNALS:
    void availableDevicesChanged();
    void currentIndexChanged();

    void connectWith(const QMidiDeviceInfo &info, QMidi::Mode mode);

private Q_SLOTS:
    void deviceAttached(QMidi::Mode mode, const QMidiDeviceInfo &info);
    void deviceDetached(QMidi::Mode mode, const QMidiDeviceInfo &info);
    void deviceCreated(const QMidiDevice *device, QMidi::Mode mode);
    void deviceDeleted(const QMidiDeviceInfo &info, QMidi::Mode mode);

private:
    Q_DISABLE_COPY(QDeclarativeMidiDevice)

protected:
    QStringList m_availableDevices;
    QList<QMidiDeviceInfo> m_availableDevicesInfo;
    int m_currentIndex;
    const Mode m_mode;

    QMidiAutoConnector *m_autoConnector;
    QMidiSystemNotifier *m_midiSystemNotifier;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QT_PREPEND_NAMESPACE(QDeclarativeMidiDevice))

#endif // QDECLARATIVEMIDIDEVICE_P_H
