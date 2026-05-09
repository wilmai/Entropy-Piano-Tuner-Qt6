#ifndef QDECLARATIVEMIDIINPUTDEVICE_P_H
#define QDECLARATIVEMIDIINPUTDEVICE_P_H

#include "qdeclarativemididevice_p.h"

QT_BEGIN_NAMESPACE

class QDeclarativeMidiInputDevice : public QDeclarativeMidiDevice {
    Q_OBJECT

public:
    explicit QDeclarativeMidiInputDevice(QObject *parent = nullptr);

signals:
    void midiMessageReceived(const QMidiMessage &message);

private slots:
    void currentInputDeviceChanged();

private:
    Q_DISABLE_COPY(QDeclarativeMidiInputDevice)

    QMetaObject::Connection m_connection;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QT_PREPEND_NAMESPACE(QDeclarativeMidiInputDevice))

#endif // QDECLARATIVEMIDIINPUTDEVICE_P_H
