isEmpty(_ORCUS_ENGINE_MONKEYOMEGA_PRI) {
_ORCUS_ENGINE_MONKEYOMEGA_PRI=1

message( "Parsing monkeyomega.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_nowidget_dll.pri )

LIBS += -lmonkeyomega

}
