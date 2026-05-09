TARGET = qtaudio_midi
QT += midi-private

HEADERS += \
    qrtmidiplugin.h

SOURCES += \
    qrtmidiplugin.cpp

OTHER_FILES += \
    rtmidi.json

PLUGIN_TYPE = midi
PLUGIN_CLASS_NAME = QRtMidiPlugin
load(qt_plugin)
