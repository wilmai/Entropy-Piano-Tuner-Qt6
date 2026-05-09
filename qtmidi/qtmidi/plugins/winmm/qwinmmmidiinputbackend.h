#ifndef QWINMMMIDIINPUTBACKEND_H
#define QWINMMMIDIINPUTBACKEND_H

#include <QtCore/qglobal.h>
#include "qwinmmmidibackend.h"

class QWinMMMidiInputBackend : public QWinMMMidiBackend
{
public:
    QWinMMMidiInputBackend(QWinMMMidiPlugin &midiPlugin);

    virtual unsigned int getPortCount() const override;
    virtual QString getPortName(unsigned int portNumber) const override;
};

#endif // QWINMMMIDIINPUTBACKEND_H
