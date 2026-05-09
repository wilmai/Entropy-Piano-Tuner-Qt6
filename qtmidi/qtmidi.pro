TEMPLATE = subdirs
SUBDIRS += qtmidi
CONFIG += examples

contains(CONFIG, examples) {
    SUBDIRS += examples
    examples.depends = qtmidi
}


# Additional targets for android (jar dependencies)
android {
    qtmidi_plugins_android.subdir = qtmidi/plugins/android/jar
    qtmidi_plugins_android.target = plugins-android-jar

    #3rdparty_android-midi-driver.subdir = 3rdparty/android-midi-driver
    #3rdparty_android-midi-driver.target = android-midi-driver

    qtmidi.depends += qtmidi_plugins_android
    #qt_midi_plugins_android.depends += 3rdparty_android-midi-driver

    #SUBDIRS += 3rdparty_android-midi-driver qtmidi_plugins_android
    SUBDIRS += qtmidi_plugins_android
}
