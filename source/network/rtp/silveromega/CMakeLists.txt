add_definitions(-DORCUS_NETWORK_RTPSILVEROMEGA_DLL)

file(GLOB NETWORK_RTP_SILVEROMEGA_HEADER inc/*.h)
file(GLOB NETWORK_RTP_SILVEROMEGA_SOURCE src/*.cxx)

add_library(rtp_silveromega SHARED ${NETWORK_RTP_SILVEROMEGA_HEADER} ${NETWORK_RTP_SILVEROMEGA_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(rtp_silveromega network_omega rtp silveromega engine common xml2 qt5core qt5gui qt5xml winmm ws2_32)
elseif (OMEGA_MACOSX)
	target_link_libraries(rtp_silveromega network_omega rtp silveromega engine common xml2 qt5core qt5gui qt5xml)
endif (OMEGA_WIN32)
