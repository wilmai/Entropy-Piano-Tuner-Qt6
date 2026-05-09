#ifndef QANDROIDMIDIOUTPUT_H
#define QANDROIDMIDIOUTPUT_H

#include <QtCore/qdebug.h>

#include <qtmidi/qmidi.h>
#include <qtmidi/qmidideviceinfo.h>
#include <qtmidi/qmidisystem.h>

#include <QJniObject>

QT_BEGIN_NAMESPACE

class QAndroidMidiOutput : public QAbstractMidiOutput
{
    Q_OBJECT
public:
    QAndroidMidiOutput(const QMidiDeviceInfo &info, QJniObject usbDriver);
    virtual ~QAndroidMidiOutput();

    QMidi::Error error() const override {return QMidi::NoError;}
    QMidi::State state() const override {return QMidi::ConnectedState;}

    void sendMidiMessage(const QMidiMessage& m) const override;

private:
    QJniObject mUsbDriver;
};

QT_END_NAMESPACE

#endif // QANDROIDMIDIOUTPUT_H
