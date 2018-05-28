message( "Parsing rtp.pro" )

include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/engine/engine.pri )
include ( $(OMEGA_HOME)/network/network.pri )

message( "Processing library rtp" )

TARGET = rtp

HEADERS  = inc/DataPacket.h
HEADERS += inc/NetworkRTPDLL.h
HEADERS += inc/RTCPPacket.h
HEADERS += inc/RTCPPacketAPP.h
HEADERS += inc/RTCPPacketBYE.h
HEADERS += inc/RTCPPacketRR.h
HEADERS += inc/RTCPPacketSDES.h
HEADERS += inc/RTCPPacketSR.h
HEADERS += inc/RTCPReportBlock.h
HEADERS += inc/RTPPacket.h
HEADERS += inc/Scheduler.h
HEADERS += inc/Session.h
HEADERS += inc/SourceState.h

SOURCES  = src/DataPacket.cxx
SOURCES += src/RTCPPacket.cxx
SOURCES += src/RTCPPacketAPP.cxx
SOURCES += src/RTCPPacketBYE.cxx
SOURCES += src/RTCPPacketRR.cxx
SOURCES += src/RTCPPacketSDES.cxx
SOURCES += src/RTCPPacketSR.cxx
SOURCES += src/RTCPReportBlock.cxx
SOURCES += src/RTPPacket.cxx
SOURCES += src/Scheduler.cxx
SOURCES += src/Session.cxx
SOURCES += src/SourceState.cxx

message ( "Done - GNUmakefile generated" )
