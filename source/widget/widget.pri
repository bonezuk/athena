isEmpty(_ORCUS_WIDGET_PRI) {
_ORCUS_WIDGET_PRI=1

message( "Parsing widget.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_dll.pri )

LIBS += -lwidget

}
