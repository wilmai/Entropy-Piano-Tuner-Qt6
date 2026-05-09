#ifndef QALSAMIDISHARED_H
#define QALSAMIDISHARED_H

#include <alsa/asoundlib.h>

#include <QtCore/qglobal.h>
#include <qtmidi/qmidi.h>
#include <QVector>
#include <QList>

QT_BEGIN_NAMESPACE

class QAlsaMidiInput;
class QAlsaMidiInBackend;

class QAlsaMidiShared
{
public:
    // A structure to hold variables related to the ALSA API
    // implementation.
    struct AlsaMidiData {
        snd_seq_t *seq = nullptr;
        unsigned int portNum = 0;
        int queue_id = -1; // an input queue is needed to get timestamped events
        int trigger_fds[2] = {-1, -1};
        int bufferSize = 0;
    };

    // A MIDI structure used internally by the class to store incoming
    // messages.  Each message represents one and only one MIDI message.
    struct MidiMessage {
        std::vector<unsigned char> bytes;
        double timeStamp = 0.0;
    };

    struct MidiQueue {
        unsigned int front = 0;
        unsigned int back = 0;
        unsigned int size = 0;
        unsigned int ringSize = 0;
        MidiMessage *ring = nullptr;
    };

    struct AlsaMidiInData {
        QAlsaMidiInBackend *backend = nullptr;
        MidiMessage message;
        unsigned char ignoreFlags = 7;
        AlsaMidiData *apiData = nullptr;
        bool continueSysex = false;

        // thread data
        bool doInput = false;
        bool firstMessage = true;
        pthread_t thread;
        pthread_t dummy_thread_id;
        MidiQueue queue;
        snd_midi_event_t *coder;
        unsigned long long lastTime;
    };

    struct AlsaMidiInDeviceData {
        const AlsaMidiData *apiData = nullptr;
        const AlsaMidiInData *inBackendData = nullptr;
        int vport = -1;
        snd_seq_port_subscribe_t *subscription = nullptr;
        QAlsaMidiInput *inputDevice = nullptr;
        snd_seq_addr_t source;
    };

    struct AlsaMidiOutBackendData {
        const AlsaMidiData *apiData = nullptr;
        snd_midi_event_t *coder = nullptr;
        int bufferSize = 32;
        QVector<unsigned char> buffer;
    };

    struct AlsaMidiOutDeviceData {
        const AlsaMidiData *outBackendData = nullptr;
        int vport = -1;
    };

    static bool portInfoById(snd_seq_t *seq, snd_seq_port_info_t *pinfo, unsigned int type, const snd_seq_addr_t &addr);
    static bool portInfoByNumber(snd_seq_t* seq, snd_seq_port_info_t* pinfo, unsigned int type, int portNumber);
    static void *alsaMidiHandler( void *ptr );
};

QT_END_NAMESPACE

#endif // QALSAMIDISHARED_H
