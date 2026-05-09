#include "qwinrtmidiwrapper.h"
#include "qwinrtmididevicewatcher.h"
#include "qwinrtnativehelper.h"
#include "qwinrtmidimessagecallback.h"
#include "qwinrtmididevicewatchercallback.h"
#include "qwinrtmidicliwrapper.h"
#include "qwinrtmidimessagefactory.h"
#include <qdebug.h>

using namespace Windows::UI::Popups;
QT_BEGIN_NAMESPACE

using namespace Windows::UI::Core;
using namespace Windows::Devices::Midi;

QWinRTMidiWrapper::QWinRTMidiWrapper(QWinRTMidiWrapperCallback *callback)
    : mMidiCLIWrapper(nullptr)
    , mCallback(callback)
{
    if (!mMidiCLIWrapper){
        mMidiCLIWrapper = new QWinRTMidiCLIWrapper(this);
    }
    mMidiCLIWrapper->GetMidiInDeviceWatcher()->Start();
    mMidiCLIWrapper->GetMidiOutDeviceWatcher()->Start();
}

QWinRTMidiWrapper::~QWinRTMidiWrapper() {
    QWinRTMidiWrapperCallbackLockGuard(this);
    mCallback = nullptr;

    if (mMidiCLIWrapper) {
        delete mMidiCLIWrapper;
        mMidiCLIWrapper = nullptr;
    }
}

bool QWinRTMidiWrapper::createInputDevice(const QString &id, QWinRTMidiMessageCallback ^callback) {
    // need to call the following on the CoreDispatcher
    create_task(mMidiCLIWrapper->GetDispatcher()->RunAsync(
        Windows::UI::Core::CoreDispatcherPriority::High,
        ref new Windows::UI::Core::DispatchedHandler([this, id, callback]() {
            QWinRTMidiDeviceConnection ^ connectionInfo = mMidiCLIWrapper->GetMidiInDeviceWatcher()
                                                              ->getPort(convert(id));
            if (nullptr == connectionInfo) {
                return;
            }

            DeviceInformation ^ devInfo = connectionInfo->Device;
            if (nullptr == devInfo) {
                return;
            }

            create_task(MidiInPort::FromIdAsync(devInfo->Id))
                .then([this, connectionInfo, callback](MidiInPort ^ inPort) {
                    connectionInfo->MidiIn = inPort;
                    if (nullptr != inPort) {
                        connectionInfo->Connection = inPort->MessageReceived
                            += ref new Windows::Foundation::TypedEventHandler < MidiInPort ^,
                        MidiMessageReceivedEventArgs ^
                            > (callback, &QWinRTMidiMessageCallback::onMessageReceived);
                    } else {
                        qWarning() << "Could not create input device";
                    }
                });
        })));

    return true;
}

bool QWinRTMidiWrapper::createOutputDevice(const QString &id) {
    // need to call the following on the CoreDispatcher
    create_task(mMidiCLIWrapper->GetDispatcher()
                    ->RunAsync(Windows::UI::Core::CoreDispatcherPriority::High,
                               ref new Windows::UI::Core::DispatchedHandler([this, id]() {
                                   QWinRTMidiDeviceConnection ^ connectionInfo
                                       = mMidiCLIWrapper->GetMidiOutDeviceWatcher()->getPort(
                                           convert(id));
                                   if (nullptr == connectionInfo) {
                                       return;
                                   }

                                   DeviceInformation ^ devInfo = connectionInfo->Device;
                                   if (nullptr == devInfo) {
                                       return;
                                   }

                                   create_task(MidiOutPort::FromIdAsync(devInfo->Id))
                                       .then([this, connectionInfo](IMidiOutPort ^ outPort) {
                                           if (outPort == nullptr) {
                                               qWarning() << "Could not create output device.";
                                           }
                                           connectionInfo->MidiOut = outPort;
                                       });
                               })));

    return true;
}

bool QWinRTMidiWrapper::deleteInputDevice(const QString &id) {
    QWinRTMidiDeviceConnection^ connectionInfo = mMidiCLIWrapper->GetMidiInDeviceWatcher()->getPort(convert(id));
    if (nullptr == connectionInfo) {
        return false;
    }
    connectionInfo->MidiIn = nullptr;

    return true;
}

bool QWinRTMidiWrapper::deleteOutputDevice(const QString &id) {
    QWinRTMidiDeviceConnection^ connectionInfo = mMidiCLIWrapper->GetMidiOutDeviceWatcher()->getPort(convert(id));
    if (nullptr == connectionInfo) {
        return false;
    }
    connectionInfo->MidiOut = nullptr;

    return true;
}

QVector<QString> QWinRTMidiWrapper::listAvailableInputDevices() const {
    QVector<QString> out;

    auto midiInDeviceWatcher = mMidiCLIWrapper->GetMidiInDeviceWatcher();
    for (unsigned int i = 0; i < midiInDeviceWatcher->portList()->Size; ++i) {
        QWinRTMidiDeviceConnection ^ dev = midiInDeviceWatcher->portList()->GetAt(i);
        if (dev->ToDelete == false) {
            out.push_back(convert(dev->Id));
        }
    }

    return out;
}

QVector<QString> QWinRTMidiWrapper::listAvailableOutputDevices() const {
    QVector<QString> out;

    auto midiOutDeviceWatcher = mMidiCLIWrapper->GetMidiOutDeviceWatcher();
    for (unsigned int i = 0; i < midiOutDeviceWatcher->portList()->Size; ++i) {
        QWinRTMidiDeviceConnection ^ dev = midiOutDeviceWatcher->portList()->GetAt(i);
        if (dev->ToDelete == false) {
            out.push_back(convert(dev->Id));
        }
    }

    return out;
}

QString QWinRTMidiWrapper::inputDeviceName(const QString& id) const {
    auto midiInDeviceWatcher = mMidiCLIWrapper->GetMidiInDeviceWatcher();
    for (unsigned int i = 0; i < midiInDeviceWatcher->portList()->Size; ++i) {
        QWinRTMidiDeviceConnection ^ dev = midiInDeviceWatcher->portList()->GetAt(i);
        if (dev->Id->Equals(convert(id))) {
            return convert(dev->Name);
        }
    }
    return QString();
}

QString QWinRTMidiWrapper::outputDeviceName(const QString& id) const {
    auto midiOutDeviceWatcher = mMidiCLIWrapper->GetMidiOutDeviceWatcher();
    for (unsigned int i = 0; i < midiOutDeviceWatcher->portList()->Size; ++i) {
        QWinRTMidiDeviceConnection ^ dev = midiOutDeviceWatcher->portList()->GetAt(i);
        if (dev->Id->Equals(convert(id))) {
            return convert(dev->Name);
        }
    }
    return QString();
}

void QWinRTMidiWrapper::sendMidiMessage(const QString& id, const QMidiMessage &m) const {
    IMidiMessage ^message = QWinRTMidiMessageFactory::create(m);
    if (message == nullptr) {return;}

    QWinRTMidiDeviceConnection^ connectionInfo = mMidiCLIWrapper->GetMidiOutDeviceWatcher()->getPort(convert(id));
    if (nullptr == connectionInfo) {return;}

    IMidiOutPort^ port = connectionInfo->MidiOut;
    if (nullptr == port) {return;}

    port->SendMessage(message);
}

void QWinRTMidiWrapper::lockCallbackMutex() {
    mCallbackMutex.lock();
}

void QWinRTMidiWrapper::unlockCallbackMutex() {
    mCallbackMutex.unlock();
}

QMutex& QWinRTMidiWrapper::callbackMutex() {
    return mCallbackMutex;
}

QWinRTMidiWrapperCallback* QWinRTMidiWrapper::getCallback() {
    return mCallback;
}

QT_END_NAMESPACE
