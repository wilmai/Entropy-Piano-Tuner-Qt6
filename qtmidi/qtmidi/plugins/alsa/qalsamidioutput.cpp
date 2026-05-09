#include "qalsamidioutput.h"
#include <QtCore/qcoreapplication.h>
#include "qalsamidioutbackend.h"

QT_BEGIN_NAMESPACE

QAlsaMidiOutput::QAlsaMidiOutput(const QMidiDeviceInfo &info, QAlsaMidiOutBackend *backend)
    : QAbstractMidiOutput(info)
    , m_device(handle())
    , m_backend(backend)
    , m_portAddr(QAlsaMidiBackend::handleToId(handle()))
    , m_vport(-1)
{
    errorState = QMidi::NoError;
    mState = QMidi::DisconnectedState;

    snd_seq_port_info_t *pinfo;
    snd_seq_port_info_alloca( &pinfo );
    const QAlsaMidiShared::AlsaMidiData *api_data = &m_backend->apiData();
    if ( QAlsaMidiShared::portInfoById( api_data->seq, pinfo, SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE, m_portAddr ) == 0 ) {
        qWarning() << "QAlsaMidiOutput::openPort: the 'portNumber' argument (" << m_portAddr.client << m_portAddr.port << ") is invalid.";
        return;
    }

    snd_seq_addr_t sender, receiver;
    receiver.client = snd_seq_port_info_get_client( pinfo );
    receiver.port = snd_seq_port_info_get_port( pinfo );
    sender.client = snd_seq_client_id( api_data->seq );

    if ( m_vport < 0 ) {
        m_vport = snd_seq_create_simple_port(api_data->seq,
                                             info.deviceName().toStdString().c_str(),
                                             SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
                                             SND_SEQ_PORT_TYPE_MIDI_GENERIC
                                                 | SND_SEQ_PORT_TYPE_APPLICATION);
        if ( m_vport < 0 ) {
            qWarning() << "QAlsaMidiOutput::openPort: ALSA error creating output port.";
            return;
        }
    }

    sender.port = m_vport;

    // Make subscription
    if (snd_seq_port_subscribe_malloc( &m_subscription ) < 0) {
        snd_seq_port_subscribe_free( m_subscription );
        qWarning() << "QAlsaMidiOutput: error allocating port subscription.";
        return;
    }

    snd_seq_port_subscribe_set_sender(m_subscription, &sender);
    snd_seq_port_subscribe_set_dest(m_subscription, &receiver);
    snd_seq_port_subscribe_set_time_update(m_subscription, 1);
    snd_seq_port_subscribe_set_time_real(m_subscription, 1);
    if ( snd_seq_subscribe_port(api_data->seq, m_subscription) ) {
        snd_seq_port_subscribe_free( m_subscription );
        qWarning() << "QAlsaMidiObject: ALSA error making port connection.";
        return;
    }

    mState = QMidi::ConnectedState;
}

QAlsaMidiOutput::~QAlsaMidiOutput()
{
    if ( mState == QMidi::ConnectedState ) {
        if (m_vport >= 0) { snd_seq_delete_port( m_backend->apiData().seq, m_vport); }
        snd_seq_unsubscribe_port( m_backend->apiData().seq, m_subscription );
        snd_seq_port_subscribe_free( m_subscription );
        mState = QMidi::DisconnectedState;
    }

    QCoreApplication::processEvents();
}

void QAlsaMidiOutput::sendMidiMessage(const QMidiMessage &m) const
{
    m_backend->sendMessage(m_vport, m);
}

QMidi::Error QAlsaMidiOutput::error() const
{
    return errorState;
}

QT_END_NAMESPACE

#include "moc_qalsamidioutput.cpp"
