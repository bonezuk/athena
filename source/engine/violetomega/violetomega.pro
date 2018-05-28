message( "Parsing violetomega.pro" )

include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/engine/engine.pri )

message( "Processing library violetomega" )

TARGET = violetomega

HEADERS  = inc/IFFChunk.h
HEADERS += inc/IFFCommonChunk.h
HEADERS += inc/IFFFile.h
HEADERS += inc/IFFFormChunk.h
HEADERS += inc/IFFID3Chunk.h
HEADERS += inc/IFFSoundChunk.h
HEADERS += inc/IFFTextChunk.h
HEADERS += inc/VioletCodec.h
HEADERS += inc/VioletOmegaDLL.h

SOURCES  = src/IFFChunk.cxx
SOURCES += src/IFFCommonChunk.cxx
SOURCES += src/IFFFile.cxx
SOURCES += src/IFFFormChunk.cxx
SOURCES += src/IFFID3Chunk.cxx
SOURCES += src/IFFSoundChunk.cxx
SOURCES += src/IFFTextChunk.cxx
SOURCES += src/VioletCodec.cxx

message( "Done - GNUmakefile generated" )
