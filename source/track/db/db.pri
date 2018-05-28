isEmpty(_ORCUS_TRACK_DB_PRI) {
_ORCUS_TRACK_DB_PRI=1

message( "Parsing track_db.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_nowidget_dll.pri )

LIBS += -ltrack_db

}
