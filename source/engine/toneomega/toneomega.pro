message( "Parsing toneomega.pro" )

include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/engine/engine.pri )

message( "Processing library toneomega" )

TARGET = toneomega

HEADERS  = inc/ToneCodec.h
HEADERS += inc/ToneOmegaDLL.h

SOURCES  = src/ToneCodec.cxx

message( "Done - GNUmakefile generated" )
