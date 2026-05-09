#ifndef QANDROIDMIDIPLUGIN_H
#define QANDROIDMIDIPLUGIN_H

#include <qmidisystemplugin.h>
#include <QJniObject>
#include "qandroidmidiinput.h"
#include <QtCore/qmutex.h>

QT_BEGIN_NAMESPACE

class QAndroidMidiPlugin : public QMidiSystemPlugin
{
    Q_OBJECT

    static QAndroidMidiPlugin *thiz;

    typedef QList<QPair<QByteArray, const QAndroidMidiInput*> > ConnectedDevices;
public:
    QAndroidMidiPlugin(QObject* parent = 0);
    virtual ~QAndroidMidiPlugin();

    QList<QByteArray> availableDevices(QMidi::Mode) const Q_DECL_OVERRIDE;
    QAbstractMidiInput* createInput(const QMidiDeviceInfo& info) Q_DECL_OVERRIDE;
    QAbstractMidiOutput* createOutput(const QMidiDeviceInfo& info) Q_DECL_OVERRIDE;
    QString deviceName(const QByteArray& device, QMidi::Mode mode) Q_DECL_OVERRIDE;

    static QAndroidMidiPlugin *instance() {return thiz;}
    static QString toDeviceName(const QByteArray &handle);
    static QByteArray toHandle(const QString &deviceName, QMidi::Mode mode);

    static void fromNativeHandleInputMessage(const QString &deviceName, const QMidiMessage &m);
    static void deviceAttached(const QString &deviceName, QMidi::Mode mode);
    static void deviceDetached(const QString &deviceName, QMidi::Mode mode);

    void registerDevice(const QByteArray &handle, const QAndroidMidiInput *input);
    void removeDevice(const QAndroidMidiInput *input);
private:
    QJniObject mUsbManager;

    QMutex mConnectedDevicesMutex;
    ConnectedDevices mConnectedDevices;
};

QT_END_NAMESPACE

#endif // QANDROIDMIDIPLUGIN_H
