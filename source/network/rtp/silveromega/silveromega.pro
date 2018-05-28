message( "Parsing rtp_silveromega.pro" )

include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/engine/engine.pri )
include ( $(OMEGA_HOME)/engine/silveromega/silveromega.pri )
include ( $(OMEGA_HOME)/network/network.pri )
include ( $(OMEGA_HOME)/network/rtp/rtp.pri )

message( "Processing library rtp_silveromega" )

TARGET = rtp_silveromega

HEADERS  = inc/RTPSilverOmegaDLL.h
HEADERS += inc/SilverRTPCodec.h
HEADERS += inc/SilverSession.h
HEADERS += inc/VOggPackets.h
HEADERS += inc/VTimeCalc.h

SOURCES  = src/SilverRTPCodec.cxx
SOURCES += src/SilverSession.cxx
SOURCES += src/VOggPackets.cxx
SOURCES += src/VTimeCalc.cxx

message ( "Done - GNUmakefile generated" )
