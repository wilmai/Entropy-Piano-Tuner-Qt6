#include "qmidisystem.h"
#include "qmidideviceinfo.h"

QT_BEGIN_NAMESPACE

class QAbstractMidiDevicePrivate {
public:
    QAbstractMidiDevicePrivate(const QMidiDeviceInfo& info)
        : deviceInfo(info)
    {}

    const QMidiDeviceInfo deviceInfo;
};

QAbstractMidiDevice::QAbstractMidiDevice(const QMidiDeviceInfo& deviceInfo)
    : d(new QAbstractMidiDevicePrivate(deviceInfo) ){
    
}

QAbstractMidiDevice::~QAbstractMidiDevice()
{
    delete d;
}

const QMidiDeviceInfo& QAbstractMidiDevice::deviceInfo() const
{
    return d->deviceInfo;
}

const QByteArray QAbstractMidiDevice::handle() const
{
    return d->deviceInfo.handle();
}


QAbstractMidiOutput::QAbstractMidiOutput(const QMidiDeviceInfo& deviceInfo)
    : QAbstractMidiDevice(deviceInfo)
{
}

void QAbstractMidiOutput::handleMidiMessage(const QMidiMessage &m) const {
    if (mForceTargetChannel) {
        QMidiMessage out(m);
        out.setChannel(mForcedChannel);
        sendMidiMessage(out);
    } else {
        sendMidiMessage(m);
    }
}

void QAbstractMidiOutput::setForceTargetChannel(bool force, quint8 channel) {
    mForceTargetChannel = force;
    mForcedChannel = channel;
}

QAbstractMidiInput::QAbstractMidiInput(const QMidiDeviceInfo &deviceInfo)
    : QAbstractMidiDevice(deviceInfo)
{
}

void QAbstractMidiInput::handleMidiMessage(const QMidiMessage &m) const {
    if (m.size() == 0 || m.byte0() == 0) {return;}
    if (mIgnoreSystemMessages && (m.byte0() & 0xF0) == 0xF0) {return;}
    emit notify(m);
}

void QAbstractMidiInput::setIgnoreSystemMessages(bool ignore) {
    mIgnoreSystemMessages = ignore;
}


// TODO: DOC

QT_END_NAMESPACE

#include "moc_qmidisystem.cpp"
