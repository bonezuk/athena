add_definitions(-DORCUS_ENGINE_GREENOMEGA_DLL)

file(GLOB GREENOMEGA_ENGINE_HEADER inc/*.h)
file(GLOB GREENOMEGA_ENGINE_SOURCE src/*.cxx)

add_library(greenomega SHARED ${GREENOMEGA_ENGINE_HEADER} ${GREENOMEGA_ENGINE_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(greenomega engine common xml2 qt5core qt5gui qt5xml winmm)
elseif (OMEGA_MACOSX)
	target_link_libraries(greenomega engine common xml2 qt5core qt5gui qt5xml)
endif (OMEGA_WIN32)
