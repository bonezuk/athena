add_definitions(-DORCUS_ENGINE_WAVPACKOMEGA_DLL)

file(GLOB WAVPACKOMEGA_ENGINE_HEADER inc/*.h)
file(GLOB WAVPACKOMEGA_ENGINE_SOURCE src/*.cxx)

add_library(wavpackomega SHARED ${WAVPACKOMEGA_ENGINE_HEADER} ${WAVPACKOMEGA_ENGINE_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(wavpackomega engine common xml2 qt5core qt5gui qt5xml winmm wavpack)
elseif (OMEGA_MACOSX)
	target_link_libraries(wavpackomega engine common xml2 qt5core qt5gui qt5xml wavpack)
endif (OMEGA_WIN32)
