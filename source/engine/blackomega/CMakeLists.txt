add_definitions(-DORCUS_ENGINE_BLACKOMEGA_DLL)

file(GLOB BLACKOMEGA_ENGINE_HEADER inc/*.h)
file(GLOB BLACKOMEGA_ENGINE_SOURCE src/*.cxx)

add_library(blackomega SHARED ${BLACKOMEGA_ENGINE_HEADER} ${BLACKOMEGA_ENGINE_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(blackomega engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml winmm)
elseif (OMEGA_MACOSX)
	target_link_libraries(blackomega engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml)
endif (OMEGA_WIN32)

if (${TIGER_LINUX_DISTRO})
	install(TARGETS blackomega DESTINATION lib)
endif (${TIGER_LINUX_DISTRO})
