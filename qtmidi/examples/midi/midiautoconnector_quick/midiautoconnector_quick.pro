TEMPLATE = app
QT += quick qml
CONFIG += c++11

include($$PWD/../../shared/app.pri)

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp

RESOURCES += \
    qml.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/midi/midiautoconnector_quick
INSTALLS += target
