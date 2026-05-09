#include "qandroidmidioutput.h"
#include "qandroidmidinativewrapper.h"
#include "qandroidmidiplugin.h"

QT_BEGIN_NAMESPACE

QAndroidMidiOutput::QAndroidMidiOutput(const QMidiDeviceInfo &info, QJniObject usbDriver)
    : QAbstractMidiOutput(info)
    , mUsbDriver(usbDriver)
{
    android_createOutputDevice(QAndroidMidiPlugin::toDeviceName(handle()), usbDriver);
}

QAndroidMidiOutput::~QAndroidMidiOutput()
{
    android_deleteOutputDevice(deviceInfo().deviceName(), mUsbDriver);
}

void QAndroidMidiOutput::sendMidiMessage(const QMidiMessage& m) const
{
    android_sendNativeMessage(QAndroidMidiPlugin::toDeviceName(handle()), mUsbDriver, m);
}

QT_END_NAMESPACE
