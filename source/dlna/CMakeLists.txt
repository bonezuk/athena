add_definitions(-DORCUS_DLNA_DLL)

file(GLOB DLNA_HEADER inc/*.h)
file(GLOB DLNA_SOURCE src/*.cxx)

add_library(dlna SHARED ${DLNA_HEADER} ${DLNA_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(dlna common xml2 Qt5::Core Qt5::Gui Qt5::Xml winmm ws2_32 ixml libupnp threadutil pthread)
elseif (OMEGA_MACOSX)
	target_link_libraries(dlna common xml2 Qt5::Core Qt5::Gui Qt5::Xml ixml libupnp threadutil)
endif (OMEGA_WIN32)
