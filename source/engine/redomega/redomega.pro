message( "Parsing redomega.pro" )

include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/engine/engine.pri )

message( "Processing library redomega" )

TARGET = redomega

HEADERS  = inc/ALACAdaptiveDecode.h
HEADERS += inc/ALACCafContainer.h
HEADERS += inc/ALACContainer.h
HEADERS += inc/ALACDecoder.h
HEADERS += inc/ALACDescription.h
HEADERS += inc/ALACFrameIndex.h
HEADERS += inc/ALACMatrix.h
HEADERS += inc/ALACSequence.h
HEADERS += inc/ALACSpecificConfig.h
HEADERS += inc/CAFFile.h
HEADERS += inc/RedCodec.h
HEADERS += inc/RedOmegaDLL.h

SOURCES  = src/ALACAdaptiveDecode.cxx
SOURCES += src/ALACCafContainer.cxx
SOURCES += src/ALACContainer.cxx
SOURCES += src/ALACDecoder.cxx
SOURCES += src/ALACDescription.cxx
SOURCES += src/ALACFrameIndex.cxx
SOURCES += src/ALACMatrix.cxx
SOURCES += src/ALACSequence.cxx
SOURCES += src/ALACSpecificConfig.cxx
SOURCES += src/CAFFile.cxx
SOURCES += src/RedCodec.cxx

message( "Done - GNUmakefile generated" )
