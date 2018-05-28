isEmpty(_ORCUS_ENGINE_CYANOMEGA_PRI) {
_ORCUS_ENGINE_CYANOMEGA_PRI=1

message( "Parsing cyanomega.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_nowidget_dll.pri )

LIBS += -lcyanomega

}
