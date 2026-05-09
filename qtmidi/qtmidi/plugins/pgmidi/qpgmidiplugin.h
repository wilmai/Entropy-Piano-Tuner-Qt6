#ifndef QPGMIDIPLUGIN_H
#define QPGMIDIPLUGIN_H

#include <qtmidi/qmidisystemplugin.h>

#include "qpgmidiwrapper.h"

QT_BEGIN_NAMESPACE

class QPGMidiPlugin : public QMidiSystemPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "org.qt-project.qt.midisystemfactory/5.0" FILE "pgmidi.json")

public:
    QPGMidiPlugin(QObject * parent = 0);


    QList<QByteArray> availableDevices(QMidi::Mode mode) const override;
    QAbstractMidiInput* createInput(const QMidiDeviceInfo& info) override;
    QAbstractMidiOutput* createOutput(const QMidiDeviceInfo &info) override;
    QString deviceName(const QByteArray &device, QMidi::Mode mode) override;

    void notifyAttached(const QByteArray &handle, QMidi::Mode mode);
    void notifyDetached(const QByteArray &handle, QMidi::Mode mode);

private slots:
    void start();

private:
    QPGMidiWrapper mWrapper;
};

QT_END_NAMESPACE

#endif // QPGMIDIPLUGIN_H
