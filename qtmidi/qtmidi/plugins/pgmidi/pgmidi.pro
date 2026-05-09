TARGET = qtpg_midi

QT += midi-private

LIBS += -L$$shadowed($$PWD/../../../lib) -lqtpgmidi -framework Foundation -framework CoreMIDI

HEADERS += \
    qpgmidiplugin.h \
    qpgmidimonitor.h \
    qpgmidiwrapper.h \
    qpgmidiinput.h \
    qpgmidioutput.h

SOURCES += \
    qpgmidiplugin.cpp

OBJECTIVE_SOURCES += \
    qpgmidimonitor.mm \
    qpgmidiwrapper.mm \
    qpgmidiinput.mm \
    qpgmidioutput.mm

OTHER_FILES += \
    pgmidi.json

PLUGIN_TYPE = midi
PLUGIN_CLASS_NAME = QPGMidiPlugin
load(qt_plugin)
