#ifndef QMIDIPLUGINLOADER_H
#define QMIDIPLUGINLOADER_H

#include <QtCore/qobject.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qmap.h>
#include <QtCore/qjsonobject.h>

#include "qtmidi/internal/qtmididefs.h"

QT_BEGIN_NAMESPACE

class QFactoryLoader;
class QMidiServiceProviderPlugin;
class QMidiSystemPlugin;

using QMidiSystemPluginCreator = std::function<QMidiSystemPlugin*(QObject*)>;

class Q_MIDI_EXPORT QMidiPluginLoader
{
public:
    static QMidiPluginLoader& instance();

    QMidiSystemPlugin* getOrCreate(QObject*parent = nullptr);

private:
    void create(QObject* parent);

    QMidiSystemPlugin *mPlugin = nullptr;
};

QT_END_NAMESPACE

#endif // QMIDIPLUGINLOADER_H
