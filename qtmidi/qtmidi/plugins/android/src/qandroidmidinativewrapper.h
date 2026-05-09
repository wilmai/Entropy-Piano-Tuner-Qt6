#ifndef QANDROIDMIDINATIVEWRAPPER_H
#define QANDROIDMIDINATIVEWRAPPER_H

#include <qtmidi/qmidi.h>
#include <qtmidi/qmidisystem.h>
#include <QVector>
#include <QStringList>
#include <QJniObject>

QT_BEGIN_NAMESPACE

bool android_registerNatives();

void initAndroidManagerJNI(QJniObject *usbmanager);
void releaseAndroidManagerJNI(QJniObject usbmanager);

QStringList android_listAvailableInputDevices(QJniObject usbmanager);
QStringList android_listAvailableOutputDevices(QJniObject usbmanager);

bool android_createInputDevice(const QString &id, QJniObject usbmanager);
bool android_createOutputDevice(const QString &id, QJniObject usbmanager);
bool android_deleteInputDevice(const QString &id, QJniObject usbmanager);
bool android_deleteOutputDevice(const QString &id, QJniObject usbmanager);
void android_sendNativeMessage(const QString &id, QJniObject usbmanager, const QMidiMessage &m);


QT_END_NAMESPACE

#endif // QANDROIDMIDINATIVEWRAPPER_H
