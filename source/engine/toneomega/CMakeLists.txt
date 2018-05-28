add_definitions(-DORCUS_ENGINE_TONEOMEGA_DLL)

file(GLOB TONEOMEGA_ENGINE_HEADER inc/*.h)
file(GLOB TONEOMEGA_ENGINE_SOURCE src/*.cxx)

add_library(toneomega SHARED ${TONEOMEGA_ENGINE_HEADER} ${TONEOMEGA_ENGINE_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(toneomega engine common xml2 qt5core qt5gui qt5xml winmm)
elseif (OMEGA_MACOSX)
	target_link_libraries(toneomega engine common xml2 qt5core qt5gui qt5xml)
endif (OMEGA_WIN32)
