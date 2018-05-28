message( "Parsing network.pro" )

include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/engine/engine.pri )

message( "Processing library network" )

TARGET = network

HEADERS  = inc/Controller.h
HEADERS += inc/Memory.h
HEADERS += inc/MemoryPacket.h
HEADERS += inc/NTPClient.h
HEADERS += inc/NetworkDLL.h
HEADERS += inc/Resource.h
HEADERS += inc/Service.h
HEADERS += inc/Socket.h
HEADERS += inc/TCPClientService.h
HEADERS += inc/TCPConnClientSocket.h
HEADERS += inc/TCPConnServerSocket.h
HEADERS += inc/TCPConnectionSocket.h
HEADERS += inc/TCPServerSocket.h
HEADERS += inc/TCPService.h
HEADERS += inc/UDPRead.h
HEADERS += inc/UDPWrite.h

SOURCES  = src/Controller.cxx
SOURCES += src/Memory.cxx
SOURCES += src/MemoryPacket.cxx
SOURCES += src/NTPClient.cxx
SOURCES += src/Resource.cxx
SOURCES += src/Service.cxx
SOURCES += src/Socket.cxx
SOURCES += src/TCPClientService.cxx
SOURCES += src/TCPConnClientSocket.cxx
SOURCES += src/TCPConnServerSocket.cxx
SOURCES += src/TCPConnectionSocket.cxx
SOURCES += src/TCPServerSocket.cxx
SOURCES += src/TCPService.cxx
SOURCES += src/UDPRead.cxx
SOURCES += src/UDPWrite.cxx

message ( "Done - GNUmakefile generated" )
