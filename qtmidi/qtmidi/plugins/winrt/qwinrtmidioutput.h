#ifndef QWINRTMIDIOUTPUT_H
#define QWINRTMIDIOUTPUT_H

#include <QtCore/qdebug.h>

#include <qtmidi/qmidi.h>
#include <qtmidi/qmidideviceinfo.h>
#include <qtmidi/qmidisystem.h>

#include "qwinrtmidiwrapper.h"

QT_BEGIN_NAMESPACE

class QWinRTMidiOutput : public QAbstractMidiOutput
{
    Q_OBJECT
public:
    QWinRTMidiOutput(const QMidiDeviceInfo &info, QWinRTMidiWrapper &wrapper);
    virtual ~QWinRTMidiOutput();

    QMidi::Error error() const override {return QMidi::NoError;}
    QMidi::State state() const override {return mState;}

    void sendMidiMessage(const QMidiMessage& m) const override;

private:
    QWinRTMidiWrapper &mWrapper;
    QMidi::State mState;
};

QT_END_NAMESPACE

#endif // QWINRTMIDIOUTPUT_H
