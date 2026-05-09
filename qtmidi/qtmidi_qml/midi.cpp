#include <QtQml/qqmlextensionplugin.h>
#include <QtQml/qqml.h>
#include <QtQml/qqmlengine.h>
#include <QtQml/qqmlcomponent.h>

#include "qdeclarativemidiglobal_p.h"
#include "qdeclarativemidiinputdevice_p.h"
#include "qdeclarativemidioutputdevice_p.h"
#include "qdeclarativemidimessagecreator.h"

static void initResources()
{
    Q_INIT_RESOURCE(qmake_QtMidi);
}

QT_BEGIN_NAMESPACE

static QObject *midi_global_object(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
    Q_UNUSED(qmlEngine)
    return new QDeclarativeMidiGlobal(jsEngine);
}

static QObject *midi_message_creator_provider(QQmlEngine *engine, QJSEngine *jsEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(jsEngine)
    return new QDeclarativeMidiMessageCreator();
}

class QMidiDeclarativeModule : public QQmlEngineExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    QMidiDeclarativeModule(QObject *parent = 0) : QQmlEngineExtensionPlugin(parent) { initResources(); }

    void initializeEngine(QQmlEngine *engine, const char *uri) override
    {
        Q_UNUSED(engine);

        Q_ASSERT(QLatin1String(uri) == QLatin1String("QtMidi"));

        qmlRegisterSingletonType<QDeclarativeMidiGlobal>(uri, 5, 0, "QtMidi", midi_global_object);
        qmlRegisterType<QDeclarativeMidiInputDevice>(uri, 5, 0, "MidiInput");
        qmlRegisterType<QDeclarativeMidiOutputDevice>(uri, 5, 0, "MidiOutput");
        qmlRegisterSingletonType<QDeclarativeMidiMessageCreator>(uri, 5, 0, "MidiMessageCreator", midi_message_creator_provider);
    }
};

QT_END_NAMESPACE

#include "midi.moc"
