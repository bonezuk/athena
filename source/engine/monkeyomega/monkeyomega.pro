message( "Parsing monkeyomega.pro" )

include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/engine/engine.pri )

include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/monkeyaudio.pri )

message( "Processing library monkeyomega" )

TARGET = monkeyomega

HEADERS  = inc/MonkeyCodec.h
HEADERS += inc/MonkeyIF.h
HEADERS += inc/MonkeyOmegaDLL.h

SOURCES  = src/MonkeyCodec.cxx
SOURCES += src/MonkeyIF.cxx

message( "Done - GNUmakefile generated" )
