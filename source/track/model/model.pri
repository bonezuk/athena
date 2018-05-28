isEmpty(_ORCUS_TRACK_MODEL_PRI) {
_ORCUS_TRACK_MODEL_PRI=1

message( "Parsing track_model.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_nowidget_dll.pri )

LIBS += -ltrack_model

}
