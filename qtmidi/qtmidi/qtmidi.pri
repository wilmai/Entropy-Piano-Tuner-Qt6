LIBS += -L$$shadowed($$PWD)/lib
LIBS += -L$$shadowed($$PWD)/../qtmidi_qml/lib

android:LIBS += -lqtmidi_$${ANDROID_TARGET_ARCH}
else:linux:LIBS += -lqtmidi -lasound
else:LIBS += -lqtmidi

INCLUDEPATH += $$PWD/plugins
INCLUDEPATH += $$PWD/..

win32:!winrt {
    LIBS += -lWinmm
}

winrt {
    CONFIG(debug, debug|release) {
        PLUGIN_DIR = $$OUT_PWD/debug
        PLUGIN_FILE=qtwinrt_midid.dll
    }
    else {
        PLUGIN_DIR = $$OUT_PWD/release
        PLUGIN_FILE=qtwinrt_midi.dll
    }

    PLUGIN_DIR ~= s,/,\\,g


    QMAKE_POST_LINK += \
        $$sprintf($$QMAKE_MKDIR_CMD, $$quote($$PLUGIN_DIR\\midi)) $$escape_expand(\\n\\t) \
        $$QMAKE_COPY $$quote($$(QTDIR)\\plugins\\midi\\$$PLUGIN_FILE) $$quote($$PLUGIN_DIR\\midi\\$$PLUGIN_FILE) $$escape_expand(\\n\\t)
}

android {
    isEmpty(ANDROID_PACKAGE_SOURCE_DIR):error("ANDROID_PACKAGE_SOURCE_DIR must be set.")

    OUT_LIBS = $$ANDROID_PACKAGE_SOURCE_DIR/libs
    message("Installing QtMidi dependencies to $$OUT_LIBS")

    # Copy aars to the android source dir so that they are included in the grade build
    JAVA_LIBS = $$files($$PWD/../qtmidi/plugins/android/jar/libs/*.aar)
    isEmpty(JAVA_LIBS):error("No dependencies found")
    for (JAVA_LIB, JAVA_LIBS) {
        contains(QMAKE_HOST.os, Windows) {
           JAVA_LIB ~= s,/,\\,g
           OUT_LIBS ~= s,/,\\,g
        }
        QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$JAVA_LIB) $$quote($$OUT_LIBS) $$escape_expand(\\n\\t)
    }
}
