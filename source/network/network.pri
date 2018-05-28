isEmpty(_ORCUS_NETWORK_PRI) {
_ORCUS_NETWORK_PRI=1

message( "Parsing network.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_nowidget_dll.pri )

LIBS += -lnetwork

}
