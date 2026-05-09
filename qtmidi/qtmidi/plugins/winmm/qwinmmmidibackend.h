#ifndef QWINMMMIDIBACKEND_H
#define QWINMMMIDIBACKEND_H

#include <QtCore/qglobal.h>
#include <QtCore/qlist.h>
#include <QtCore/qbytearray.h>

#include <qtmidi/qmidi.h>

#include <windows.h>
#include <mmsystem.h>

QT_BEGIN_NAMESPACE

class QWinMMMidiPlugin;

class QWinMMMidiBackend
{
public:
    QWinMMMidiBackend(QMidi::Mode mode, QWinMMMidiPlugin &midiPlugin);

    virtual unsigned int getPortCount() const = 0;
    virtual QString getPortName(unsigned int portNumber) const = 0;
    int getPortId(const QString &portName) const;
    int getPortId(const QByteArray &portHandle) const;

    QList<QByteArray> listAvailableDevices() const;

    QString handleToName(const QByteArray &handle) const;
    QByteArray nameToHandle(const QString &name) const;

    QMidi::Mode getMode() const {return mMode;}
    const QWinMMMidiPlugin &getMidiPlugin() const {return mMidiPlugin;}

protected:
    const QMidi::Mode mMode;
    QWinMMMidiPlugin &mMidiPlugin;
};

QT_END_NAMESPACE

#endif // QWINMMMIDIBACKEND_H
