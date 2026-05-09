#ifndef QWINRTMIDIWRAPPER_H
#define QWINRTMIDIWRAPPER_H

#include <QtCore/qstring.h>
#include <QtCore/qmutex.h>
#include <QtCore/qvector.h>

#include <qtmidi/qmidimessage.h>

QT_BEGIN_NAMESPACE

class QWinRTMidiCLIWrapper;
class QWinRTMidiInput;
ref class QWinRTMidiMessageCallback;

class QWinRTMidiWrapperCallback {
public:
    virtual void inputDeviceAttached(const QString &id, const QString &humanReadable) = 0;
    virtual void outputDeviceAttached(const QString &id, const QString &humanReadable) = 0;
    virtual void inputDeviceDetached(const QString &id, const QString &humanReadable) = 0;
    virtual void outputDeviceDetached(const QString &id, const QString &humanReadable) = 0;
};

class QWinRTMidiWrapper {
public:
    QWinRTMidiWrapper(QWinRTMidiWrapperCallback* callback);
    ~QWinRTMidiWrapper();

    bool createInputDevice(const QString &id, QWinRTMidiMessageCallback ^ callback);
    bool createOutputDevice(const QString &id);
    bool deleteInputDevice(const QString &id);
    bool deleteOutputDevice(const QString &id);

    QVector<QString> listAvailableInputDevices() const;
    QVector<QString> listAvailableOutputDevices() const;

    QString inputDeviceName(const QString& id) const;
    QString outputDeviceName(const QString& id) const;

    void sendMidiMessage(const QString &id, const QMidiMessage &m) const;

    QWinRTMidiWrapperCallback* getCallback();
    void lockCallbackMutex();
    void unlockCallbackMutex();
    QMutex &callbackMutex();

private:
    mutable QMutex mCallbackMutex;

    QWinRTMidiCLIWrapper* mMidiCLIWrapper;
    QWinRTMidiWrapperCallback* mCallback = nullptr;
};

#define QWinRTMidiWrapperCallbackLockGuard(wrapper) \
    QMutexLocker _lockguard(&wrapper->callbackMutex()); \
    (void) _lockguard

QT_END_NAMESPACE

#endif // QWINRTMIDIWRAPPER_H
