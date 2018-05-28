isEmpty(_ORCUS_ENGINE_BLACKOMEGA_PRI) {
_ORCUS_ENGINE_BLACKOMEGA_PRI=1

message( "Parsing blackomega.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_nowidget_dll.pri )

LIBS += -lblackomega

}
