#include "qandroidmidinativewrapper.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QJniEnvironment>

#include "qandroidmidiplugin.h"

JNIEXPORT void java_midi_sendMidiMessageI(JNIEnv *env, jobject thiz, jstring deviceName, jint byte1) {
    Q_UNUSED(env);
    Q_UNUSED(thiz);
    QVector<quint8> bytes = {static_cast<quint8>(byte1)};
    QMidiMessage m(bytes, QDateTime::currentMSecsSinceEpoch());

    QString name = QJniObject(deviceName).toString();
#if !defined QT_NO_DEBUG
    if (qgetenv("QT_DEBUG_PLUGINS").toInt() > 0) {
        qDebug() << "java_sendMidiMissage from" << name << "with" << m << "raw" << byte1;
    }
#endif
    QAndroidMidiPlugin::fromNativeHandleInputMessage(name, m);
}

JNIEXPORT void java_midi_sendMidiMessageII(JNIEnv *env, jobject thiz, jstring deviceName, jint byte1, jint byte2) {
    Q_UNUSED(env);
    Q_UNUSED(thiz);
    QVector<quint8> bytes = {static_cast<quint8>(byte1), static_cast<quint8>(byte2)};
    QMidiMessage m(bytes, QDateTime::currentMSecsSinceEpoch());

    QString name = QJniObject(deviceName).toString();
#if !defined QT_NO_DEBUG
    if (qgetenv("QT_DEBUG_PLUGINS").toInt() > 0) {
        qDebug() << "java_sendMidiMissage from" << name << "with" << m << "raw" << byte1 << byte2;
    }
#endif
    QAndroidMidiPlugin::fromNativeHandleInputMessage(name, m);
}

JNIEXPORT void java_midi_sendMidiMessageIII(JNIEnv *env, jobject thiz, jstring deviceName, jint byte1, jint byte2, jint byte3) {
    Q_UNUSED(env);
    Q_UNUSED(thiz);
    QVector<quint8> bytes = {static_cast<quint8>(byte1), static_cast<quint8>(byte2), static_cast<quint8>(byte3)};
    QMidiMessage m(bytes, QDateTime::currentMSecsSinceEpoch());

    QString name = QJniObject(deviceName).toString();
#if !defined QT_NO_DEBUG
    if (qgetenv("QT_DEBUG_PLUGINS").toInt() > 0) {
        qDebug() << "java_sendMidiMissage from" << name << "with" << m << "raw" << byte1 << byte2 << byte3;
    }
#endif
    QAndroidMidiPlugin::fromNativeHandleInputMessage(name, m);
}

JNIEXPORT void java_midi_sendMidiMessageBytes(JNIEnv *env, jobject thiz, jstring deviceName, jbyteArray jbytes) {
    Q_UNUSED(env);
    Q_UNUSED(thiz);
    jboolean isCopy;
    jbyte* a = env->GetByteArrayElements(jbytes, &isCopy);
    int length = strlen((const char*)a);
    QVector<quint8> bytes(length);
    memcpy(bytes.data(), a, length);
    env->ReleaseByteArrayElements(jbytes, a, JNI_ABORT);

    QMidiMessage m(bytes, QDateTime::currentMSecsSinceEpoch());

    QString name = QJniObject(deviceName).toString();
#if !defined QT_NO_DEBUG
    if (qgetenv("QT_DEBUG_PLUGINS").toInt() > 0) {
        qDebug() << "java_sendMidiMissage from" << name << "with" << m;
    }
#endif
    QAndroidMidiPlugin::fromNativeHandleInputMessage(name, m);
}

JNIEXPORT void java_midi_inputDeviceAttached(JNIEnv *env, jobject thiz, jstring deviceName) {
    Q_UNUSED(env);
    Q_UNUSED(thiz);
#if !defined QT_NO_DEBUG
    if (qgetenv("QT_DEBUG_PLUGINS").toInt() > 0) {
        qDebug() << "java_inputDeviceAttached";
    }
#endif
    const char* raw = env->GetStringUTFChars(deviceName, 0);
    QAndroidMidiPlugin::deviceAttached(QString::fromUtf8(raw), QMidi::MidiInput);
    env->ReleaseStringUTFChars(deviceName, raw);
}

JNIEXPORT void java_midi_inputDeviceDetached(JNIEnv *env, jobject thiz, jstring deviceName) {
    Q_UNUSED(env);
    Q_UNUSED(thiz);
#if !defined QT_NO_DEBUG
    if (qgetenv("QT_DEBUG_PLUGINS").toInt() > 0) {
        qDebug() << "java_inputDeviceDetached";
    }
#endif
    const char* raw = env->GetStringUTFChars(deviceName, 0);
    QAndroidMidiPlugin::deviceDetached(QString::fromUtf8(raw), QMidi::MidiInput);
    env->ReleaseStringUTFChars(deviceName, raw);
}

JNIEXPORT void java_midi_outputDeviceAttached(JNIEnv *env, jobject thiz, jstring deviceName) {
    Q_UNUSED(env);
    Q_UNUSED(thiz);
#if !defined QT_NO_DEBUG
    if (qgetenv("QT_DEBUG_PLUGINS").toInt() > 0) {
        qDebug() << "java_outputDeviceAttached";
    }
#endif
    const char* raw = env->GetStringUTFChars(deviceName, 0);
    QAndroidMidiPlugin::deviceAttached(QString::fromUtf8(raw), QMidi::MidiOutput);
    env->ReleaseStringUTFChars(deviceName, raw);
}

JNIEXPORT void java_midi_outputDeviceDetached(JNIEnv *env, jobject thiz, jstring deviceName) {
    Q_UNUSED(env);
    Q_UNUSED(thiz);
#if !defined QT_NO_DEBUG
    if (qgetenv("QT_DEBUG_PLUGINS").toInt() > 0) {
        qDebug() << "java_midi_outputDeviceDetached";
    }
#endif
    const char* raw = env->GetStringUTFChars(deviceName, 0);
    QAndroidMidiPlugin::deviceDetached(QString::fromUtf8(raw), QMidi::MidiOutput);
    env->ReleaseStringUTFChars(deviceName, raw);
}

QT_BEGIN_NAMESPACE

static JNINativeMethod jniMidiNativeMethods[] = {
    {"java_midi_sendMidiMessage", "(Ljava/lang/String;I)V", (void *) java_midi_sendMidiMessageI},
    {"java_midi_sendMidiMessage", "(Ljava/lang/String;II)V", (void *) java_midi_sendMidiMessageII},
    {"java_midi_sendMidiMessage", "(Ljava/lang/String;III)V", (void *) java_midi_sendMidiMessageIII},
    {"java_midi_sendMidiMessage",
     "(Ljava/lang/String;[B)V",
     (void *) java_midi_sendMidiMessageBytes},
    {"java_midi_inputDeviceAttached",
     "(Ljava/lang/String;)V",
     (void *) java_midi_inputDeviceAttached},
    {"java_midi_inputDeviceDetached",
     "(Ljava/lang/String;)V",
     (void *) java_midi_inputDeviceDetached},
    {"java_midi_outputDeviceAttached",
     "(Ljava/lang/String;)V",
     (void *) java_midi_outputDeviceAttached},
    {"java_midi_outputDeviceDetached",
     "(Ljava/lang/String;)V",
     (void *) java_midi_outputDeviceDetached},
};

#define QT_ANDROID_MIDI_CLASS "org/uniwue/tp3/midi/QAndroidMidiInterface"

JNIEXPORT void JNICALL initAndroidManagerJNI(QJniObject *usbmanager) {
#if !defined QT_NO_DEBUG
    bool doDebug = qgetenv("QT_DEBUG_PLUGINS").toInt() > 0;

    if (doDebug) {
        qDebug() << "initAndroidManagerJNI: Attaching";
        qDebug() << "initAndroidManagerJNI: CallStaticObjectMethod";
    }
#endif

    *usbmanager = QJniObject::callStaticObjectMethod(
        QT_ANDROID_MIDI_CLASS,
        "create",
        "(Landroid/app/Activity;)L" QT_ANDROID_MIDI_CLASS ";",
        QNativeInterface::QAndroidApplication::context().object());

#if !defined QT_NO_DEBUG
    if (doDebug) {qDebug() << "initAndroidManagerJNI: NewGlobalRef";}
#endif

    // calling init
    usbmanager->callMethod<void>("init", "()V");

#if !defined QT_NO_DEBUG
    if (doDebug) {qDebug() << "initAndroidManagerJNI: initialized";}
#endif
}

JNIEXPORT void JNICALL releaseAndroidManagerJNI(QJniObject usbmanager) {
    usbmanager.callMethod<void>("exit", "()V");
#if !defined QT_NO_DEBUG
    if (qgetenv("QT_DEBUG_PLUGINS").toInt() > 0) {
        qDebug() << "exitAndroidManagerJNI";
    }
#endif
}

QStringList android_listAvailableInputDevices(QJniObject usbmanager) {
    QStringList res;

    QJniObject names = usbmanager.callObjectMethod("getInputDeviceNames", "()Ljava/lang/String;");
    res = names.toString().split("\n");

    return res;
}

QStringList android_listAvailableOutputDevices(QJniObject usbmanager) {
    QStringList res;

    QJniObject names = usbmanager.callObjectMethod("getOutputDeviceNames", "()Ljava/lang/String;");
    res = names.toString().split("\n");

    return res;
}

bool android_createInputDevice(const QString &id, QJniObject usbmanager) {
    return usbmanager.callMethod<jboolean>("connectInputDevice", "(Ljava/lang/String;)Z", QJniObject::fromString(id).object<jstring>());
}

bool android_createOutputDevice(const QString &id, QJniObject usbmanager) {
    return usbmanager.callMethod<jboolean>("connectOutputDevice", "(Ljava/lang/String;)Z", QJniObject::fromString(id).object<jstring>());
}

bool android_deleteInputDevice(const QString &id, QJniObject usbmanager) {
    return usbmanager.callMethod<jboolean>("disconnectInputDevice", "(Ljava/lang/String;)Z", QJniObject::fromString(id).object<jstring>());
}

bool android_deleteOutputDevice(const QString &id, QJniObject usbmanager) {
    return usbmanager.callMethod<jboolean>("disconnectOutputDevice", "(Ljava/lang/String;)Z", QJniObject::fromString(id).object<jstring>());
}

void android_sendNativeMessage(const QString &id, QJniObject usbmanager, const QMidiMessage &m) {
#if !defined QT_NO_DEBUG
    if (qgetenv("QT_DEBUG_PLUGINS").toInt() > 0) {
        qDebug() << "android_receiveMidiOutputEvent to" << id << "with command:"  << m;
    }
#endif

    usbmanager.callMethod<void>("receiveMidiOutputEvent",
                                "(Ljava/lang/String;III)V",
                                QJniObject::fromString(id).object<jstring>(),
                                static_cast<jint>(m.byte0()),
                                static_cast<jint>(m.byte1()),
                                static_cast<jint>(m.byte2()));
}


bool android_registerNatives() {
    QJniEnvironment env;

    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
    }

    qDebug() << "QAndroidMidi: RegisterNatives";

    // search for UsbMidiDriver class
    jclass midi_class = env.findClass(QT_ANDROID_MIDI_CLASS);

    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
    }

    // register our native methods
    if (midi_class && env->RegisterNatives(midi_class, jniMidiNativeMethods, sizeof(jniMidiNativeMethods) / sizeof(jniMidiNativeMethods[0])) < 0)
    {
        qCritical() << "QAndroidMidi: Failed to register native methods.";
        return false;
    }

    qDebug() << "QAndroidMidi: Connected to JNI";

    return true;
}

QT_END_NAMESPACE
