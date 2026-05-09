#ifndef QDECLARATIVEMIDIOUTPUTDEVICE_P_H
#define QDECLARATIVEMIDIOUTPUTDEVICE_P_H

#include "qdeclarativemididevice_p.h"

QT_BEGIN_NAMESPACE

class QDeclarativeMidiOutputDevice : public QDeclarativeMidiDevice {
    Q_OBJECT

public:
    explicit QDeclarativeMidiOutputDevice(QObject *parent = nullptr);

    Q_INVOKABLE void sendMessage(const QMidiMessage &message);

private:
    Q_DISABLE_COPY(QDeclarativeMidiOutputDevice)
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QT_PREPEND_NAMESPACE(QDeclarativeMidiOutputDevice))

#endif // QDECLARATIVEMIDIOUTPUTDEVICE_P_H
