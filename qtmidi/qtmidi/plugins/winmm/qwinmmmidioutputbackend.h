#ifndef QWINMMMIDIOUTPUTBACKEND_H
#define QWINMMMIDIOUTPUTBACKEND_H

#include <QtCore/qglobal.h>
#include "qwinmmmidibackend.h"

QT_BEGIN_NAMESPACE

class QWinMMMidiOutputBackend : public QWinMMMidiBackend
{
public:
    QWinMMMidiOutputBackend(QWinMMMidiPlugin &midiPlugin);

    virtual unsigned int getPortCount() const override;
    virtual QString getPortName(unsigned int portNumber) const override;
};

QT_END_NAMESPACE

#endif // QWINMMMIDIOUTPUTBACKEND_H
