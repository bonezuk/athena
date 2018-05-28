message( "Parsing track_model.pro" )

include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/engine/engine.pri )
include ( $(OMEGA_HOME)/engine/blackomega/blackomega.pri )
include ( $(OMEGA_HOME)/engine/silveromega/silveromega.pri )
include ( $(OMEGA_HOME)/engine/blueomega/blueomega.pri )
include ( $(OMEGA_HOME)/engine/redomega/redomega.pri )
include ( $(OMEGA_HOME)/engine/whiteomega/whiteomega.pri )
include ( $(OMEGA_HOME)/engine/greenomega/greenomega.pri )
include ( $(OMEGA_HOME)/audioio/audioio.pri )
include ( $(OMEGA_HOME)/network/network.pri )
include ( $(OMEGA_HOME)/network/rtp/rtp.pri )
include ( $(OMEGA_HOME)/network/rtp/silveromega/silveromega.pri )
include ( $(OMEGA_HOME)/network/http/http.pri )
include ( $(OMEGA_HOME)/network/mime/mime.pri )
include ( $(OMEGA_HOME)/track/info/info.pri )
include ( $(OMEGA_HOME)/track/db/db.pri )

message( "Processing library track_model" )

TARGET = track_model

HEADERS  = inc/AbstractTrackItem.h
HEADERS += inc/AbstractTrackModel.h
HEADERS += inc/AlbumModel.h
HEADERS += inc/AlbumModelKey.h
HEADERS += inc/AlbumModelUtilities.h
HEADERS += inc/ImageRepositary.h
HEADERS += inc/TrackModelDLL.h
HEADERS += inc/TrackModelKey.h

SOURCES  = src/AbstractTrackItem.cxx
SOURCES += src/AbstractTrackModel.cxx
SOURCES += src/AlbumModel.cxx
SOURCES += src/AlbumModelKey.cxx
SOURCES += src/AlbumModelUtilities.cxx
SOURCES += src/ImageRepositary.cxx
SOURCES += src/TrackModelKey.cxx

message ( "Done - GNUmakefile generated" )
