add_definitions(-DORCUS_ENGINE_MONKEYOMEGA_DLL)

file(GLOB MONKEYOMEGA_ENGINE_HEADER inc/*.h)
file(GLOB MONKEYOMEGA_ENGINE_SOURCE src/*.cxx)

add_library(monkeyomega SHARED ${MONKEYOMEGA_ENGINE_HEADER} ${MONKEYOMEGA_ENGINE_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(monkeyomega engine common monkey xml2 qt5core qt5gui qt5xml winmm)
elseif (OMEGA_MACOSX)
	target_link_libraries(monkeyomega engine common monkey xml2 qt5core qt5gui qt5xml)
endif (OMEGA_WIN32)
