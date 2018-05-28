add_definitions(-DORCUS_ENGINE_BLUEOMEGA_DLL)

file(GLOB BLUEOMEGA_ENGINE_HEADER inc/*.h)
file(GLOB BLUEOMEGA_ENGINE_SOURCE src/*.cxx)

add_library(blueomega SHARED ${BLUEOMEGA_ENGINE_HEADER} ${BLUEOMEGA_ENGINE_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(blueomega engine common xml2 qt5core qt5gui qt5xml winmm)
elseif (OMEGA_MACOSX)
	target_link_libraries(blueomega engine common xml2 qt5core qt5gui qt5xml)
endif (OMEGA_WIN32)
