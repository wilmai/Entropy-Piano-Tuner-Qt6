#include "qalsamidiinbackend.h"
#include "qalsamidiinput.h"

QT_BEGIN_NAMESPACE

QAlsaMidiInBackend::QAlsaMidiInBackend(QAlsaMidiPlugin *plugin)
    : QAlsaMidiBackend(QMidi::MidiInput, plugin, SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ)
{
    m_data.backend = this;
    m_data.apiData = &mApiData;

    // Set up the ALSA sequencer client.
    snd_seq_t *seq;
    int result = snd_seq_open(&seq, "default", SND_SEQ_OPEN_DUPLEX, SND_SEQ_NONBLOCK);
    if ( result < 0 ) {
        qCritical() << "QAlsaMidiInBackend: error creating ALSA sequencer client object.";
        return;
    }

    // Set client name.
    snd_seq_set_client_name( seq, QT_MIDI_INPUT_CLIENT_NAME );

    // Save our api-specific connection information.
    auto *data = &mApiData;
    data->seq = seq;
    data->portNum = -1;
    data->trigger_fds[0] = -1;
    data->trigger_fds[1] = -1;

    if ( pipe(data->trigger_fds) == -1 ) {
        qCritical() << "QAlsaMidiInBackend: error creating pipe objects.";
        return;
    }

// Create the input queue
#ifndef QT_MIDI_AVOID_TIMESTAMPING
    data->queue_id = snd_seq_alloc_named_queue(seq, QT_MIDI_INPUT_QUEUE_NAME);
    // Set arbitrary tempo (mm=100) and resolution (240)
    snd_seq_queue_tempo_t *qtempo;
    snd_seq_queue_tempo_alloca(&qtempo);
    snd_seq_queue_tempo_set_tempo(qtempo, 600000);
    snd_seq_queue_tempo_set_ppq(qtempo, 240);
    snd_seq_set_queue_tempo(data->seq, data->queue_id, qtempo);
    snd_seq_drain_output(data->seq);
#endif
}

QAlsaMidiInBackend::~QAlsaMidiInBackend()
{
    // Cleanup.
    close ( mApiData.trigger_fds[0] );
    close ( mApiData.trigger_fds[1] );
#ifndef QT_MIDI_AVOID_TIMESTAMPING
    snd_seq_free_queue( mApiData.seq, mApiData.queue_id );
#endif
    snd_seq_close( mApiData.seq );
}



void QAlsaMidiInBackend::registerDevice(snd_seq_addr_t source, QAlsaMidiInput* in) {
    if (m_data.doInput == false) {
        // event loop not started yet
        startEventLoop();
    }

    mDevices[source] = in;
}

void QAlsaMidiInBackend::unregisterDevice(QAlsaMidiInput* in) {
    for (auto it = mDevices.begin(); it != mDevices.end(); ++it) {
        if (it.value() == in) {
            mDevices.erase(it);
            break;
        }
    }

    if (mDevices.size() == 0) {
        stopEventLoop();
    }
}

void QAlsaMidiInBackend::requestDelete(snd_seq_addr_t source) {
    if (mDevices.contains(source)) {
        QAlsaMidiInput* in = mDevices[source];
        in->requestDelete();
    }
}

void QAlsaMidiInBackend::handleAlsaMidiMessage(snd_seq_addr_t source, const QAlsaMidiShared::MidiMessage &m) const {
    if (mDevices.contains(source)) {
        QAlsaMidiInput* in = mDevices[source];
        in->handleAlsaMidiMessage(m);
    }
}

void QAlsaMidiInBackend::startEventLoop() {
    stopEventLoop();

    m_data.dummy_thread_id = pthread_self();
    m_data.thread = m_data.dummy_thread_id;
    m_data.doInput = true;

    // Start our MIDI input thread.
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setschedpolicy(&attr, SCHED_OTHER);

    m_data.doInput = true;
    int err = pthread_create(&m_data.thread, &attr, QAlsaMidiShared::alsaMidiHandler, &m_data);
    pthread_attr_destroy(&attr);

    if (err) {
        qWarning() << "QAlsaMidiInput::initialize(): error starting MIDI input thread!";
        m_data.doInput = false;
    }
}

void QAlsaMidiInBackend::stopEventLoop() {
    // Shutdown the input thread.
    if ( m_data.doInput ) {
        m_data.doInput = false;
        int res = write( m_data.apiData->trigger_fds[1], &m_data.doInput, sizeof(m_data.doInput) );
        (void) res;
        if ( !pthread_equal(m_data.thread, m_data.dummy_thread_id) )
            pthread_join( m_data.thread, NULL );
    }
}

QT_END_NAMESPACE
