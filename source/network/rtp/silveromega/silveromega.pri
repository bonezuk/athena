isEmpty(_ORCUS_RTP_SILVEROMEGA_PRI) {
_ORCUS_RTP_SILVEROMEGA_PRI=1

message( "Parsing rtp_silveromega.pri" )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_nowidget_dll.pri )

LIBS += -lrtp_silveromega

}
