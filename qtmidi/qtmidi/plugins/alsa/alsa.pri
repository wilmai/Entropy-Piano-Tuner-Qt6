INCLUDEPATH += $$PWD

HEADERS += \
    plugins/alsa/qalsamidiplugin.h \
    plugins/alsa/qalsamidibackend.h \
    plugins/alsa/qalsamidideviceinfo.h \
    plugins/alsa/qalsamidiinbackend.h \
    plugins/alsa/qalsamidiinput.h \
    plugins/alsa/qalsamidioutbackend.h \
    plugins/alsa/qalsamidioutput.h \
    plugins/alsa/qalsamidishared.h

SOURCES += \
    plugins/alsa/qalsamidiplugin.cpp \
    plugins/alsa/qalsamidibackend.cpp \
    plugins/alsa/qalsamidideviceinfo.cpp \
    plugins/alsa/qalsamidiinbackend.cpp \
    plugins/alsa/qalsamidiinput.cpp \
    plugins/alsa/qalsamidioutbackend.cpp \
    plugins/alsa/qalsamidioutput.cpp \
    plugins/alsa/qalsamidishared.cpp
