#ifndef QMIDISYSTEMNOTIFIERPRIVATE_H
#define QMIDISYSTEMNOTIFIERPRIVATE_H

#include <QtCore/qobject.h>
#include "qmidi.h"
#include "qmidideviceinfo.h"

class QMidiInput;
class QMidiOutput;

QT_BEGIN_NAMESPACE

class QMidiSystemNotifierPrivate : public QObject
{
public:
    Q_OBJECT

public:
    static QMidiSystemNotifierPrivate *instance();

    QMidiSystemNotifierPrivate();

    void sendAttached(const QMidiDeviceInfo& deviceInfo, QMidi::Mode mode);
    void sendDetached(const QMidiDeviceInfo& deviceInfo, QMidi::Mode mode);

Q_SIGNALS:
    void inputDeviceAttached(const QMidiDeviceInfo& deviceInfo);
    void outputDeviceAttached(const QMidiDeviceInfo& deviceInfo);
    void inputDeviceDetached(const QMidiDeviceInfo& deviceInfo);
    void outputDeviceDetached(const QMidiDeviceInfo& deviceInfo);

    void inputDeviceCreated(const QMidiInput* device);
    void outputDeviceCreated(const QMidiOutput* device);
    void inputDeviceDeleted(QMidiDeviceInfo info);
    void outputDeviceDeleted(QMidiDeviceInfo info);
};

QT_END_NAMESPACE

#endif // QMIDISYSTEMNOTIFIERPRIVATE_H
