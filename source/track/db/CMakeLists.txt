add_definitions(-DORCUS_TRACK_DB_DLL)

file(GLOB TRACK_DB_HEADER inc/*.h)
if (OMEGA_WIN32 OR OMEGA_LINUX)
	file(GLOB TRACK_DB_SOURCE src/*.cxx src/*.c)
	list(REMOVE_ITEM TRACK_DB_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/inc/ITunesLocationMac.h" "${CMAKE_CURRENT_SOURCE_DIR}/inc/SandboxBookmarkService.h" "${CMAKE_CURRENT_SOURCE_DIR}/inc/SBBookmarkTrackDB.h")
elseif (OMEGA_MACOSX)
	file(GLOB TRACK_DB_SOURCE src/*.cxx src/*.c src/*.mm)
endif (OMEGA_WIN32 OR OMEGA_LINUX)

add_library(trackdb SHARED ${TRACK_DB_HEADER} ${TRACK_DB_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(trackdb engine common dlna network_omega trackinfo xml2 qt5core qt5gui qt5xml winmm ws2_32)
elseif (OMEGA_MACOSX OR OMEGA_LINUX)
	target_link_libraries(trackdb engine common dlna network_omega trackinfo xml2 qt5core qt5gui qt5xml)
endif (OMEGA_WIN32)
