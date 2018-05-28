isEmpty(_ORCUS_ENGINE_SILVEROMEGA_PRI) {
_ORCUS_ENGINE_SILVEROMEGA_PRI=1

message( "Parsing silveromega.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_nowidget_dll.pri )

LIBS += -lsilveromega

}
