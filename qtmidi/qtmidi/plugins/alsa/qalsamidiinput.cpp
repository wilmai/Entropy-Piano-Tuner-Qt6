#include "qalsamidiinput.h"
#include "qalsamidiinbackend.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDateTime>
#include "qalsamidiplugin.h"

QT_BEGIN_NAMESPACE

QAlsaMidiInput::QAlsaMidiInput(const QMidiDeviceInfo &info, QAlsaMidiInBackend *backend)
    : QAbstractMidiInput(info)
    , m_device(handle())
    , m_portAddr(QAlsaMidiBackend::handleToId(m_device))
    , m_backend(backend)
{
    errorState = QMidi::NoError;
    mState = QMidi::DisconnectedState;

    m_data.apiData = &backend->apiData();
    m_data.inBackendData = backend->inputData();
    m_data.inputDevice = this;

    initialize();
}

QAlsaMidiInput::~QAlsaMidiInput()
{
    if (mState == QMidi::ConnectedState) {
        if (m_data.subscription) {
            snd_seq_unsubscribe_port(m_data.apiData->seq, m_data.subscription);
            snd_seq_port_subscribe_free(m_data.subscription );
            m_data.subscription = nullptr;
        }

#ifndef QT_MIDI_AVOID_TIMESTAMPING
        snd_seq_stop_queue(m_data.apiData->seq, m_data.apiData->queue_id, NULL);
        snd_seq_drain_output(m_data.apiData->seq);
#endif
    }

    mState = QMidi::DisconnectedState;

    if ( m_data.vport >= 0 ) {
        snd_seq_delete_port( m_data.apiData->seq, m_data.vport );
    }

    m_backend->unregisterDevice(this);

    QCoreApplication::processEvents();
}

QMidi::Error QAlsaMidiInput::error() const
{
    return errorState;
}

QMidi::State QAlsaMidiInput::state() const
{
    return mState;
}

void QAlsaMidiInput::handleAlsaMidiMessage(const QAlsaMidiShared::MidiMessage &m) const
{
    QVector<quint8> message(m.bytes.size());
    for (int i = 0; i < message.size(); ++i) {message[i] = m.bytes[i];}
    handleMidiMessage(QMidiMessage(message, QDateTime::currentMSecsSinceEpoch()));
}

void QAlsaMidiInput::initialize()
{
    snd_seq_port_info_t *src_pinfo;
    snd_seq_port_info_alloca( &src_pinfo );
    auto *data = &m_backend->apiData();
    if ( QAlsaMidiShared::portInfoById( data->seq, src_pinfo, SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ, m_portAddr ) == 0 ) {
        qWarning() << "QAlsaMidiInput::initialize(): the 'portNumber' argument (" << m_portAddr.client << m_portAddr.port << ") is invalid.";
        errorState = QMidi::OpenError;
        return;
    }

    snd_seq_addr_t sender, receiver;
    sender.client = snd_seq_port_info_get_client( src_pinfo );
    sender.port = snd_seq_port_info_get_port( src_pinfo );
    receiver.client = snd_seq_client_id( data->seq );

    snd_seq_port_info_t *pinfo;
    snd_seq_port_info_alloca( &pinfo );
    if ( m_data.vport < 0 ) {
        snd_seq_port_info_set_client( pinfo, 0 );
        snd_seq_port_info_set_port( pinfo, 0 );
        snd_seq_port_info_set_capability(pinfo,
                                         SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE);
        snd_seq_port_info_set_type(pinfo,
                                   SND_SEQ_PORT_TYPE_MIDI_GENERIC | SND_SEQ_PORT_TYPE_APPLICATION);
        snd_seq_port_info_set_midi_channels(pinfo, 16);
#ifndef QT_MIDI_AVOID_TIMESTAMPING
        snd_seq_port_info_set_timestamping(pinfo, 1);
        snd_seq_port_info_set_timestamp_real(pinfo, 1);
        snd_seq_port_info_set_timestamp_queue(pinfo, data->queue_id);
#endif
        snd_seq_port_info_set_name(pinfo, QT_MIDI_INPUT_CLIENT_NAME );
        m_data.vport = snd_seq_create_port(data->seq, pinfo);

        if ( m_data.vport < 0 ) {
            qWarning() << "QAlsaMidiInput::initialize(): ALSA error creating input port.";
            errorState = QMidi::OpenError;
            return;
        }
        m_data.vport = snd_seq_port_info_get_port(pinfo);
    }

    receiver.port = m_data.vport;

    if ( !m_data.subscription ) {
        // Make subscription
        if (snd_seq_port_subscribe_malloc( &m_data.subscription ) < 0) {
            qWarning() << "QAlsaMidiInput::initialize(): ALSA error allocation port subscription.";
            errorState = QMidi::OpenError;
            return;
        }
        snd_seq_port_subscribe_set_sender(m_data.subscription, &sender);
        snd_seq_port_subscribe_set_dest(m_data.subscription, &receiver);
        if ( snd_seq_subscribe_port(data->seq, m_data.subscription) ) {
            snd_seq_port_subscribe_free( m_data.subscription );
            m_data.subscription = 0;
            qWarning() << "QAlsaMidiInput::initialize(): ALSA error making port connection.";
            errorState = QMidi::OpenError;
            return;
        }
    }

// Start the input queue
#ifndef QT_MIDI_AVOID_TIMESTAMPING
    snd_seq_start_queue( data->seq, data->queue_id, NULL );
    snd_seq_drain_output( data->seq );
#endif

    m_data.source = sender;
    mState = QMidi::ConnectedState;
    m_backend->registerDevice(sender, this);
}

QT_END_NAMESPACE

#include "moc_qalsamidiinput.cpp"
