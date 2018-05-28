message( "Parsing http.pro" )

include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/engine/engine.pri )
include ( $(OMEGA_HOME)/network/network.pri )

message( "Processing library http" )

TARGET = http

HEADERS  = inc/Directory.h
HEADERS += inc/HTTPClient.h
HEADERS += inc/HTTPClientService.h
HEADERS += inc/HTTPConnection.h
HEADERS += inc/HTTPDLL.h
HEADERS += inc/HTTPServer.h
HEADERS += inc/HTTPService.h
HEADERS += inc/Query.h
HEADERS += inc/ResponseCodes.h
HEADERS += inc/Unit.h

SOURCES  = src/HTTPClient.cxx
SOURCES += src/HTTPClientService.cxx
SOURCES += src/HTTPConnection.cxx
SOURCES += src/HTTPServer.cxx
SOURCES += src/HTTPService.cxx
SOURCES += src/Query.cxx
SOURCES += src/ResponseCodes.cxx
SOURCES += src/Unit.cxx

message ( "Done - GNUmakefile generated" )
