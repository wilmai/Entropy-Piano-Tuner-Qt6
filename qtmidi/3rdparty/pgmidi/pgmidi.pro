TARGET = qtpgmidi

CONFIG += \
    static \
    hide_symbols \
    installed

load(qt_helper_lib)

OBJECTIVE_SOURCES += \
    PGMidi.mm \
    PGMidiAllSources.mm \
    PGMidiFind.mm \

OBJECTIVE_HEADERS += \
    iOSVersionDetection.h \
    PGArc.h \
    PGMidi.h \
    PGMidiAllSources.h \
    PGMidiFind.h
