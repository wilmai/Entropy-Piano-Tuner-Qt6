#include "qalsamidioutbackend.h"

QT_BEGIN_NAMESPACE

QAlsaMidiOutBackend::QAlsaMidiOutBackend(QAlsaMidiPlugin *plugin)
    : QAlsaMidiBackend(QMidi::MidiOutput, plugin, SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE)
{
    m_data.apiData = &mApiData;

    // Set up the ALSA sequencer client.
    snd_seq_t *seq;
    int result1 = snd_seq_open( &seq, "default", SND_SEQ_OPEN_OUTPUT, SND_SEQ_NONBLOCK );
    if ( result1 < 0 ) {
        qCritical() << "QAlsaMidiOutBackend: error creating ALSA sequencer client object.";
        return;
    }

    // Set client name.
    snd_seq_set_client_name( seq, QT_MIDI_OUTPUT_CLIENT_NAME );

    // Save our api-specific connection information.
    auto *data = &mApiData;
    data->seq = seq;
    data->portNum = -1;
    mApiData.bufferSize = 32;
    int result = snd_midi_event_new( m_data.bufferSize, &m_data.coder );
    if ( result < 0 ) {
        qCritical() << "QAlsaMidiOutBackend: error initializing MIDI event parser!";
        return;
    }
    m_data.buffer.resize(m_data.bufferSize);
    if (m_data.buffer.data() == nullptr) {
        qCritical() << "QAlsaMidiOutBackend: error allocating buffer memory!";
        return;
    }
    snd_midi_event_init( m_data.coder );
}

QAlsaMidiOutBackend::~QAlsaMidiOutBackend()
{
    // Cleanup.
    //if ( m_data.vport >= 0 ) snd_seq_delete_port( mApiData.seq, m_data.vport );
    if ( m_data.coder ) snd_midi_event_free( m_data.coder );
    snd_seq_close( mApiData.seq );
}

void QAlsaMidiOutBackend::sendMessage(int vport, const QMidiMessage &message)
{
    int result;
    unsigned int nBytes = message.size();
    if ( nBytes > mApiData.bufferSize ) {
        mApiData.bufferSize = nBytes;
        result = snd_midi_event_resize_buffer ( m_data.coder, nBytes);
        if ( result != 0 ) {
            qWarning() << "QAlsaMidiOutBackend::sendMessage: ALSA error resizing MIDI event buffer.";
            return;
        }
        m_data.buffer.resize(m_data.bufferSize);
        if ( m_data.buffer.data() == NULL ) {
            qWarning() << "QAlsaMidiOutBackend::sendMessage: error allocating buffer memory!";
            return;
        }
    }

    snd_seq_event_t ev;
    snd_seq_ev_clear(&ev);
    snd_seq_ev_set_source(&ev, vport);
    snd_seq_ev_set_subs(&ev);
    snd_seq_ev_set_direct(&ev);
    for ( unsigned int i=0; i<nBytes; ++i ) m_data.buffer[i] = message[i];
    result = snd_midi_event_encode( m_data.coder, m_data.buffer.data(), (long)nBytes, &ev );
    if ( result < (int)nBytes ) {
        qWarning() << "QAlsaMidiOutBackend::sendMessage: event parsing error!";
        return;
    }

    // Send the event.
    result = snd_seq_event_output(mApiData.seq, &ev);
    if ( result < 0 ) {
        qWarning() << "QAlsaMidiOutBackend::sendMessage: error sending MIDI message to port.";
        return;
    }
    snd_seq_drain_output(mApiData.seq);
}

QT_END_NAMESPACE
