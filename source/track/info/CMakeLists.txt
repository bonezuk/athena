add_definitions(-DORCUS_TRACK_INFO_DLL)

file(GLOB TRACK_INFO_HEADER inc/*.h)
file(GLOB TRACK_INFO_SOURCE src/*.cxx)

add_library(trackinfo SHARED ${TRACK_INFO_HEADER} ${TRACK_INFO_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(trackinfo engine common greenomega silveromega violetomega whiteomega cyanomega mpcdec xml2 Qt5::Core Qt5::Gui Qt5::Xml winmm ws2_32)
elseif (OMEGA_MACOSX)
	target_link_libraries(trackinfo engine common greenomega silveromega violetomega whiteomega cyanomega mpcdec xml2 Qt5::Core Qt5::Gui Qt5::Xml)
endif (OMEGA_WIN32)
