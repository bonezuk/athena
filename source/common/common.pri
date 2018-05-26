isEmpty(_ORCUS_COMMON_PRI) {
_ORCUS_COMMON_PRI=1

message( "Parsing common.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_nowidget_dll.pri )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/libxml2.pri )

LIBS += -lcommon

}
