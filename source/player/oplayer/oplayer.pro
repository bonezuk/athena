message( "Parsing oplayer.pro" )

include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_dll.pri )
include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/engine/engine.pri )
include ( $(OMEGA_HOME)/engine/blackomega/blackomega.pri )
include ( $(OMEGA_HOME)/engine/silveromega/silveromega.pri )
include ( $(OMEGA_HOME)/engine/blueomega/blueomega.pri )
include ( $(OMEGA_HOME)/engine/cyanomega/cyanomega.pri )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/musepack.pri )
include ( $(OMEGA_HOME)/engine/redomega/redomega.pri )
include ( $(OMEGA_HOME)/engine/whiteomega/whiteomega.pri )
include ( $(OMEGA_HOME)/engine/greenomega/greenomega.pri )
include ( $(OMEGA_HOME)/engine/violetomega/violetomega.pri )
include ( $(OMEGA_HOME)/engine/toneomega/toneomega.pri )
!linux-build {
	include ( $(OMEGA_HOME)/engine/monkeyomega/monkeyomega.pri )
}
include ( $(OMEGA_HOME)/engine/wavpackomega/wavpackomega.pri )
include ( $(OMEGA_HOME)/audioio/audioio.pri )
include ( $(OMEGA_HOME)/network/network.pri )
include ( $(OMEGA_HOME)/network/rtp/rtp.pri )
include ( $(OMEGA_HOME)/network/rtp/silveromega/silveromega.pri )
include ( $(OMEGA_HOME)/network/http/http.pri )
include ( $(OMEGA_HOME)/network/mime/mime.pri )
include ( $(OMEGA_HOME)/track/info/info.pri )
include ( $(OMEGA_HOME)/track/db/db.pri )
include ( $(OMEGA_HOME)/track/model/model.pri )
include ( $(OMEGA_HOME)/dlna/dlna.pri )
include ( $(OMEGA_HOME)/remote/remote.pri )
include ( $(OMEGA_HOME)/widget/widget.pri )

message( "Processing oplayer" )

TEMPLATE = app

TARGET = "oplayer"

LIBS     += -L$${TIGER_INSTALL_DIR}

HEADERS  = ./inc/OPlayerMain.h

SOURCES  = ./src/OPlayerMain.cxx

mac {
	LIBS += -framework Cocoa
	LIBS += -framework AppKit
	LIBS += -framework IOKit
	LIBS += -framework CoreServices
	LIBS += -framework CoreAudio
	LIBS += -framework AudioUnit
	LIBS += -framework AudioToolbox
}

message ( "Done - GNUmakefile generated" )
