isEmpty(_ORCUS_ENGINE_VIOLETOMEGA_PRI) {
_ORCUS_ENGINE_VIOLETOMEGA_PRI=1

message( "Parsing violetomega.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_nowidget_dll.pri )

LIBS += -lvioletomega

}
