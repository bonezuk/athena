TEMPLATE = app

QT += quick qml

TARGET = BlackOmegaIOS

HEADERS  = ./playerapp/playerios/inc/PlayerIOSBaseModel.h

SOURCES  = ./playerapp/playerios/src/PlayerIOSBaseModel.cxx
SOURCES += ./playerapp/playerios/src/PlayerIOSMain.cxx

RESOURCES = ./playerapp/playerios/playerios.qrc
