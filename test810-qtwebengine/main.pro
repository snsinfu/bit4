TEMPLATE = app
TARGET = qtwebengine

# Disable all deprecated features before Qt 6.0.0.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
CONFIG += c++17

QT += qml quick webengine

SOURCES += main.cpp
RESOURCES += main.qrc
OTHER_FILES += main.qml
