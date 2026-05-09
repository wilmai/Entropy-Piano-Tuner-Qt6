#ifndef QALSAMIDIOUTPUT_H
#define QALSAMIDIOUTPUT_H

#include <alsa/asoundlib.h>

#include <QtCore/qdebug.h>

#include <qtmidi/qmidi.h>
#include <qtmidi/qmidideviceinfo.h>
#include <qtmidi/qmidisystem.h>

QT_BEGIN_NAMESPACE

class QAlsaMidiOutBackend;

class QAlsaMidiOutput : public QAbstractMidiOutput
{
    Q_OBJECT
public:
    QAlsaMidiOutput(const QMidiDeviceInfo &info, QAlsaMidiOutBackend *backend);
    ~QAlsaMidiOutput();

    void sendMidiMessage(const QMidiMessage& m) const override;

    QMidi::Error error() const;
    QMidi::State state() const {return mState;}

    QMidi::Error errorState;

private:
    QByteArray m_device;
    QMidi::State mState;
    QAlsaMidiOutBackend* m_backend;
    const snd_seq_addr_t m_portAddr;
    int m_vport;
    snd_seq_port_subscribe_t *m_subscription = nullptr;
};

QT_END_NAMESPACE

#endif // QALSAMIDIOUTPUT_H
