#ifndef QMIDIDEVICEWATCHER_H
#define QMIDIDEVICEWATCHER_H

#include "qmidi.h"
#include "qmidideviceinfo.h"

QT_BEGIN_NAMESPACE

class QMidiDeviceWatcher : public QObject
{
    Q_OBJECT

public:
    QMidiDeviceWatcher(QMidi::Mode mode, int msec, QObject* parent);
    virtual ~QMidiDeviceWatcher();

private Q_SLOTS:
    void checkNow();

private:
    const QMidi::Mode mMode;
    QList<QMidiDeviceInfo> mLastDeviceInformations;
};

QT_END_NAMESPACE

#endif // QMIDIDEVICEWATCHER_H
