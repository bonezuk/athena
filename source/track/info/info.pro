message( "Parsing track_info.pro" )

include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/engine/engine.pri )
include ( $(OMEGA_HOME)/engine/silveromega/silveromega.pri )
include ( $(OMEGA_HOME)/engine/redomega/redomega.pri )
include ( $(OMEGA_HOME)/engine/whiteomega/whiteomega.pri )
include ( $(OMEGA_HOME)/engine/greenomega/greenomega.pri )
include ( $(OMEGA_HOME)/engine/violetomega/violetomega.pri )
include ( $(OMEGA_HOME)/engine/cyanomega/cyanomega.pri )
include ( $(OMEGA_HOME)/dlna/dlna.pri )

include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/musepack.pri )

message( "Processing library track_info" )

TARGET = track_info

HEADERS  = inc/AIFFInfo.h
HEADERS += inc/APEInfo.h
HEADERS += inc/APETag.h
HEADERS += inc/AtomInfo.h
HEADERS += inc/CueInfo.h
HEADERS += inc/CueParser.h
HEADERS += inc/DirInfo.h
HEADERS += inc/EmbeddedID3Info.h
HEADERS += inc/FileInfo.h
HEADERS += inc/FLACInfo.h
HEADERS += inc/ID3Info1.h
HEADERS += inc/ID3Info2.h
HEADERS += inc/Info.h
HEADERS += inc/InfoDLL.h
HEADERS += inc/MPCChapterTag.h
HEADERS += inc/PListXMLParser.h
HEADERS += inc/VorbisInfo.h
HEADERS += inc/WavInfo.h
HEADERS += inc/XMCDInfo.h

SOURCES  = src/AIFFInfo.cxx
SOURCES += src/APEInfo.cxx
SOURCES += src/APETag.cxx
SOURCES += src/AtomInfo.cxx
SOURCES += src/CueInfo.cxx
SOURCES += src/CueParser.cxx
SOURCES += src/DirInfo.cxx
SOURCES += src/EmbeddedID3Info.cxx
SOURCES += src/FileInfo.cxx
SOURCES += src/FLACInfo.cxx
SOURCES += src/ID3Info1.cxx
SOURCES += src/ID3Info2.cxx
SOURCES += src/Info.cxx
SOURCES += src/MPCChapterTag.cxx
SOURCES += src/PListXMLParser.cxx
SOURCES += src/VorbisInfo.cxx
SOURCES += src/WavInfo.cxx
SOURCES += src/XMCDInfo.cxx

message ( "Done - GNUmakefile generated" )
