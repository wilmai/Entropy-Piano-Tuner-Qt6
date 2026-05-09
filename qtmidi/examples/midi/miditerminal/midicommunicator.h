#ifndef MIDICOMMUNICATOR_H
#define MIDICOMMUNICATOR_H

#include <QObject>
#include <qtmidi/qmidideviceinfo.h>
#include <qtmidi/qmidiinput.h>
#include <qtmidi/qmidioutput.h>

class MidiCommunicator : public QObject
{
public:
    MidiCommunicator();

protected slots:
    void addLine(const QString &s);

    void inputDeviceAttached(const QMidiDeviceInfo info);
    void outputDeviceAttached(const QMidiDeviceInfo info);
    void inputDeviceDetached(const QMidiDeviceInfo info);
    void outputDeviceDetached(const QMidiDeviceInfo info);

    void inputDeviceCreated(const QMidiInput* device);
    void outputDeviceCreated(const QMidiOutput* device);
    void inputDeviceDeleted(QMidiDeviceInfo info);
    void outputDeviceDeleted(QMidiDeviceInfo info);

    void receiveMessage(const QMidiMessage &m);

};

#endif // MIDICOMMUNICATOR_H
