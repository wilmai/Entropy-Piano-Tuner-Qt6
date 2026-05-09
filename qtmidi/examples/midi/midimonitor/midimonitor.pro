TEMPLATE = app
QT += widgets
TARGET = midimonitor

include($$PWD/../../shared/app.pri)

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

target.path = $$[QT_INSTALL_EXAMPLES]/midi/midimonitor
INSTALLS += target
