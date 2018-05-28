add_definitions(-DORCUS_ENGINE_REDOMEGA_DLL)

file(GLOB REDOMEGA_ENGINE_HEADER inc/*.h)
file(GLOB REDOMEGA_ENGINE_SOURCE src/*.cxx)

add_library(redomega SHARED ${REDOMEGA_ENGINE_HEADER} ${REDOMEGA_ENGINE_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(redomega engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml winmm)
elseif (OMEGA_MACOSX)
	target_link_libraries(redomega engine common xml2 Qt5::Core Qt5::Gui Qt5::Xml)
endif (OMEGA_WIN32)
