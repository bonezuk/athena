isEmpty(_ORCUS_ENGINE_REDOMEGA_PRI) {
_ORCUS_ENGINE_REDOMEGA_PRI=1

message( "Parsing redomega.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_nowidget_dll.pri )

LIBS += -lredomega

}
