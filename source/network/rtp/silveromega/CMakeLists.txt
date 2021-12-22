add_definitions(-DORCUS_NETWORK_RTPSILVEROMEGA_DLL)

file(GLOB NETWORK_RTP_SILVEROMEGA_HEADER inc/*.h)
file(GLOB NETWORK_RTP_SILVEROMEGA_SOURCE src/*.cxx)

add_library(rtp_silveromega SHARED ${NETWORK_RTP_SILVEROMEGA_HEADER} ${NETWORK_RTP_SILVEROMEGA_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(rtp_silveromega network_omega rtp silveromega engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml winmm ws2_32)
elseif (OMEGA_MACOSX)
	target_link_libraries(rtp_silveromega network_omega rtp silveromega engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml)
endif (OMEGA_WIN32)

if (${TIGER_LINUX_DISTRO})
	install(TARGETS rtp_silveromega DESTINATION lib)
endif (${TIGER_LINUX_DISTRO})
