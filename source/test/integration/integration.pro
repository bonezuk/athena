message( "Parsing integration.pro" )

include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_dll.pri )
include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/engine/engine.pri )
include ( $(OMEGA_HOME)/engine/blackomega/blackomega.pri )
include ( $(OMEGA_HOME)/engine/silveromega/silveromega.pri )
include ( $(OMEGA_HOME)/engine/blueomega/blueomega.pri )
include ( $(OMEGA_HOME)/engine/redomega/redomega.pri )
include ( $(OMEGA_HOME)/engine/whiteomega/whiteomega.pri )
include ( $(OMEGA_HOME)/engine/greenomega/greenomega.pri )
include ( $(OMEGA_HOME)/engine/violetomega/violetomega.pri )
include ( $(OMEGA_HOME)/engine/cyanomega/cyanomega.pri )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/musepack.pri )
!linux-build {
	include ( $(OMEGA_HOME)/engine/monkeyomega/monkeyomega.pri )
	include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/monkeyaudio.pri )
}
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

include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/gtest.pri )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/gmock.pri )

message( "Processing integration" )

QT += testlib

TEMPLATE = app
CONFIG -= app_bundle

bundle {
	DEFINES += OMEGA_BUNDLE_TEST
}

TARGET = "Integration"

LIBS     += -L$${TIGER_INSTALL_DIR}

HEADERS  = ../../remote/test/integration/WinLIRCDummyServer.h
HEADERS += ../../remote/test/integration/QKeyLineEditTester.h

SOURCES += ../../engine/test/integration/FFTRadix2Test.cxx
SOURCES += ../../remote/test/integration/WinLIRCDummyServer.cxx
SOURCES += ../../remote/test/integration/QKeyLineEditTester.cxx

HEADERS += ../../engine/violetomega/test/integration/VioletPlayback.h
SOURCES += ../../engine/violetomega/test/integration/VioletPlayback.cxx

HEADERS += ../../engine/cyanomega/test/integration/CyanPlayback.h
SOURCES += ../../engine/cyanomega/test/integration/CyanPlayback.cxx

!linux-build {
	HEADERS += ../../engine/monkeyomega/test/integration/MonkeyPlayback.h
	SOURCES += ../../engine/monkeyomega/test/integration/MonkeyPlayback.cxx
	SOURCES += ../../engine/monkeyomega/test/integration/MonkeyDecodeTest.cxx
}

linux-build {
	HEADERS += ../../audioio/test/integration/ALSATest.h
	SOURCES += ../../audioio/test/integration/ALSATest.cxx
	LIBS += -lasound
}

SOURCES += ../../audioio/test/integration/SampleConvertorExpectTest.cxx

SOURCES += ./TestIntegrationMain.cxx

mac {
	HEADERS += ../../remote/test/integration/AppleRemoteServiceIntegration.h
	OBJECTIVE_SOURCES += ../../remote/test/integration/AppleRemoteServiceIntegration.mm

	HEADERS += ../../audioio/test/integration/AOCoreHALInteger.h
	OBJECTIVE_SOURCES += ../../audioio/test/integration/AOCoreHALInteger.mm

	LIBS += -framework Cocoa
	LIBS += -framework AppKit
	LIBS += -framework IOKit
	LIBS += -framework CoreServices
	LIBS += -framework CoreAudio
	LIBS += -framework AudioUnit
	LIBS += -framework AudioToolbox
}

message ( "Done - GNUmakefile generated" )
