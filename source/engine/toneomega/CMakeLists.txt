add_definitions(-DORCUS_ENGINE_TONEOMEGA_DLL)

file(GLOB TONEOMEGA_ENGINE_HEADER inc/*.h)
file(GLOB TONEOMEGA_ENGINE_SOURCE src/*.cxx)

add_library(toneomega SHARED ${TONEOMEGA_ENGINE_HEADER} ${TONEOMEGA_ENGINE_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(toneomega engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml winmm)
else (OMEGA_WIN32)
	target_link_libraries(toneomega engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml)
endif (OMEGA_WIN32)

if (${TIGER_LINUX_DISTRO})
	install(TARGETS toneomega DESTINATION lib)
endif (${TIGER_LINUX_DISTRO})
