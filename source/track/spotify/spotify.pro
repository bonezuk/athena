message( "Parsing track_spotify.pro" )

include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/spotify.pri )
include ( $(OMEGA_HOME)/common/common.pri )

message( "Processing library track_spotify" )

TARGET = track_spotify

HEADERS  = inc/SpotifyAPIKey.h
HEADERS += inc/SpotifyDLL.h

SOURCES  = src/SpotifyAPIKey.cxx

message ( "Done - GNUmakefile generated" )
