add_definitions(-DORCUS_TRACK_MODEL_DLL)

file(GLOB TRACK_MODEL_HEADER inc/*.h)
file(GLOB TRACK_MODEL_SOURCE src/*.cxx)

add_library(trackmodel SHARED ${TRACK_MODEL_HEADER} ${TRACK_MODEL_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(trackmodel common engine trackdb trackinfo xml2 qt5core qt5gui qt5xml winmm ws2_32)
elseif (OMEGA_MACOSX)
	target_link_libraries(trackmodel common engine trackdb trackinfo xml2 qt5core qt5gui qt5xml)
endif (OMEGA_WIN32)
