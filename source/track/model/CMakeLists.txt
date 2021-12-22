add_definitions(-DORCUS_TRACK_MODEL_DLL)

file(GLOB TRACK_MODEL_HEADER inc/*.h)
file(GLOB TRACK_MODEL_SOURCE src/*.cxx)

add_library(trackmodel SHARED ${TRACK_MODEL_HEADER} ${TRACK_MODEL_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(trackmodel common engine trackdb trackinfo xml2 Qt5::Core Qt5::Gui Qt5::Xml winmm ws2_32)
elseif (OMEGA_MACOSX)
	target_link_libraries(trackmodel common engine trackdb trackinfo xml2 Qt5::Core Qt5::Gui Qt5::Xml)
endif (OMEGA_WIN32)

if (${TIGER_LINUX_DISTRO})
	install(TARGETS trackmodel DESTINATION lib)
endif (${TIGER_LINUX_DISTRO})
