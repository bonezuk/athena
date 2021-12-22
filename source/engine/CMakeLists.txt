add_definitions(-DORCUS_ENGINE_DLL)

file(GLOB ENGINE_HEADER inc/*.h)
file(GLOB ENGINE_SOURCE src/*.cxx)

add_library(engine SHARED ${ENGINE_HEADER} ${ENGINE_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml winmm)
elseif (OMEGA_MACOSX)
	target_link_libraries(engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml)
endif (OMEGA_WIN32)

if (${TIGER_LINUX_DISTRO})
	install(TARGETS engine DESTINATION lib)
endif (${TIGER_LINUX_DISTRO})
