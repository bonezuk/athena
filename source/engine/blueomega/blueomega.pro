message( "Parsing blueomega.pro" )

include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/engine/engine.pri )

message( "Processing library blueomega" )

TARGET = blueomega

HEADERS  = inc/BlueOmegaDLL.h
HEADERS += inc/WaveEngine.h
HEADERS += inc/WaveInformation.h

SOURCES  = src/WaveEngine.cxx
SOURCES += src/WaveInformation.cxx

message( "Done - GNUmakefile generated" )
