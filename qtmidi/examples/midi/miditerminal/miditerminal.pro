CONFIG += console
QT += core

include($$PWD/../../shared/app.pri)

SOURCES += \
    main.cpp \
    midicommunicator.cpp

HEADERS += \
    midicommunicator.h

target.path = $$[QT_INSTALL_EXAMPLES]/midi/miditerminal
INSTALLS += target
