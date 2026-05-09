TEMPLATE = lib
TARGET = qtmidi_qml
CONFIG += staticlib
DESTDIR = $$shadowed($$PWD)/lib

include($$PWD/../qtmidi/qtmidi.pri)

QT += qml quick

HEADERS += \
    qdeclarativemidiglobal_p.h \
    qdeclarativemididevice_p.h \
    qdeclarativemidiinputdevice_p.h \
    qdeclarativemidioutputdevice_p.h \
    qdeclarativemidimessagecreator.h

SOURCES += \
    midi.cpp \
    qdeclarativemidiglobal.cpp \
    qdeclarativemididevice.cpp \
    qdeclarativemidiinputdevice.cpp \
    qdeclarativemidioutputdevice.cpp \
    qdeclarativemidimessagecreator.cpp

OTHER_FILES += \
    plugins.qmltypes

QML_FILES += \
    MidiInputComboBox.qml \
    MidiOutputComboBox.qml \
    MidiPowerBar.qml
