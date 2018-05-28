add_definitions(-DORCUS_ENGINE_SILVEROMEGA_DLL)

file(GLOB SILVEROMEGA_ENGINE_HEADER inc/*.h)
file(GLOB SILVEROMEGA_ENGINE_SOURCE src/*.cxx)

add_library(silveromega SHARED ${SILVEROMEGA_ENGINE_HEADER} ${SILVEROMEGA_ENGINE_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(silveromega engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml winmm)
elseif (OMEGA_MACOSX)
	target_link_libraries(silveromega engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml)
endif (OMEGA_WIN32)
