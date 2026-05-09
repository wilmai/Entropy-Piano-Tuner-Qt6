TARGET = QtMidi
TEMPLATE = lib
QT -= core gui
CONFIG += java

JAVACLASSPATH += \
    $$PWD/src \
    $$(QTDIR)/jar/Qt6Android.jar \
    $$files($$PWD/libs/*.jar)

JAVASOURCES += $$PWD/src/org/uniwue/tp3/midi/QAndroidMidiInterface.java
