isEmpty(_ORCUS_RTP_PRI) {
_ORCUS_RTP_PRI=1

message( "Parsing rtp.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_nowidget_dll.pri )

LIBS += -lrtp

}
