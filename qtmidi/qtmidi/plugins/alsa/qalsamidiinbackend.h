#ifndef QALSAMIDIINBACKEND_H
#define QALSAMIDIINBACKEND_H

#include <QtCore/qglobal.h>
#include <QtCore/qmap.h>

#include "qalsamidideviceinfo.h"
#include "qalsamidibackend.h"


QT_BEGIN_NAMESPACE

class QAlsaMidiInput;

class QAlsaMidiInBackend : public QAlsaMidiBackend
{
public:
    QAlsaMidiInBackend(QAlsaMidiPlugin* plugin);
    virtual ~QAlsaMidiInBackend();

    const QAlsaMidiShared::AlsaMidiInData *inputData() const {return &m_data;}

    void registerDevice(snd_seq_addr_t source, QAlsaMidiInput*in);
    void unregisterDevice(QAlsaMidiInput *in);
    void requestDelete(snd_seq_addr_t source);
    void handleAlsaMidiMessage(snd_seq_addr_t source, const QAlsaMidiShared::MidiMessage &m) const;

private:
    void startEventLoop();
    void stopEventLoop();

private:
    QAlsaMidiShared::AlsaMidiInData m_data;

    QMap<snd_seq_addr_t, QAlsaMidiInput*> mDevices;
};

Q_DECL_CONSTEXPR inline bool operator<(snd_seq_addr_t c1, snd_seq_addr_t c2) Q_DECL_NOTHROW { return c1.client < c2.client; }

QT_END_NAMESPACE

#endif // QALSAMIDIINBACKEND_H
