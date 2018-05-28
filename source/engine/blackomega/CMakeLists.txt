add_definitions(-DORCUS_ENGINE_BLACKOMEGA_DLL)

file(GLOB BLACKOMEGA_ENGINE_HEADER inc/*.h)
file(GLOB BLACKOMEGA_ENGINE_SOURCE src/*.cxx)

add_library(blackomega SHARED ${BLACKOMEGA_ENGINE_HEADER} ${BLACKOMEGA_ENGINE_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(blackomega engine common xml2 qt5core qt5gui qt5xml winmm)
elseif (OMEGA_MACOSX)
	target_link_libraries(blackomega engine common xml2 qt5core qt5gui qt5xml)
endif (OMEGA_WIN32)
