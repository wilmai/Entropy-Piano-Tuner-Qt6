#ifndef QALSAMIDIDEVICEINFO_H
#define QALSAMIDIDEVICEINFO_H

#include <alsa/asoundlib.h>

#include <QtCore/qbytearray.h>
#include <QtCore/qlist.h>
#include <QtCore/qdebug.h>

#include <qtmidi/qmidi.h>
#include <qtmidi/qmidideviceinfo.h>
#include <qtmidi/qmidisystem.h>

QT_BEGIN_NAMESPACE

class QAlsaMidiDeviceInfo : public QAbstractMidiDeviceInfo
{
public:
    QAlsaMidiDeviceInfo(QString dev, QMidi::Mode mode);
    ~QAlsaMidiDeviceInfo();

    QString deviceName() const override;

private:
    QString device;
    QMidi::Mode mode;
};

QT_END_NAMESPACE

#endif // QALSAMIDIDEVICEINFO_H
