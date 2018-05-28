isEmpty(_ORCUS_REMOTE_PRI) {
_ORCUS_REMOTE_PRI=1

message( "Parsing remote.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_dll.pri )

LIBS += -lremote

}
