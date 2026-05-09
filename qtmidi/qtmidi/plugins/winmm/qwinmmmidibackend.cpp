#include "qwinmmmidibackend.h"
#include <QtCore/qdebug.h>

QT_BEGIN_NAMESPACE

QWinMMMidiBackend::QWinMMMidiBackend(QMidi::Mode mode, QWinMMMidiPlugin &midiPlugin)
    : mMode(mode)
    , mMidiPlugin(midiPlugin)
{
    
}

QList<QByteArray> QWinMMMidiBackend::listAvailableDevices() const
{
    QList<QByteArray> out;

    for (unsigned int i = 0; i < getPortCount(); ++i) {
        out.append(nameToHandle(getPortName(i)));
    }

    return out;
}

QString QWinMMMidiBackend::handleToName(const QByteArray &handle) const {
    QString name = QString::fromUtf8(handle);
    return name.left(name.size() - 4);
}

QByteArray QWinMMMidiBackend::nameToHandle(const QString &name) const {
    return QString("%1 [%2]").arg(name, QString::number(mMode)).toUtf8();
}

int QWinMMMidiBackend::getPortId(const QString &portName) const {
    for (unsigned int i = 0; i < getPortCount(); ++i) {
        if (getPortName(i) == portName) {
            return i;
        }
    }

    qWarning() << "Port name id for port" << portName << "not found.";
    return -1;
}

int QWinMMMidiBackend::getPortId(const QByteArray &portHandle) const {
    return getPortId(handleToName(portHandle));
}

QT_END_NAMESPACE
