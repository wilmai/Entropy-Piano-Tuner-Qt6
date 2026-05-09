TARGET = qtwinrt_midi

QT += midi-private

# MSVC 2017 requires VCIDEINSTALLDIR for vcpackages
# else use the default VCINSTALLDIR
VCIDEINSTALLDIR=$$(VCIDEINSTALLDIR)
!isEmpty(VCIDEINSTALLDIR) { VCINSTALLDIR=$$VCIDEINSTALLDIR }
else { VCINSTALLDIR=$$(VCINSTALLDIR) }

# mark as win rt code (ZW flag)
WINRTPACKAGES = \"$${VCINSTALLDIR}vcpackages\"
QMAKE_CXXFLAGS += -ZW /AI $$WINRTPACKAGES /EHsc

HEADERS += \
    qwinrtmidiplugin.h \
    qwinrtmididevicewatcher.h \
    qwinrtmididevicewatchercallback.h \
    qwinrtmidimessagecallback.h \
    qwinrtmidicliwrapper.h \
    qwinrtmidiwrapper.h \
    qwinrtnativehelper.h \
    qwinrtmidiinput.h \
    qwinrtmidioutput.h \
    qwinrtmidimessagefactory.h

SOURCES += \
    qwinrtmidiplugin.cpp \
    qwinrtmididevicewatcher.cpp \
    qwinrtmididevicewatchercallback.cpp \
    qwinrtmidimessagecallback.cpp \
    qwinrtmidicliwrapper.cpp \
    qwinrtmidiwrapper.cpp \
    qwinrtnativehelper.cpp \
    qwinrtmidiinput.cpp \
    qwinrtmidioutput.cpp \
    qwinrtmidimessagefactory.cpp

OTHER_FILES += \
   winrt.json

PLUGIN_TYPE = midi
PLUGIN_CLASS_NAME = QWinRTPlugin
load(qt_plugin)
