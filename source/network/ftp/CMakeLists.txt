add_definitions(-DORCUS_FTP_DLL)

file(GLOB NETWORK_FTP_HEADER inc/*.h)
file(GLOB NETWORK_FTP_SOURCE src/*.cxx)

add_library(ftp SHARED ${NETWORK_FTP_HEADER} ${NETWORK_FTP_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(ftp network_omega common xml2 Qt5::Core Qt5::Gui Qt5::Xml winmm ws2_32)
elseif (OMEGA_MACOSX)
	target_link_libraries(ftp network_omega common xml2 Qt5::Core Qt5::Gui Qt5::Xml)
endif (OMEGA_WIN32)

if (${TIGER_LINUX_DISTRO})
	install(TARGETS ftp DESTINATION lib)
endif (${TIGER_LINUX_DISTRO})
