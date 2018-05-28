add_definitions(-DORCUS_ENGINE_DLL)

file(GLOB ENGINE_HEADER inc/*.h)
file(GLOB ENGINE_SOURCE src/*.cxx)

add_library(engine SHARED ${ENGINE_HEADER} ${ENGINE_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(engine common xml2 qt5core qt5gui qt5xml winmm)
elseif (OMEGA_MACOSX)
	target_link_libraries(engine common xml2 qt5core qt5gui qt5xml)
endif (OMEGA_WIN32)
