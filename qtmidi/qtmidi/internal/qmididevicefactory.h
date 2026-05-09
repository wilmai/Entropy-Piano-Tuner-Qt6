#ifndef QMIDIDEVICEFACTORY_H
#define QMIDIDEVICEFACTORY_H

#include <QtCore/qbytearray.h>
#include <QtCore/qlist.h>

#include "qtmidi/internal/qtmididefs.h"
#include "qtmidi/qmidi.h"
#include "qtmidi/qmidideviceinfo.h"
#include "qtmidi/internal/qmidisystemnotifierprivate.h"
#include "qtmidi/internal/qmididevicewatcher.h"

QT_BEGIN_NAMESPACE

class QAbstractMidiInput;
class QAbstractMidiOutput;
class QAbstractMidiDeviceInfo;
class QMidiInput;
class QMidiOutput;

class QMidiDeviceFactory
{
public:
    static void instantiate();

    static QMidiDeviceInfo createDeviceInfo(const QString& realm, const QByteArray& handle, QMidi::Mode mode);
    static QList<QByteArray> availableDeviceHandles(const QString& realm, QMidi::Mode mode);
    static QList<QMidiDeviceInfo> availableDevices(QMidi::Mode mode);

    static QMidiDeviceInfo defaultInputDevice();
    static QMidiDeviceInfo defaultOutputDevice();

    static QString midiDeviceName(const QString& realm, const QByteArray& handle, QMidi::Mode mode);

    static QAbstractMidiInput* createDefaultInputDevice();
    static QAbstractMidiOutput* createDefaultOutputDevice();

    static QAbstractMidiInput* createInputDevice(QMidiDeviceInfo const &device);
    static QAbstractMidiOutput* createOutputDevice(QMidiDeviceInfo const &device);

    static QAbstractMidiInput* createNullInput();
    static QAbstractMidiOutput* createNullOutput();
};

QT_END_NAMESPACE

#endif // QMIDIDEVICEFACTORY_H
