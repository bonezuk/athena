message( "Parsing test.pro" )

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
include ( $(OMEGA_HOME)/engine/toneomega/toneomega.pri )
include ( $(OMEGA_HOME)/engine/violetomega/violetomega.pri )
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

include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/gtest.pri )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/gmock.pri )

message( "Processing test" )

QT += testlib

TEMPLATE = app

applestore {
	message( "App Store Info.plist" )
	QMAKE_INFO_PLIST = ./appstore/Info.plist
	QMAKE_LFLAGS += -sectcreate __TEXT __info_plist  $(OMEGA_HOME)/test/appstore/Info.plist
	CONFIG += app_bundle
}

bundle {
	DEFINES += OMEGA_BUNDLE_TEST
}

TARGET = "TestDLNA"

LIBS     += -L$${TIGER_INSTALL_DIR}

RESOURCES = ./test.qrc

HEADERS  = ../audioio/test/AOChannelMapMock.h
HEADERS += ../audioio/test/AudioHardwareBufferTester.h
HEADERS += ../audioio/test/AudioItemMock.h
HEADERS += ../audioio/test/AOBaseTest.h
HEADERS += ../audioio/test/AOQueryDeviceMock.h

HEADERS += ../common/test/AsynchronousFileReaderMock.h
HEADERS += ../common/test/AsynchronousFileReaderTest.h
HEADERS += ../common/test/BIOStreamMock.h
HEADERS += ../common/test/BIOTimeCachedStreamTest.h
HEADERS += ../common/test/CachedFileStreamTest.h
HEADERS += ../common/test/CachedFileStreamMock.h
HEADERS += ../common/test/ServiceEventTest.h

HEADERS += ../dlna/test/DiskMockIF.h
HEADERS += ../dlna/test/UPnPMockIF.h
HEADERS += ../dlna/test/UPnPProviderTestEnviroment.h
HEADERS += ../dlna/test/XMLLibMockIF.h

HEADERS += ../engine/test/ADataMock.h
HEADERS += ../engine/test/CodecMock.h
HEADERS += ../engine/test/CodecTest.h
HEADERS += ../engine/test/RDataMock.h

HEADERS += ../remote/test/KeyAssignmentMock.h
HEADERS += ../remote/test/RemoteIFMock.h
HEADERS += ../remote/test/WinLIRCSettingsMock.h

HEADERS += ../track/db/test/SQLiteMock.h

HEADERS += ../track/info/test/AIFFInfoTest.h
HEADERS += ../track/info/test/WavInfoTest.h

HEADERS += ../track/model/test/AbstractTrackModelTest.h
HEADERS += ../track/model/test/ImageRepositaryMock.h
HEADERS += ../track/model/test/TrackDBTestEnviroment.h

HEADERS += ./QUnitTestTimer.h

SOURCES  = ../audioio/test/AOBaseTest.cxx
SOURCES += ../audioio/test/AOChannelMapMock.cxx
SOURCES += ../audioio/test/AudioHardwareBufferTester.cxx
SOURCES += ../audioio/test/AudioItemMock.cxx
SOURCES += ../audioio/test/FormatDescriptionTest.cxx
SOURCES += ../audioio/test/FormatDescriptionUtilsTest.cxx
SOURCES += ../audioio/test/FormatsSupportedTest.cxx
SOURCES += ../audioio/test/IOTimeStampTest.cxx
SOURCES += ../audioio/test/SampleConverterTest.cxx

SOURCES += ../common/test/AsynchronousFileReaderMock.cxx
SOURCES += ../common/test/AsynchronousFileReaderTest.cxx
SOURCES += ../common/test/BIOStreamTest.cxx
SOURCES += ../common/test/BIOStreamFunctionalTest.cxx
SOURCES += ../common/test/BIOBufferedStreamTest.cxx
SOURCES += ../common/test/BIOMemoryTest.cxx
SOURCES += ../common/test/BIOMemoryStreamTest.cxx
SOURCES += ../common/test/BIOStreamMock.cxx
SOURCES += ../common/test/BIOTimeCachedStreamTest.cxx
SOURCES += ../common/test/CachedFileStreamTest.cxx
SOURCES += ../common/test/CachedFileStreamMock.cxx
SOURCES += ../common/test/CommonTypesTest.cxx
SOURCES += ../common/test/LargeNumberTest.cxx
SOURCES += ../common/test/CommonDirectoriesForFilesTest.cxx
SOURCES += ../common/test/CryptoRSAEncryptTest.cxx
SOURCES += ../common/test/DictionaryXMLMapTest.cxx
SOURCES += ../common/test/ProductVersionInfoTest.cxx
SOURCES += ../common/test/JaroWinklerDistanceTest.cxx
SOURCES += ../common/test/RandomTest.cxx
SOURCES += ../common/test/ServiceEventTest.cxx

SOURCES += ../dlna/test/DiskLayerTestIF.cxx
SOURCES += ../dlna/test/DiskMockIF.cxx
SOURCES += ../dlna/test/UPnPDeviceDescriptionTest.cxx
SOURCES += ../dlna/test/UPnPMockIF.cxx
SOURCES += ../dlna/test/UPnPProviderTest.cxx
SOURCES += ../dlna/test/UPnPProviderTestEnviroment.cxx
SOURCES += ../dlna/test/UPnPServiceDescriptionTest.cxx
SOURCES += ../dlna/test/UPnPSpecVersionTest.cxx
SOURCES += ../dlna/test/XMLLibMockIF.cxx
SOURCES += ../dlna/test/XMLNodeParserTest.cxx
SOURCES += ../dlna/test/XMLReadOpsTest.cxx

SOURCES += ../engine/test/ADataMock.cxx
SOURCES += ../engine/test/CodecMock.cxx
SOURCES += ../engine/test/CodecTest.cxx
SOURCES += ../engine/test/FormatTypeTest.cxx
SOURCES += ../engine/test/FormatType8BitTest.cxx
SOURCES += ../engine/test/FormatTypeFromFloatTest.cxx
SOURCES += ../engine/test/InterleavedCodecTest.cxx
SOURCES += ../engine/test/OrcusDCT64Test.cxx
SOURCES += ../engine/test/RDataMock.cxx
SOURCES += ../engine/test/RDataTest.cxx

SOURCES += ../engine/violetomega/test/IFFChunkTest.cxx
SOURCES += ../engine/violetomega/test/IFFCommonChunkTest.cxx
SOURCES += ../engine/violetomega/test/IFFFileTest.cxx
SOURCES += ../engine/violetomega/test/IFFFormChunkTest.cxx
SOURCES += ../engine/violetomega/test/IFFSoundChunkTest.cxx
SOURCES += ../engine/violetomega/test/IFFTextChunkTest.cxx

SOURCES += ../engine/wavpackomega/test/WavPackBIOReaderTest.cxx
SOURCES += ../engine/wavpackomega/test/WavPackCodecTest.cxx

SOURCES += ../network/test/ResourceTest.cxx

SOURCES += ../remote/test/KeyAssignmentMock.cxx
SOURCES += ../remote/test/KeyAssignmentTest.cxx
SOURCES += ../remote/test/KeyCodeExclusionTest.cxx
SOURCES += ../remote/test/KeyCodesContainerTest.cxx
SOURCES += ../remote/test/KeyCodeStateEngineTest.cxx
SOURCES += ../remote/test/KeyControlServiceTest.cxx
SOURCES += ../remote/test/KeyStateParserTest.cxx
SOURCES += ../remote/test/WinLIRCClientTest.cxx
SOURCES += ../remote/test/WinLIRCRemoteTest.cxx
SOURCES += ../remote/test/WinLIRCSettingsMock.cxx

SOURCES += ../track/db/test/ITunesLocationTest.cxx
SOURCES += ../track/db/test/SQLiteMock.cxx
SOURCES += ../track/db/test/TrackFileDependenciesTest.cxx

SOURCES += ../track/info/test/AIFFInfoTest.cxx
SOURCES += ../track/info/test/APEInfoTest.cxx
SOURCES += ../track/info/test/APETagTest.cxx
SOURCES += ../track/info/test/MPCChapterTagTest.cxx
SOURCES += ../track/info/test/CueInfoTest.cxx
SOURCES += ../track/info/test/InfoTest.cxx
SOURCES += ../track/info/test/WavInfoTest.cxx
SOURCES += ../track/info/test/XMCDInfoTest.cxx

SOURCES += ../track/model/test/AbstractTrackItemTest.cxx
SOURCES += ../track/model/test/AbstractTrackModelTest.cxx
SOURCES += ../track/model/test/AlbumModelTest.cxx
SOURCES += ../track/model/test/AlbumModelKeyTest.cxx
SOURCES += ../track/model/test/AlbumModelUtilitiesTest.cxx
SOURCES += ../track/model/test/ImageRepositaryMock.cxx
SOURCES += ../track/model/test/ImageRepositaryTest.cxx
SOURCES += ../track/model/test/TrackDBTestEnviroment.cxx
SOURCES += ../track/model/test/TrackModelKeyTest.cxx

SOURCES += ./QUnitTestTimer.cxx
SOURCES += ./TestMain.cxx

mac {
	HEADERS += ../audioio/test/CoreAudioMockIF.h
	
	SOURCES += ../audioio/test/CoreAudioMockIF.cxx
	SOURCES += ../audioio/test/AOCoreAudioTest.cxx
	SOURCES += ../audioio/test/AOQueryCoreAudioTest.cxx

	OBJECTIVE_SOURCES += ../track/db/test/SandboxBookmarkServiceTest.mm

	LIBS += -framework Cocoa
	LIBS += -framework AppKit
	LIBS += -framework IOKit
	LIBS += -framework CoreServices
	LIBS += -framework CoreAudio
	LIBS += -framework AudioUnit
	LIBS += -framework AudioToolbox
}

linux-build {
	HEADERS += ../audioio/test/AOQueryALSAMock.h
	HEADERS += ../audioio/test/LinuxALSAMockIF.h
	
	SOURCES += ../audioio/test/AOLinuxALSATest.cxx
	SOURCES += ../audioio/test/AOQueryALSATest.cxx
	SOURCES += ../audioio/test/AudioHardwareBufferALSATest.cxx
	SOURCES += ../audioio/test/LinuxALSAMockIF.cxx
}

message ( "Done - GNUmakefile generated" )
