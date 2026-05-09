#include "qwinrtmidiplugin.h"
#include "QtMidi/qmidideviceinfo.h"

#include "qwinrtmidiinput.h"
#include "qwinrtmidioutput.h"

QT_BEGIN_NAMESPACE

QWinRTMidiPlugin::QWinRTMidiPlugin(QObject* parent)
    : QMidiSystemPlugin(parent)
    , mWrapper(this)
{}

QWinRTMidiPlugin::~QWinRTMidiPlugin()
{}

QList<QByteArray> QWinRTMidiPlugin::availableDevices(QMidi::Mode mode) const {
    auto toByteArrayList = [](const QVector<QString> &v){
        QList<QByteArray> list;
        for (auto s : v) {
            list << s.toUtf8();
        }
        
        return list;
    };

    if (mode == QMidi::MidiInput) {
        return toByteArrayList(mWrapper.listAvailableInputDevices());
    } else {
        return toByteArrayList(mWrapper.listAvailableOutputDevices());
    }
}

QAbstractMidiInput* QWinRTMidiPlugin::createInput(const QMidiDeviceInfo& info) {
    return new QWinRTMidiInput(info, mWrapper);
}

QAbstractMidiOutput* QWinRTMidiPlugin::createOutput(const QMidiDeviceInfo& info) {
    return new QWinRTMidiOutput(info, mWrapper);
}

QString QWinRTMidiPlugin::deviceName(const QByteArray& device, QMidi::Mode mode) {
    if (mode == QMidi::MidiInput) {
        return mWrapper.inputDeviceName(QString::fromUtf8(device));
    } else {
        return mWrapper.outputDeviceName(QString::fromUtf8(device));
    }
}

void QWinRTMidiPlugin::inputDeviceAttached(const QString &id, const QString &humanReadable) {
    Q_UNUSED(humanReadable);
    notifyInputDeviceAttached(deviceInfo("winrt", id.toUtf8(), QMidi::MidiInput));
}

void QWinRTMidiPlugin::outputDeviceAttached(const QString &id, const QString &humanReadable) {
    Q_UNUSED(humanReadable);
    notifyOutputDeviceAttached(deviceInfo("winrt", id.toUtf8(), QMidi::MidiOutput));
}

void QWinRTMidiPlugin::inputDeviceDetached(const QString &id, const QString &humanReadable) {
    Q_UNUSED(humanReadable);
    notifyInputDeviceDetached(deviceInfo("winrt", id.toUtf8(), QMidi::MidiInput));
}

void QWinRTMidiPlugin::outputDeviceDetached(const QString &id, const QString &humanReadable) {
    Q_UNUSED(humanReadable);
    notifyOutputDeviceDetached(deviceInfo("winrt", id.toUtf8(), QMidi::MidiOutput));
}

QT_END_NAMESPACE
