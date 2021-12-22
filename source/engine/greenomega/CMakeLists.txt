add_definitions(-DORCUS_ENGINE_GREENOMEGA_DLL)

file(GLOB GREENOMEGA_ENGINE_HEADER inc/*.h)
file(GLOB GREENOMEGA_ENGINE_SOURCE src/*.cxx)

add_library(greenomega SHARED ${GREENOMEGA_ENGINE_HEADER} ${GREENOMEGA_ENGINE_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(greenomega engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml winmm)
elseif (OMEGA_MACOSX)
	target_link_libraries(greenomega engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml)
endif (OMEGA_WIN32)

if (${TIGER_LINUX_DISTRO})
	install(TARGETS greenomega DESTINATION lib)
endif (${TIGER_LINUX_DISTRO})
