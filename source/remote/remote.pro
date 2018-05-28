message( "Parsing remote.pro" )

include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_dll.pri )
include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/network/network.pri )
include ( $(OMEGA_HOME)/dlna/dlna.pri )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/libupnp.pri )

message( "Processing library remote" )

TARGET = remote

RESOURCES = ./remote.qrc

HEADERS  = inc/RemoteDLL.h
HEADERS += inc/KeyAssignment.h
HEADERS += inc/KeyCodeExclusion.h
HEADERS += inc/KeyCodesContainer.h
HEADERS += inc/KeyCodeStateEngine.h
HEADERS += inc/KeyControlService.h
HEADERS += inc/KeyStateParser.h
HEADERS += inc/RemoteIF.h
HEADERS += inc/QKeyLineEdit.h
HEADERS += inc/WinLIRCClient.h
HEADERS += inc/WinLIRCRemote.h
HEADERS += inc/WinLIRCService.h
HEADERS += inc/WinLIRCSettings.h

SOURCES  = src/KeyAssignment.cxx
SOURCES += src/KeyCodeExclusion.cxx
SOURCES += src/KeyCodesContainer.cxx
SOURCES += src/KeyCodeStateEngine.cxx
SOURCES += src/KeyControlService.cxx
SOURCES += src/KeyStateParser.cxx
SOURCES += src/RemoteIF.cxx
SOURCES += src/QKeyLineEdit.cxx
SOURCES += src/WinLIRCClient.cxx
SOURCES += src/WinLIRCRemote.cxx
SOURCES += src/WinLIRCService.cxx
SOURCES += src/WinLIRCSettings.cxx

mac {
	HEADERS += inc/AppleIRRemoteController.h
	HEADERS += inc/AppleRemoteService.h
	
	OBJECTIVE_SOURCES += src/AppleIRRemoteController.mm
	OBJECTIVE_SOURCES += src/AppleRemoteService.mm

	LIBS += -framework Cocoa
	LIBS += -framework AppKit
	LIBS += -framework IOKit
}

message ( "Done - GNUmakefile generated" )
