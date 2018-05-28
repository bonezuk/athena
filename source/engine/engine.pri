isEmpty(_ORCUS_ENGINE_PRI) {
_ORCUS_ENGINE_PRI=1

message( "Parsing engine.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_nowidget_dll.pri )

LIBS += -lengine

}
