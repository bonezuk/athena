add_definitions(-DORCUS_NETWORKRTP_DLL)

file(GLOB NETWORK_RTP_HEADER inc/*.h)
file(GLOB NETWORK_RTP_SOURCE src/*.cxx)

add_library(rtp SHARED ${NETWORK_RTP_HEADER} ${NETWORK_RTP_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(rtp network_omega engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml winmm ws2_32)
elseif (OMEGA_MACOSX)
	target_link_libraries(rtp network_omega engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml)
endif (OMEGA_WIN32)
