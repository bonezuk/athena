message( "Parsing mime.pro" )

include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/engine/engine.pri )
include ( $(OMEGA_HOME)/network/network.pri )
include ( $(OMEGA_HOME)/network/http/http.pri )

message( "Processing library mime" )

TARGET = mime

HEADERS  = inc/LicenseClient.h
HEADERS += inc/LicenseFile.h
HEADERS += inc/MimeForm.h
HEADERS += inc/MimeHeader.h
HEADERS += inc/MimeHeaderItem.h
HEADERS += inc/MimeNetworkDLL.h
HEADERS += inc/MimeSection.h

SOURCES  = src/LicenseClient.cxx
SOURCES += src/LicenseFile.cxx
SOURCES += src/MimeForm.cxx
SOURCES += src/MimeHeader.cxx
SOURCES += src/MimeHeaderItem.cxx
SOURCES += src/MimeSection.cxx

message ( "Done - GNUmakefile generated" )
