add_definitions(-DORCUS_ENGINE_CYANOMEGA_DLL)

file(GLOB CYANOMEGA_ENGINE_HEADER inc/*.h)
file(GLOB CYANOMEGA_ENGINE_SOURCE src/*.cxx)

add_library(cyanomega SHARED ${CYANOMEGA_ENGINE_HEADER} ${CYANOMEGA_ENGINE_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(cyanomega engine common xml2 mpcdec Qt5::Core Qt5::Gui Qt5::Xml winmm)
elseif (OMEGA_MACOSX)
	target_link_libraries(cyanomega engine common xml2 mpcdec Qt5::Core Qt5::Gui Qt5::Xml)
endif (OMEGA_WIN32)

if (${TIGER_LINUX_DISTRO})
	install(TARGETS cyanomega DESTINATION lib)
endif (${TIGER_LINUX_DISTRO})
