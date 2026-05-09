#include "qtmidi/internal/qmidipluginloader.h"

#include <QtCore/qdebug.h>

#include "qtmidi/qmidisystemplugin.h"

#if defined(QTMIDI_WINMM)
#include "qtmidi/plugins/winmm/qwinmmmidiplugin.h"
#elif defined(QTMIDI_ANDROID)
#include "qtmidi/plugins/android/src/qandroidmidiplugin.h"
#elif defined(QTMIDI_ALSA)
#include "qtmidi/plugins/alsa/qalsamidiplugin.h"
#endif

QT_BEGIN_NAMESPACE

QMidiPluginLoader& QMidiPluginLoader::instance() {
    static QMidiPluginLoader loader;
    return loader;
}

void QMidiPluginLoader::create(QObject* parent) {
    if (mPlugin) {
        qWarning() << "Deleting old Midi plugin. This is unexpected.";
        delete mPlugin;
    }
#if defined(QTMIDI_WINMM)
    mPlugin = new QWinMMMidiPlugin(parent);
#elif defined(QTMIDI_ANDROID)
    mPlugin = new QAndroidMidiPlugin(parent);
#elif defined(QTMIDI_ALSA)
    mPlugin = new QAlsaMidiPlugin(parent);
#else
#error "Midi not supported"
#endif
    qDebug() << "Midi plugin created.";
}

QMidiSystemPlugin* QMidiPluginLoader::getOrCreate(QObject*parent) {
    if (mPlugin == nullptr) {
        create(parent);
    }

    return mPlugin;
}

QT_END_NAMESPACE
