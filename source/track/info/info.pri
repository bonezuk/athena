isEmpty(_ORCUS_TRACK_INFO_PRI) {
_ORCUS_TRACK_INFO_PRI=1

message( "Parsing track_info.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_nowidget_dll.pri )

LIBS += -ltrack_info

}
