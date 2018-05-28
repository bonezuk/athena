message( "Parsing audioio.pro" )

include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/engine/engine.pri )
include ( $(OMEGA_HOME)/network/network.pri )
include ( $(OMEGA_HOME)/network/http/http.pri )
include ( $(OMEGA_HOME)/network/mime/mime.pri )

message( "Processing library audioio" )

TARGET = audioio

HEADERS  = inc/AbstractAudioHardwareBuffer.h
HEADERS += inc/AOBase.h
HEADERS += inc/AudioInfo.h
HEADERS += inc/AudioIODLL.h
HEADERS += inc/AOQueryDevice.h
HEADERS += inc/FormatDescription.h
HEADERS += inc/FormatDescriptionUtils.h
HEADERS += inc/FormatsSupported.h
HEADERS += inc/IOTimeStamp.h
HEADERS += inc/SampleConverter.h

SOURCES  = src/AbstractAudioHardwareBuffer.cxx
SOURCES += src/AOBase.cxx
SOURCES += src/AudioInfo.cxx
SOURCES += src/AOQueryDevice.cxx
SOURCES += src/FormatDescription.cxx
SOURCES += src/FormatDescriptionUtils.cxx
SOURCES += src/FormatsSupported.cxx
SOURCES += src/IOTimeStamp.cxx
SOURCES += src/SampleConverter.cxx

mac {
	HEADERS += inc/AOCoreAudio.h
	HEADERS += inc/AOQueryCoreAudio.h
	HEADERS += inc/AudioHardwareBufferCoreAudio.h
	HEADERS += inc/CoreAudioIF.h
	HEADERS += inc/CoreAudioLayerIF.h
	
	SOURCES += src/AudioHardwareBufferCoreAudio.cxx
	SOURCES += src/CoreAudioIF.cxx
	SOURCES += src/CoreAudioLayerIF.cxx
	
	OBJECTIVE_SOURCES += src/AOCoreAudio.mm
	OBJECTIVE_SOURCES += src/AOQueryCoreAudio.mm

	LIBS += -framework CoreServices
	LIBS += -framework CoreAudio
	LIBS += -framework AudioUnit
	LIBS += -framework AudioToolbox
	LIBS += -framework Cocoa
}

linux-build {
	HEADERS += inc/AOQueryALSA.h
	HEADERS += inc/AOLinuxALSA.h
	HEADERS += inc/AudioHardwareBufferALSA.h
	HEADERS += inc/LinuxALSAIF.h
	HEADERS += inc/LinuxALSALayerIF.h
	
	SOURCES += src/AOQueryALSA.cxx
	SOURCES += src/AOLinuxALSA.cxx
	SOURCES += src/AudioHardwareBufferALSA.cxx
	SOURCES += src/LinuxALSAIF.cxx
	SOURCES += src/LinuxALSALayerIF.cxx
	
	LIBS += -lasound
}

message( "Done - GNUmakefile generated" )
