#ifndef QWINRTMIDIMESSAGEFACTORY_H
#define QWINRTMIDIMESSAGEFACTORY_H

#include <QtCore/qglobal.h>
#include <qtmidi/qmidimessage.h>

QT_BEGIN_NAMESPACE

using namespace Windows::Devices::Midi;

class QWinRTMidiMessageFactory
{
public:
    static IMidiMessage^ create(int messageType, int byte1, int byte2, int byte3);
    static IMidiMessage^ create(const QMidiMessage &m);
};

QT_END_NAMESPACE

#endif // QWINRTMIDIMESSAGEFACTORY_H
