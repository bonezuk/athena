add_definitions(-DORCUS_MIME_DLL)

file(GLOB NETWORK_MIME_HEADER inc/*.h)
file(GLOB NETWORK_MIME_SOURCE src/*.cxx)

add_library(mime SHARED ${NETWORK_MIME_HEADER} ${NETWORK_MIME_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(mime network_omega http engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml winmm ws2_32)
elseif (OMEGA_MACOSX)
	target_link_libraries(mime network_omega http engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml)
endif (OMEGA_WIN32)

if (${TIGER_LINUX_DISTRO})
	install(TARGETS mime DESTINATION lib)
endif (${TIGER_LINUX_DISTRO})
