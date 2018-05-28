isEmpty(_ORCUS_HTTP_PRI) {
_ORCUS_HTTP_PRI=1

message( "Parsing http.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_nowidget_dll.pri )

LIBS += -lhttp

}
