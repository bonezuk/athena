isEmpty(_ORCUS_TRACK_SPOTIFY_PRI) {
_ORCUS_TRACK_SPOTIFY_PRI=1

message( "Parsing track_spotify.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_nowidget_dll.pri )

LIBS += -ltrack_spotify

}
