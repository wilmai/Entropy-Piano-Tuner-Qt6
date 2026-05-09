#include "qalsamidideviceinfo.h"

QT_BEGIN_NAMESPACE

QAlsaMidiDeviceInfo::QAlsaMidiDeviceInfo(QString dev, QMidi::Mode mode)
{
    device = dev;
    this->mode = mode;
}

QAlsaMidiDeviceInfo::~QAlsaMidiDeviceInfo()
{
    
}

QString QAlsaMidiDeviceInfo::deviceName() const
{
    return device;
}

QT_END_NAMESPACE
