#include "qtmidi/internal/qmididevicewatcher.h"

#include "qtmidi/internal/qmidisystemnotifierprivate.h"
#include "qtmidi/internal/qmididevicefactory.h"
#include "qtmidi/qmidideviceinfo.h"

#include <QTimer>

QT_BEGIN_NAMESPACE

QMidiDeviceWatcher::QMidiDeviceWatcher(QMidi::Mode mode, int msec, QObject *parent)
    : QObject(parent)
    , mMode(mode)
{
    auto timer = new QTimer(this);
    timer->setSingleShot(false);
    connect(timer, &QTimer::timeout, this, &QMidiDeviceWatcher::checkNow);
    timer->start(msec);
}

QMidiDeviceWatcher::~QMidiDeviceWatcher()
{
}

void QMidiDeviceWatcher::checkNow() {
    QList<QMidiDeviceInfo> devInfos(QMidiDeviceFactory::availableDevices(mMode));

    for (auto it = mLastDeviceInformations.begin(); it != mLastDeviceInformations.end(); ) {
        if (devInfos.contains(*it)) {
            ++it;
            continue;
        }

        // midi device removed
        QMidiSystemNotifierPrivate::instance()->sendDetached(*it, mMode);
        it = mLastDeviceInformations.erase(it);
    }

    for (const QMidiDeviceInfo &newDev : devInfos) {
        if (mLastDeviceInformations.contains(newDev)) {continue;}

        // new midi device found
        QMidiSystemNotifierPrivate::instance()->sendAttached(newDev, mMode);
        mLastDeviceInformations.append(newDev);
    }
}

QT_END_NAMESPACE
