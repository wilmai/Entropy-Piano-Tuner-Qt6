#ifndef QASLAMIDIPLUGIN_H
#define QASLAMIDIPLUGIN_H

#include <qmidisystemplugin.h>

QT_BEGIN_NAMESPACE

class QAlsaMidiInBackend;
class QAlsaMidiOutBackend;

class QAlsaMidiPlugin : public QMidiSystemPlugin
{
    Q_OBJECT

public:
    QAlsaMidiPlugin(QObject* parent = 0);
    ~QAlsaMidiPlugin();

    QList<QByteArray> availableDevices(QMidi::Mode) const Q_DECL_OVERRIDE;
    QAbstractMidiInput* createInput(const QMidiDeviceInfo& info) Q_DECL_OVERRIDE;
    QAbstractMidiOutput* createOutput(const QMidiDeviceInfo& info) Q_DECL_OVERRIDE;
    QString deviceName(const QByteArray& device, QMidi::Mode mode) Q_DECL_OVERRIDE;

private:
    QAlsaMidiInBackend* mIn;
    QAlsaMidiOutBackend* mOut;
};

QT_END_NAMESPACE

#endif // QASLAMIDIPLUGIN_H
