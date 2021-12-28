add_definitions(-DORCUS_DLNA_DLL)

file(GLOB DLNA_HEADER inc/*.h)
file(GLOB DLNA_SOURCE src/*.cxx)

add_library(dlna SHARED ${DLNA_HEADER} ${DLNA_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(dlna common xml2 Qt5::Core Qt5::Gui Qt5::Xml winmm ws2_32 ${LIBUPNP_LIBRARIES})
else (OMEGA_WIN32)
	target_link_libraries(dlna common xml2 Qt5::Core Qt5::Gui Qt5::Xml ${LIBUPNP_LIBRARIES})
endif (OMEGA_WIN32)

if (${TIGER_LINUX_DISTRO})
	install(TARGETS dlna DESTINATION lib)
endif (${TIGER_LINUX_DISTRO})

