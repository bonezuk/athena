message( "Parsing wavpackomega.pro" )

include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/engine/engine.pri )

include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/wavpack.pri )

message( "Processing library wavpackomega" )

TARGET = wavpackomega

HEADERS  = inc/WavPackBIOReader.h
HEADERS += inc/WavPackCodec.h
HEADERS += inc/WavPackOmegaDLL.h

SOURCES  = src/WavPackBIOReader.cxx
SOURCES += src/WavPackCodec.cxx

message( "Done - GNUmakefile generated" )
