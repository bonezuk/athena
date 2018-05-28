add_definitions(-DORCUS_FTP_DLL)

file(GLOB NETWORK_FTP_HEADER inc/*.h)
file(GLOB NETWORK_FTP_SOURCE src/*.cxx)

add_library(ftp SHARED ${NETWORK_FTP_HEADER} ${NETWORK_FTP_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(ftp network_omega common xml2 qt5core qt5gui qt5xml winmm ws2_32)
elseif (OMEGA_MACOSX)
	target_link_libraries(ftp network_omega common xml2 qt5core qt5gui qt5xml)
endif (OMEGA_WIN32)
