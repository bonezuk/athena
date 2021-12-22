add_definitions(-DORCUS_ENGINE_WAVPACKOMEGA_DLL)

file(GLOB WAVPACKOMEGA_ENGINE_HEADER inc/*.h)
file(GLOB WAVPACKOMEGA_ENGINE_SOURCE src/*.cxx)

add_library(wavpackomega SHARED ${WAVPACKOMEGA_ENGINE_HEADER} ${WAVPACKOMEGA_ENGINE_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(wavpackomega engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml winmm wavpack)
elseif (OMEGA_MACOSX)
	target_link_libraries(wavpackomega engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml wavpack)
endif (OMEGA_WIN32)

if (${TIGER_LINUX_DISTRO})
	install(TARGETS wavpackomega DESTINATION lib)
endif (${TIGER_LINUX_DISTRO})
