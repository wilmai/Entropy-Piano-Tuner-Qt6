#ifndef QPGMIDIWRAPPER_H
#define QPGMIDIWRAPPER_H

#include <QtCore/qlist.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qstring.h>

#include <qtmidi/qmidi.h>

QT_BEGIN_NAMESPACE

class QPGMidiPlugin;

class QPGMidiWrapper
{
public:
    QPGMidiWrapper(QPGMidiPlugin &plugin);
    ~QPGMidiWrapper();

    void start();

    QList<QByteArray> availableDevices(QMidi::Mode mode) const;

    void *monitor() {return mMonitor;}
    QPGMidiPlugin &plugin() {return mPlugin;}

private:
    QPGMidiPlugin &mPlugin;
    void *mMonitor;
};

QT_END_NAMESPACE

#endif // QPGMIDIWRAPPER_H
