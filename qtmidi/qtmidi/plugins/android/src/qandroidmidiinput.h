#ifndef QANDROIDMIDIINPUT_H
#define QANDROIDMIDIINPUT_H

#include <QtCore/qdebug.h>

#include <qtmidi/qmidi.h>
#include <qtmidi/qmidideviceinfo.h>
#include <qtmidi/qmidisystem.h>

#include <QJniObject>

QT_BEGIN_NAMESPACE

class QAndroidMidiInput : public QAbstractMidiInput
{
    Q_OBJECT

public:
public:
    QAndroidMidiInput(const QMidiDeviceInfo &info, QJniObject usbDriver);
    virtual ~QAndroidMidiInput();

    QMidi::Error error() const override {return QMidi::NoError;}
    QMidi::State state() const override {return mState;}

private:
    QJniObject mUsbDriver;
    QMidi::State mState = QMidi::DisconnectedState;
};

QT_END_NAMESPACE

#endif // QANDROIDMIDIINPUT_H
