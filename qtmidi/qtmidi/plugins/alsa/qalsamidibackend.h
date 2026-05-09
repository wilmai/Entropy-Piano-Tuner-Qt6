#ifndef QALSAMIDIBACKEND_H
#define QALSAMIDIBACKEND_H

#include <qalsamidishared.h>

#include <QtCore/qdebug.h>
#include <QtCore/qlist.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qstring.h>


QT_BEGIN_NAMESPACE

class QAbstractMidiDeviceInfo;
class QAlsaMidiPlugin;

class QAlsaMidiBackend
{
public:
    QAlsaMidiBackend(QMidi::Mode mode, QAlsaMidiPlugin* plugin, unsigned int alsaType);
    virtual ~QAlsaMidiBackend();

    QList<QByteArray> availableDevices() const;
    QList<QByteArray> portHandles(snd_seq_t* seq) const;
    QString deviceName(const QByteArray& device) const;
    QByteArray defaultDevice() const;

    const QAlsaMidiShared::AlsaMidiData &apiData() const {return mApiData;}
    QString getPortName(const snd_seq_addr_t &addr) const;

    QAlsaMidiPlugin* plugin() const {return mPlugin;}

    static QByteArray idToHandle(const snd_seq_addr_t &addr);
    static snd_seq_addr_t handleToId(const QByteArray &handle);

protected:
    const QMidi::Mode mMode;
    QAlsaMidiPlugin* mPlugin;
    QAlsaMidiShared::AlsaMidiData mApiData;
    const unsigned int mAlsaType;
};

QT_END_NAMESPACE

#define QT_MIDI_INPUT_CLIENT_NAME "QtMidi::Input"
#define QT_MIDI_OUTPUT_CLIENT_NAME "QtMidi::Output"
#define QT_MIDI_INPUT_QUEUE_NAME "QtMidi::InputQueue"
#define QT_MIDI_OUTPUT_QUEUE_NAME "QtMidi::OutputQueue"


#endif // QALSAMIDIBACKEND_H
