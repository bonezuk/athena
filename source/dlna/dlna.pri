isEmpty(_ORCUS_DLNA_PRI) {
_ORCUS_DLNA_PRI=1

message( "Parsing.dlna.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_nowidget_dll.pri )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/libupnp.pri )

LIBS += -ldlna

}
