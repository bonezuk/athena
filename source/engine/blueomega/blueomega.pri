isEmpty(_ORCUS_ENGINE_BLUEOMEGA_PRI) {
_ORCUS_ENGINE_BLUEOMEGA_PRI=1

message( "Parsing blueomega.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_nowidget_dll.pri )

LIBS += -lblueomega

}
