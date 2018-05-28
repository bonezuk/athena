add_definitions(-DORCUS_ENGINE_REDOMEGA_DLL)

file(GLOB REDOMEGA_ENGINE_HEADER inc/*.h)
file(GLOB REDOMEGA_ENGINE_SOURCE src/*.cxx)

add_library(redomega SHARED ${REDOMEGA_ENGINE_HEADER} ${REDOMEGA_ENGINE_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(redomega engine common xml2 qt5core qt5gui qt5xml winmm)
elseif (OMEGA_MACOSX)
	target_link_libraries(redomega engine common xml2 qt5core qt5gui qt5xml)
endif (OMEGA_WIN32)
