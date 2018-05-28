message( "Parsing cyanomega.pro" )

include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/engine/engine.pri )

include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/musepack.pri )

message( "Processing library cyanomega" )

TARGET = cyanomega

HEADERS  = inc/CyanCodec.h
HEADERS += inc/CyanOmegaDLL.h
HEADERS += inc/MusePackDecoder.h
HEADERS += inc/MusePackDecoderImpl.h
HEADERS += inc/MusePackIF.h

SOURCES  = src/CyanCodec.cxx
SOURCES += src/MusePackDecoder.cxx
SOURCES += src/MusePackDecoderImpl.cxx
SOURCES += src/MusePackIF.cxx

message( "Done - GNUmakefile generated" )
