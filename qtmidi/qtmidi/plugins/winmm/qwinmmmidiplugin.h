#ifndef QWINMMMIDIPLUGIN_H
#define QWINMMMIDIPLUGIN_H


#include <qmidisystemplugin.h>
#include "qwinmmmidiinputbackend.h"
#include "qwinmmmidioutputbackend.h"

QT_BEGIN_NAMESPACE

class QWinMMMidiPlugin : public QMidiSystemPlugin
{
    Q_OBJECT

public:
    QWinMMMidiPlugin(QObject* parent = 0);
    virtual ~QWinMMMidiPlugin();

    QList<QByteArray> availableDevices(QMidi::Mode) const Q_DECL_OVERRIDE;
    QAbstractMidiInput* createInput(const QMidiDeviceInfo& info) Q_DECL_OVERRIDE;
    QAbstractMidiOutput* createOutput(const QMidiDeviceInfo& info) Q_DECL_OVERRIDE;
    QString deviceName(const QByteArray& device, QMidi::Mode mode) Q_DECL_OVERRIDE;

private:
    QWinMMMidiInputBackend mInputBackend;
    QWinMMMidiOutputBackend mOutputBackend;
};

QT_END_NAMESPACE

#endif // QWINMMMIDIPLUGIN_H
