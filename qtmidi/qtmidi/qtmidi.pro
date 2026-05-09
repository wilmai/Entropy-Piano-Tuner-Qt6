TEMPLATE = lib
TARGET = qtmidi
DESTDIR = $$shadowed($$PWD)/lib
CONFIG += optimize_full

qtmidiDLL {
    DEFINES += QTMIDI_DLL
    DEFINES += QTMIDI_MAKEDLL
} else {
    CONFIG += staticlib
}

INCLUDEPATH += $$PWD/..

HEADERS += \
    qmidi.h \
    qmidiautoconnector.h \
    qmididevice.h \
    qmidideviceinfo.h \
    qmidiinput.h \
    qmidimessage.h \
    qmidioutput.h \
    qmidisystem.h \
    qmidisystemnotifier.h \
    qmidisystemplugin.h

SOURCES += \
    qmidi.cpp \
    qmidiautoconnector.cpp \
    qmididevice.cpp \
    qmidideviceinfo.cpp \
    qmidiinput.cpp \
    qmidimessage.cpp \
    qmidioutput.cpp \
    qmidisystem.cpp \
    qmidisystemnotifier.cpp \
    qmidisystemplugin.cpp

#load(configure)
#qtCompileTest(openal)
#win32 {
#} else:mac {
#} else:qnx {
#} else:!android {
#    contains(QT_CONFIG, alsa):qtCompileTest(alsa)
#    contains(QT_CONFIG, pulseaudio):qtCompileTest(pulseaudio)
#}

include($$PWD/internal/internal.pri)

win32:!winrt {
    include($$PWD/plugins/winmm/winmm.pri)
    DEFINES += QTMIDI_WINMM
}

android {
    include($$PWD/plugins/android/android.pri)
    DEFINES += QTMIDI_ANDROID
}

linux:!android {
    include($$PWD/plugins/alsa/alsa.pri)
    DEFINES += QTMIDI_ALSA
}

