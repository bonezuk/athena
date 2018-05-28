message( "Parsing silveromega.pro" )

include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/engine/engine.pri )

message( "Processing library silveromega" )

TARGET = silveromega

HEADERS  = inc/SilverCodec.h
HEADERS += inc/SilverOmegaDLL.h
HEADERS += inc/VSilverCodebook.h
HEADERS += inc/VSilverCodecComments.h
HEADERS += inc/VSilverCodecData.h
HEADERS += inc/VSilverCodecInformation.h
HEADERS += inc/VSilverContainer.h
HEADERS += inc/VSilverFloorBase.h
HEADERS += inc/VSilverFloorData0.h
HEADERS += inc/VSilverFloorData1.h
HEADERS += inc/VSilverFunctions.h
HEADERS += inc/VSilverInfoObject.h
HEADERS += inc/VSilverMapData.h
HEADERS += inc/VSilverModeData.h
HEADERS += inc/VSilverOgg.h
HEADERS += inc/VSilverResidueBase.h
HEADERS += inc/VSilverSeeker.h
HEADERS += inc/VSilverWindow.h

SOURCES  = src/SilverCodec.cxx
SOURCES += src/VSilverCodebook.cxx
SOURCES += src/VSilverCodecComments.cxx
SOURCES += src/VSilverCodecData.cxx
SOURCES += src/VSilverCodecInformation.cxx
SOURCES += src/VSilverContainer.cxx
SOURCES += src/VSilverFloorBase.cxx
SOURCES += src/VSilverFloorData0.cxx
SOURCES += src/VSilverFloorData1.cxx
SOURCES += src/VSilverFrameSynthesis.cxx
SOURCES += src/VSilverFunctions.cxx
SOURCES += src/VSilverInfoObject.cxx
SOURCES += src/VSilverMapData.cxx
SOURCES += src/VSilverModeData.cxx
SOURCES += src/VSilverOgg.cxx
SOURCES += src/VSilverResidueBase.cxx
SOURCES += src/VSilverSeeker.cxx
SOURCES += src/VSilverWindow.cxx

message( "Done - GNUmakefile generated" )
