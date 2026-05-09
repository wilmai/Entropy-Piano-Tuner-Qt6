include($$PWD/src/src.pri)

JAVACLASSPATH +=  $$shadowed($$PWD/jar/QtMidi-bundled_$${ANDROID_TARGET_ARCH}.jar)
JAVACLASSPATH += $$files($$PWD/../../../3rdparty/*.jar, false)
DISTFILES += $$files($$PWD/../../../3rdparty/*.aar, false)
