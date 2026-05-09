#ifndef QMIDIAUTOCONNECTORPRIVATE_P_H
#define QMIDIAUTOCONNECTORPRIVATE_P_H

#include <qtmidi/qmidi.h>
#include <qtmidi/qmidiinput.h>
#include <qtmidi/qmidioutput.h>

QT_BEGIN_NAMESPACE

class QMidiAutoConnector;
class QMidiSystemNotifier;

class QMidiAutoConnectorPrivate : public QObject {
    Q_OBJECT
public:
    static QMidiAutoConnectorPrivate *instance();

    QMidiAutoConnectorPrivate();
    virtual ~QMidiAutoConnectorPrivate();

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

public:
    bool isAutoConnectEnabled(QMidi::Mode mode) const {return mAutoConnect[mode];}
    bool isSingleDeviceForced(QMidi::Mode mode) const {return mForceSingleDevice[mode];}

    void setAutoConnect(QMidi::Mode mode, bool autoconnect = true);
    void setForceSingleDevice(QMidi::Mode mode, bool force = true);
    void restore(const QByteArray &array, bool notify = true);
    QByteArray store() const;
    void connectWith(const QMidiDeviceInfo &info, QMidi::Mode mode);
    void disconnectFrom(const QMidiDeviceInfo &info, QMidi::Mode mode);
    void disconnectFromAll(QMidi::Mode mode);

private:
    void onAttached(const QMidiDeviceInfo &info, QMidi::Mode mode);
    void onDetached(const QMidiDeviceInfo &info, QMidi::Mode mode);
    void createDevice(const QMidiDeviceInfo &info, QMidi::Mode mode);
    void createDefaultDevice(QMidi::Mode mode);
    void deleteDevice(QMidiDevice *dev, QMidi::Mode mode);
    void deleteAllDevices(QMidi::Mode mode);

private:
    // always two for input and output
    bool mAutoConnect[2];
    bool mForceSingleDevice[2];
    QList<QMidiDevice*> mCurrentDevices[2];
    QMidiSystemNotifier *mNotifier;
    QMidiAutoConnector *q;

    friend class QMidiAutoConnector;
};

QT_END_NAMESPACE

#endif // QMIDIAUTOCONNECTORPRIVATE_P_H
