add_definitions(-DORCUS_TRACK_INFO_DLL)

file(GLOB TRACK_INFO_HEADER inc/*.h)
if (OMEGA_WIN32 OR OMEGA_LINUX)
	file(GLOB TRACK_INFO_SOURCE src/*.cxx src/*.c)
	list(REMOVE_ITEM TRACK_INFO_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/inc/SBBookmarkService.h" )
elseif (OMEGA_MACOSX)
	file(GLOB TRACK_INFO_SOURCE src/*.cxx src/*.c src/*.mm)
endif (OMEGA_WIN32 OR OMEGA_LINUX)


add_library(trackinfo SHARED ${TRACK_INFO_HEADER} ${TRACK_INFO_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(trackinfo engine common greenomega silveromega violetomega whiteomega cyanomega mpcdec xml2 Qt5::Core Qt5::Gui Qt5::Xml winmm ws2_32)
else (OMEGA_WIN32)
	target_link_libraries(trackinfo engine common greenomega silveromega violetomega whiteomega cyanomega mpcdec xml2 Qt5::Core Qt5::Gui Qt5::Xml)
endif (OMEGA_WIN32)

if (${TIGER_LINUX_DISTRO})
	install(TARGETS trackinfo DESTINATION lib)
endif (${TIGER_LINUX_DISTRO})
