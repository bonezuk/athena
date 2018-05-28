isEmpty(_ORCUS_ENGINE_TONEOMEGA_PRI) {
_ORCUS_ENGINE_TONEOMEGA_PRI=1

message( "Parsing toneomega.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_nowidget_dll.pri )

LIBS += -ltoneomega

}
