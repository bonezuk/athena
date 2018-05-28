add_definitions(-DORCUS_HTTP_DLL)

file(GLOB NETWORK_HTTP_HEADER inc/*.h)
file(GLOB NETWORK_HTTP_SOURCE src/*.cxx)

add_library(http SHARED ${NETWORK_HTTP_HEADER} ${NETWORK_HTTP_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(http network_omega engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml winmm ws2_32)
elseif (OMEGA_MACOSX)
	target_link_libraries(http network_omega engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml)
endif (OMEGA_WIN32)
