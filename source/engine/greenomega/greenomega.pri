isEmpty(_ORCUS_ENGINE_GREENOMEGA_PRI) {
_ORCUS_ENGINE_GREENOMEGA_PRI=1

message( "Parsing greenomega.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_nowidget_dll.pri )

LIBS += -lgreenomega

}
