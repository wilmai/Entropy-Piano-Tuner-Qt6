#ifndef QWINRTMIDIPLUGIN_H
#define QWINRTMIDIPLUGIN_H

#include <qtmidi/qmidisystemplugin.h>
#include "qwinrtmidiwrapper.h"

QT_BEGIN_NAMESPACE

class QWinRTMidiPlugin: public QMidiSystemPlugin, public QWinRTMidiWrapperCallback
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "org.qt-project.qt.midisystemfactory/5.0" FILE "winrt.json")
public:
    QWinRTMidiPlugin(QObject* parent = 0);
    virtual ~QWinRTMidiPlugin();

    QList<QByteArray> availableDevices(QMidi::Mode) const Q_DECL_OVERRIDE;
    QAbstractMidiInput* createInput(const QMidiDeviceInfo& info) Q_DECL_OVERRIDE;
    QAbstractMidiOutput* createOutput(const QMidiDeviceInfo& info) Q_DECL_OVERRIDE;
    QString deviceName(const QByteArray& device, QMidi::Mode mode) Q_DECL_OVERRIDE;

    // QWinRTMidiWrapperCallback
    virtual void inputDeviceAttached(const QString &id, const QString &humanReadable) override;
    virtual void outputDeviceAttached(const QString &id, const QString &humanReadable) override;
    virtual void inputDeviceDetached(const QString &id, const QString &humanReadable) override;
    virtual void outputDeviceDetached(const QString &id, const QString &humanReadable) override;
private:
    QWinRTMidiWrapper mWrapper;
};

QT_END_NAMESPACE

#endif // QWINRTMIDIPLUGIN_H
