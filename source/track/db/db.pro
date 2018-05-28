message( "Parsing track_db.pro" )

include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/engine/engine.pri )
include ( $(OMEGA_HOME)/engine/silveromega/silveromega.pri )
include ( $(OMEGA_HOME)/engine/redomega/redomega.pri )
include ( $(OMEGA_HOME)/engine/whiteomega/whiteomega.pri )
include ( $(OMEGA_HOME)/engine/greenomega/greenomega.pri )
include ( $(OMEGA_HOME)/track/info/info.pri )
include ( $(OMEGA_HOME)/network/network.pri )
include ( $(OMEGA_HOME)/dlna/dlna.pri )

message( "Processing library track_db" )

TARGET = track_db

HEADERS  = inc/DbDLL.h
HEADERS += inc/DBInfo.h
HEADERS += inc/ITunesLocation.h
HEADERS += inc/SBBookmarkTrackDB.h
HEADERS += inc/Schema.h
HEADERS += inc/sqlite3.h
HEADERS += inc/SQLiteDatabase.h
HEADERS += inc/SQLiteException.h
HEADERS += inc/SQLiteInsert.h
HEADERS += inc/SQLiteLog.h
HEADERS += inc/SQLiteOperation.h
HEADERS += inc/SQLiteQuery.h
HEADERS += inc/TrackDB.h
HEADERS += inc/TrackFileDependencies.h

SOURCES  = src/DBInfo.cxx
SOURCES += src/ITunesLocation.cxx
SOURCES += src/Schema.cxx
SOURCES += src/sqlite3.c
SOURCES += src/SQLiteDatabase.cxx
SOURCES += src/SQLiteException.cxx
SOURCES += src/SQLiteInsert.cxx
SOURCES += src/SQLiteLog.cxx
SOURCES += src/SQLiteOperation.cxx
SOURCES += src/SQLiteQuery.cxx
SOURCES += src/TrackDB.cxx
SOURCES += src/TrackFileDependencies.cxx

mac {
	HEADERS += inc/ITunesLocationMac.h
	HEADERS += inc/SandboxBookmarkService.h
	
	OBJECTIVE_SOURCES += src/ITunesLocationMac.mm
	OBJECTIVE_SOURCES += src/SBBookmarkTrackDB.mm
	OBJECTIVE_SOURCES += src/SandboxBookmarkService.mm
	
	LIBS += -framework Cocoa
	LIBS += -framework Foundation
}

message ( "Done - GNUmakefile generated" )
