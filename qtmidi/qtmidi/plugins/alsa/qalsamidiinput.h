#ifndef QALSAMIDIINPUT_H
#define QALSAMIDIINPUT_H

#include <QtCore/qdebug.h>

#include <qtmidi/qmidi.h>
#include <qtmidi/qmidideviceinfo.h>
#include <qtmidi/qmidisystem.h>

#include "qalsamidishared.h"

class QAlsaMidiInBackend;

QT_BEGIN_NAMESPACE

class QAlsaMidiInput : public QAbstractMidiInput
{
    Q_OBJECT
public:
    QAlsaMidiInput(const QMidiDeviceInfo &info, QAlsaMidiInBackend* backend);
    ~QAlsaMidiInput();

    const QAlsaMidiShared::AlsaMidiInDeviceData &data() const {return m_data;}
    QMidi::Error error() const override;
    QMidi::State state() const override;

    QMidi::Error errorState;

    void handleAlsaMidiMessage(const QAlsaMidiShared::MidiMessage &m) const;
private:
    void initialize();

private:
    QByteArray m_device;
    const snd_seq_addr_t m_portAddr;
    QAlsaMidiInBackend* m_backend;
    QAlsaMidiShared::AlsaMidiInDeviceData m_data;

    QMidi::State mState;
};

QT_END_NAMESPACE

#endif // QALSAMIDIINPUT_H
