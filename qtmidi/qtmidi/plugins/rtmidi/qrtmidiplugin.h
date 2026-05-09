#ifndef QRTMIDIPLUGIN_H
#define QRTMIDIPLUGIN_H

#include <qtmidi/qmidisystemplugin.h>

QT_BEGIN_NAMESPACE

class QRtMidiPlugin : public QMidiSystemPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "org.qt-project.qt.midisystemfactory/5.0" FILE "rtmidi.json")

public:
    QRtMidiPlugin(QObject* parent = 0);
    ~QRtMidiPlugin() {}

    QList<QByteArray> availableDevices(QMidi::Mode) const Q_DECL_OVERRIDE;
    QAbstractMidiInput* createInput(const QByteArray& device) Q_DECL_OVERRIDE;
    QAbstractMidiOutput* createOutput(const QByteArray& device) Q_DECL_OVERRIDE;
    QAbstractMidiDeviceInfo* createDeviceInfo(const QByteArray& device, QMidi::Mode mode) Q_DECL_OVERRIDE;
};

QT_END_NAMESPACE

#endif // QRTMIDIPLUGIN_H
