isEmpty(_ORCUS_ENGINE_WHITEOMEGA_PRI) {
_ORCUS_ENGINE_WHITEOMEGA_PRI=1

message( "Parsing whiteomega.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_nowidget_dll.pri )

LIBS += -lwhiteomega

}
