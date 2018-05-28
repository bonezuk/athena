message( "Parsing dlna.pro" )

include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/libupnp.pri )

message( "Processing library dlna" )

TARGET = dlna

HEADERS  = inc/DiskIF.h
HEADERS += inc/DiskLayerIF.h
HEADERS += inc/DLNADLL.h
HEADERS += inc/UPnPDeviceDescription.h
HEADERS += inc/UPnPIF.h
HEADERS += inc/UPnPLayerIF.h
HEADERS += inc/UPnPProvider.h
HEADERS += inc/UPnPServiceDescription.h
HEADERS += inc/UPnPSpecVersion.h
HEADERS += inc/XMLLibIF.h
HEADERS += inc/XMLLibLayerIF.h
HEADERS += inc/XMLNodeParser.h
HEADERS += inc/XMLReadOps.h

SOURCES  = src/DiskIF.cxx
SOURCES += src/DiskLayerIF.cxx
SOURCES += src/UPnPDeviceDescription.cxx
SOURCES += src/UPnPIF.cxx
SOURCES += src/UPnPLayerIF.cxx
SOURCES += src/UPnPProvider.cxx
SOURCES += src/UPnPServiceDescription.cxx
SOURCES += src/UPnPSpecVersion.cxx
SOURCES += src/XMLLibIF.cxx
SOURCES += src/XMLLibLayerIF.cxx
SOURCES += src/XMLNodeParser.cxx
SOURCES += src/XMLReadOps.cxx

message ( "Done - GNUmakefile generated" )
