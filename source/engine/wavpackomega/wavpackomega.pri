isEmpty(_ORCUS_ENGINE_WAVPACKOMEGA_PRI) {
_ORCUS_ENGINE_WAVPACKOMEGA_PRI=1

message( "Parsing wavpackomega.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_nowidget_dll.pri )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/wavpack.pri )

LIBS += -lwavpackomega

}
