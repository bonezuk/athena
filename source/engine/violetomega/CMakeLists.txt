add_definitions(-DORCUS_ENGINE_VIOLETOMEGA_DLL)

file(GLOB VIOLETOMEGA_ENGINE_HEADER inc/*.h)
file(GLOB VIOLETOMEGA_ENGINE_SOURCE src/*.cxx)

add_library(violetomega SHARED ${VIOLETOMEGA_ENGINE_HEADER} ${VIOLETOMEGA_ENGINE_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(violetomega engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml winmm)
elseif (OMEGA_MACOSX)
	target_link_libraries(violetomega engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml)
endif (OMEGA_WIN32)

if (${TIGER_LINUX_DISTRO})
	install(TARGETS violetomega DESTINATION lib)
endif (${TIGER_LINUX_DISTRO})
