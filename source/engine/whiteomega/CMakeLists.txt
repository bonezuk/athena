add_definitions(-DORCUS_ENGINE_WHITEOMEGA_DLL)

file(GLOB WHITEOMEGA_ENGINE_HEADER inc/*.h)
file(GLOB WHITEOMEGA_ENGINE_SOURCE src/*.cxx)

add_library(whiteomega SHARED ${WHITEOMEGA_ENGINE_HEADER} ${WHITEOMEGA_ENGINE_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(whiteomega engine common redomega xml2 qt5core qt5gui qt5xml winmm)
elseif (OMEGA_MACOSX)
	target_link_libraries(whiteomega engine common redomega xml2 qt5core qt5gui qt5xml)
endif (OMEGA_WIN32)
