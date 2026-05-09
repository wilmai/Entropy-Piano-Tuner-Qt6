#include "qandroidmidiinput.h"
#include "qandroidmidinativewrapper.h"
#include "qandroidmidiplugin.h"
#include <qdebug.h>

QT_BEGIN_NAMESPACE

QAndroidMidiInput::QAndroidMidiInput(const QMidiDeviceInfo &info, QJniObject usbDriver)
    : QAbstractMidiInput(info)
    , mUsbDriver(usbDriver)
{
    if (android_createInputDevice(QAndroidMidiPlugin::toDeviceName(handle()), mUsbDriver)) {
        mState = QMidi::ConnectedState;
    }

    // add us to connected devices
    if (QAndroidMidiPlugin::instance()) {
        QAndroidMidiPlugin::instance()->registerDevice(handle(), this);
    } else {
        qWarning() << "AndroidMidiPlugin does not exist";
    }
}

QAndroidMidiInput::~QAndroidMidiInput()
{
    // remove us from connected devices
    if (QAndroidMidiPlugin::instance()) {
        QAndroidMidiPlugin::instance()->removeDevice(this);
    }

    mState = QMidi::DisconnectedState;
    android_deleteInputDevice(QAndroidMidiPlugin::toDeviceName(handle()), mUsbDriver);
}

QT_END_NAMESPACE
