HEADERS += \
    internal/qmididevicefactory.h \
    internal/qmididevicewatcher.h \
    internal/qmidipluginloader.h \
    internal/qmidisystemnotifierprivate.h \
    internal/qmidiautoconnectorprivate.h
    internal/qtmididefs.h

SOURCES += \
    internal/qmididevicefactory.cpp \
    internal/qmididevicewatcher.cpp \
    internal/qmidipluginloader.cpp \
    internal/qmidisystemnotifierprivate.cpp \
    internal/qmidiautoconnectorprivate.cpp

ANDROID_BUNDLED_JAR_DEPENDENCIES = \
    jar/QtMidi-bundled.jar:org.qtproject.qt5.android.midi.QAndroidMidiInterface \
    jar/QtAndroidMidiDriver-bundled.jar

ANDROID_JAR_DEPENDENCIES = \
    jar/QtMidi.jar:org.qtproject.qt5.android.midi.QAndroidMidiInterface \
    jar/QtAndroidMidiDriver.jar

ANDROID_PERMISSIONS +=

ANDROID_FEATURES += \
    android.hardware.usb.host

MODULE_WINRT_CAPABILITIES_DEVICE += 
    usb
