message( "Parsing greenomega.pro" )

include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/engine/engine.pri )

message( "Processing library greenomega" )

TARGET = greenomega

HEADERS  = inc/FLACFrame.h
HEADERS += inc/FLACFramework.h
HEADERS += inc/FLACMetaBase.h
HEADERS += inc/FLACMetaComment.h
HEADERS += inc/FLACMetaCuesheet.h
HEADERS += inc/FLACMetaPicture.h
HEADERS += inc/FLACMetaSeekTable.h
HEADERS += inc/FLACMetaStreamInfo.h
HEADERS += inc/FLACSubframe.h
HEADERS += inc/GreenCodec.h
HEADERS += inc/GreenOmegaDLL.h

SOURCES  = src/FLACFrame.cxx
SOURCES += src/FLACFramework.cxx
SOURCES += src/FLACHeader.cxx
SOURCES += src/FLACMetaBase.cxx
SOURCES += src/FLACMetaComment.cxx
SOURCES += src/FLACMetaCuesheet.cxx
SOURCES += src/FLACMetaPicture.cxx
SOURCES += src/FLACMetaSeekTable.cxx
SOURCES += src/FLACMetaStreamInfo.cxx
SOURCES += src/FLACSubframe.cxx
SOURCES += src/GreenCodec.cxx

message( "Done - GNUmakefile generated" )
