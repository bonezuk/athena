isEmpty(_ORCUS_MIME_PRI) {
_ORCUS_MIME_PRI=1

message( "Parsing mime.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_nowidget_dll.pri )

LIBS += -lmime

}
