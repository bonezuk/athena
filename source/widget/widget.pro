message( "Parsing widget.pro" )

include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_dll.pri )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/libxml2.pri )
include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/engine/engine.pri )
include ( $(OMEGA_HOME)/network/network.pri )
include ( $(OMEGA_HOME)/network/http/http.pri )
include ( $(OMEGA_HOME)/network/mime/mime.pri )
include ( $(OMEGA_HOME)/track/db/db.pri )

message( "Processing library widget" )

TARGET = widget

DEPENDPATH  += ../
INCLUDEPATH += ../

UI_DIR  = $(OMEGA_HOME)/widget

FORMS   = inc/LicenseRegisterDialog.ui
FORMS  += inc/AboutDialog.ui

RESOURCES = ./widget.qrc

HEADERS  = inc/LicenseRegisterDialog.h
HEADERS += inc/SandBoxBase.h
HEADERS += inc/WidgetDLL.h

SOURCES  = src/LicenseRegisterDialog.cxx
SOURCES += src/SandBoxBase.cxx

!mac {
	HEADERS += inc/SandBoxWindows.h
	SOURCES += src/SandBoxWindows.cxx
}

mac {
	FORMS += inc/ImportPlaylistDialog.ui

	HEADERS += inc/ImportPlaylistDialog.h
	HEADERS += inc/SandBoxMac.h

	OBJECTIVE_SOURCES += src/ImportPlaylistDialog.mm
	OBJECTIVE_SOURCES += src/SandBoxMac.mm
	
	LIBS += -framework Cocoa
	LIBS += -framework Foundation
	LIBS += -framework AppKit
}

message( "Done - GNUmakefile generated" )
