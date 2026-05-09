#ifndef QWINRTMIDIINPUT_H
#define QWINRTMIDIINPUT_H

#include <QtCore/qdebug.h>

#include <qtmidi/qmidi.h>
#include <qtmidi/qmidideviceinfo.h>
#include <qtmidi/qmidisystem.h>

#include "qwinrtmidiwrapper.h"

QT_BEGIN_NAMESPACE

class QWinRTMidiInput : public QAbstractMidiInput
{
    Q_OBJECT
public:
    QWinRTMidiInput(const QMidiDeviceInfo &info, QWinRTMidiWrapper &wrapper);
    virtual ~QWinRTMidiInput();

    QMidi::Error error() const override {return QMidi::NoError;}
    QMidi::State state() const override {return mState;}

private:
    QWinRTMidiWrapper &mWrapper;
    QMidi::State mState;

    QWinRTMidiMessageCallback ^ mMidiMessageCallback;
};

QT_END_NAMESPACE

#endif // QWINRTMIDIINPUT_H
