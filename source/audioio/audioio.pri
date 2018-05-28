isEmpty(_ORCUS_AUDIOIO_PRI) {
_ORCUS_AUDIOIO_PRI=1

message( "Parsing audioio.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_nowidget_dll.pri )

LIBS += -laudioio

}
