add_definitions(-DORCUS_NETWORK_DLL)

if (OMEGA_MSVC12)
	add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
endif (OMEGA_MSVC12)

file(GLOB NETWORK_HEADER inc/*.h)
file(GLOB NETWORK_SOURCE src/*.cxx)

add_library(network_omega SHARED ${NETWORK_HEADER} ${NETWORK_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(network_omega engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml winmm ws2_32)
elseif (OMEGA_MACOSX)
	target_link_libraries(network_omega engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml)
endif (OMEGA_WIN32)
