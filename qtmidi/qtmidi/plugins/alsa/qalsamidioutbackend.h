#ifndef QALSAMIDIOUTBACKEND_H
#define QALSAMIDIOUTBACKEND_H

#include <QtCore/qglobal.h>

#include "qalsamidideviceinfo.h"
#include "qalsamidibackend.h"


QT_BEGIN_NAMESPACE

class QAlsaMidiOutBackend : public QAlsaMidiBackend
{
public:
    QAlsaMidiOutBackend(QAlsaMidiPlugin *plugin);
    virtual ~QAlsaMidiOutBackend();

    void sendMessage(int vport, const QMidiMessage &message);

protected:
    QAlsaMidiShared::AlsaMidiOutBackendData m_data;
};

QT_END_NAMESPACE

#endif // QALSAMIDIOUTBACKEND_H
