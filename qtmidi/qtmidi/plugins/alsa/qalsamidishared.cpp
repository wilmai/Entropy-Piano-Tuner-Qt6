#include "qalsamidishared.h"
#include <QtCore/qdebug.h>
#include <alsa/asoundef.h>
#include "qalsamidiinbackend.h"

QT_BEGIN_NAMESPACE



bool QAlsaMidiShared::portInfoById(snd_seq_t *seq, snd_seq_port_info_t *pinfo, unsigned int type, const snd_seq_addr_t &addr)
{
    snd_seq_client_info_t *cinfo;
    int client;
    snd_seq_client_info_alloca( &cinfo );

    snd_seq_client_info_set_client( cinfo, -1 );
    while ( snd_seq_query_next_client( seq, cinfo ) >= 0 ) {
        client = snd_seq_client_info_get_client( cinfo );
        if ( client == 0 ) continue;
        // Reset query info
        snd_seq_port_info_set_client( pinfo, client );
        snd_seq_port_info_set_port( pinfo, -1 );
        while ( snd_seq_query_next_port( seq, pinfo ) >= 0 ) {
            // check if matching type
            unsigned int atyp = snd_seq_port_info_get_type( pinfo );
            if (((atyp & SND_SEQ_PORT_TYPE_MIDI_GENERIC) == 0)
                && ((atyp & SND_SEQ_PORT_TYPE_SYNTH) == 0))
                continue;
            unsigned int caps = snd_seq_port_info_get_capability( pinfo );
            if ( ( caps & type ) != type ) continue;

            // check if matching addr
            const snd_seq_addr_t *paddr = snd_seq_port_info_get_addr(pinfo);
            if (paddr->client == addr.client && paddr->port == addr.port) {return true;}
        }
    }

    return false;
}

bool QAlsaMidiShared::portInfoByNumber(snd_seq_t* seq, snd_seq_port_info_t* pinfo, unsigned int type, int portNumber)
{
    snd_seq_client_info_t *cinfo;
    int client;
    int count = 0;
    snd_seq_client_info_alloca( &cinfo );

    snd_seq_client_info_set_client( cinfo, -1 );
    while ( snd_seq_query_next_client( seq, cinfo ) >= 0 ) {
        client = snd_seq_client_info_get_client( cinfo );
        if ( client == 0 ) continue;
        // Reset query info
        snd_seq_port_info_set_client( pinfo, client );
        snd_seq_port_info_set_port( pinfo, -1 );
        while ( snd_seq_query_next_port( seq, pinfo ) >= 0 ) {
            unsigned int atyp = snd_seq_port_info_get_type( pinfo );
            if (((atyp & SND_SEQ_PORT_TYPE_MIDI_GENERIC) == 0)
                && ((atyp & SND_SEQ_PORT_TYPE_SYNTH) == 0))
                continue;
            unsigned int caps = snd_seq_port_info_get_capability( pinfo );
            if ( ( caps & type ) != type ) continue;
            if ( count == portNumber ) return true;
            ++count;
        }
    }

    return false;
}

void *QAlsaMidiShared::alsaMidiHandler( void *ptr )
{
    AlsaMidiInData *data = static_cast<AlsaMidiInData*>(ptr);
    AlsaMidiData *apiData = data->apiData;

    long nBytes;
    unsigned long long time, lastTime;
    bool continueSysex = false;
    bool doDecode = false;
    MidiMessage message;
    int poll_fd_count;
    struct pollfd *poll_fds;

    snd_seq_event_t *ev;
    int result;
    apiData->bufferSize = 32;
    result = snd_midi_event_new( 0, &data->coder );
    if ( result < 0 ) {
        data->doInput = false;
        qWarning() << "alsaMidiHandler: error initializing MIDI event parser!";
        return 0;
    }

    QVector<unsigned char> bufferData(apiData->bufferSize);
    unsigned char *buffer = bufferData.data();
    if ( buffer == NULL ) {
        data->doInput = false;
        snd_midi_event_free( data->coder );
        data->coder = 0;
        qWarning() << "alsaMidiHandler: error initializing buffer memory!";
        return 0;
    }

    snd_midi_event_init( data->coder );
    snd_midi_event_no_status( data->coder, 1 ); // suppress running status messages

    poll_fd_count = snd_seq_poll_descriptors_count( apiData->seq, POLLIN ) + 1;
    poll_fds = (struct pollfd*)alloca( poll_fd_count * sizeof( struct pollfd ));
    snd_seq_poll_descriptors( apiData->seq, poll_fds + 1, poll_fd_count - 1, POLLIN );
    poll_fds[0].fd = apiData->trigger_fds[0];
    poll_fds[0].events = POLLIN;

    while ( data->doInput ) {
        if ( snd_seq_event_input_pending( apiData->seq, 1 ) == 0 ) {
            // No data pending
            if ( poll( poll_fds, poll_fd_count, -1) >= 0 ) {
                if ( poll_fds[0].revents & POLLIN ) {
                    bool dummy;
                    int res = read( poll_fds[0].fd, &dummy, sizeof(dummy) );
                    (void) res;
                }
            }
            continue;
        }

        // If here, there should be data.
        result = snd_seq_event_input( apiData->seq, &ev );
        if ( result == -ENOSPC ) {
            qWarning() << "alsaMidiHandler: MIDI input buffer overrun!";
            continue;
        }
        else if ( result <= 0 ) {
            qWarning() << "alsaMidiHandler: unknown MIDI input error!\n";
            perror("System reports");
            continue;
        }

        // This is a bit weird, but we now have to decode an ALSA MIDI
        // event (back) into MIDI bytes.  We'll ignore non-MIDI types.
        if ( !continueSysex ) message.bytes.clear();

        doDecode = false;
        switch ( ev->type ) {
        case SND_SEQ_EVENT_PORT_SUBSCRIBED:
#if !defined QT_NO_DEBUG
            qDebug() << "alsaMidiHandler: port connection made!";
#endif
            break;

        case SND_SEQ_EVENT_PORT_UNSUBSCRIBED:
            emit data->backend->requestDelete(ev->source);
#if !defined QT_NO_DEBUG
            qWarning() << "alsaMidiHandler: port connection has closed!";
            qDebug() << "sender = " << (int) ev->data.connect.sender.client << ":"
                     << (int) ev->data.connect.sender.port
                     << ", dest = " << (int) ev->data.connect.dest.client << ":"
                     << (int) ev->data.connect.dest.port;
#endif
            break;

        case SND_SEQ_EVENT_QFRAME: // MIDI time code
            if ( !( data->ignoreFlags & 0x02 ) ) doDecode = true;
            break;

        case SND_SEQ_EVENT_TICK: // 0xF9 ... MIDI timing tick
            if ( !( data->ignoreFlags & 0x02 ) ) doDecode = true;
            break;

        case SND_SEQ_EVENT_CLOCK: // 0xF8 ... MIDI timing (clock) tick
            if ( !( data->ignoreFlags & 0x02 ) ) doDecode = true;
            break;

        case SND_SEQ_EVENT_SENSING: // Active sensing
            if ( !( data->ignoreFlags & 0x04 ) ) doDecode = true;
            break;

        case SND_SEQ_EVENT_SYSEX:
            if ( (data->ignoreFlags & 0x01) ) break;
            if ( ev->data.ext.len > apiData->bufferSize ) {
                apiData->bufferSize = ev->data.ext.len;
                bufferData.resize(apiData->bufferSize);
                buffer = bufferData.data();
                if ( buffer == NULL ) {
                    data->doInput = false;
                    qWarning() << "alsaMidiHandler: error resizing buffer memory!";
                    break;
                }
            }

        default:
            doDecode = true;
        }

        if ( doDecode ) {
            nBytes = snd_midi_event_decode( data->coder, buffer, apiData->bufferSize, ev );
            if ( nBytes > 0 ) {
                // The ALSA sequencer has a maximum buffer size for MIDI sysex
                // events of 256 bytes.  If a device sends sysex messages larger
                // than this, they are segmented into 256 byte chunks.  So,
                // we'll watch for this and concatenate sysex chunks into a
                // single sysex message if necessary.
                if ( !continueSysex )
                    message.bytes.assign( buffer, &buffer[nBytes] );
                else
                    message.bytes.insert( message.bytes.end(), buffer, &buffer[nBytes] );

                continueSysex = ( ( ev->type == SND_SEQ_EVENT_SYSEX ) && ( message.bytes.back() != 0xF7 ) );
                if ( !continueSysex ) {
                    // Calculate the time stamp:
                    message.timeStamp = 0.0;

                    // Method 1: Use the system time.
                    //(void)gettimeofday(&tv, (struct timezone *)NULL);
                    //time = (tv.tv_sec * 1000000) + tv.tv_usec;

                    // Method 2: Use the ALSA sequencer event time data.
                    // (thanks to Pedro Lopez-Cabanillas!).
                    time = ( ev->time.time.tv_sec * 1000000 ) + ( ev->time.time.tv_nsec/1000 );
                    lastTime = time;
                    time -= data->lastTime;
                    data->lastTime = lastTime;
                    if ( data->firstMessage == true )
                        data->firstMessage = false;
                    else
                        message.timeStamp = time * 0.000001;
                }
                else {
#if defined(__RTMIDI_DEBUG__)
                    std::cerr << "\nMidiInAlsa::alsaMidiHandler: event parsing error or not a MIDI event!\n\n";
#endif
                }
            }
        }

        if ( message.bytes.size() != 0 && !continueSysex ) {
            if ( data->backend ) {
                data->backend->handleAlsaMidiMessage(ev->source, message);
            }
            else {
                // As long as we haven't reached our queue size limit, push the message.
                if ( data->queue.size < data->queue.ringSize ) {
                    data->queue.ring[data->queue.back++] = message;
                    if ( data->queue.back == data->queue.ringSize )
                        data->queue.back = 0;
                    data->queue.size++;
                }
                else
                    qDebug() << "MidiInAlsa: message queue limit reached!";
            }
        }

        snd_seq_free_event( ev );
    }

    snd_midi_event_free( data->coder );
    data->coder = 0;
    data->thread = data->dummy_thread_id;
    return 0;
}

QT_END_NAMESPACE
