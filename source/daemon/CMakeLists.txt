add_definitions(-DORCUS_DAEMON_DLL)

file(GLOB DAEMON_HEADER inc/*.h)
file(GLOB DAEMON_SOURCE src/*.cxx)

add_library(daemon SHARED ${DAEMON_HEADER} ${DAEMON_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(daemon audioio blackomega blueomega common cyanomega dlna engine greenomega http mime toneomega wavpackomega network_omega redomega remote rtp rtp_silveromega silveromega trackdb trackinfo violetomega whiteomega widget xml2 Qt5::Core Qt5::Gui Qt5::Xml Qt5::Widgets winmm ws2_32 wavpack)
elseif (OMEGA_MACOSX)
	target_link_libraries(daemon audioio blackomega blueomega common cyanomega dlna engine greenomega http mime toneomega wavpackomega network_omega redomega remote rtp rtp_silveromega silveromega trackdb trackinfo violetomega whiteomega widget xml2 Qt5::Core Qt5::Gui Qt5::Xml Qt5::Widgets wavpack ${COCOA_FRAMEWORK_UNIT} ${APPKIT_FRAMEWORK_UNIT} ${IOKIT_FRAMEWORK_UNIT})
elseif (OMEGA_LINUX)
	target_link_libraries(daemon audioio blackomega blueomega common cyanomega dlna engine greenomega http mime toneomega wavpackomega network_omega redomega remote rtp rtp_silveromega silveromega trackdb trackinfo violetomega whiteomega widget xml2 Qt5::Core Qt5::Gui Qt5::Xml Qt5::Widgets wavpack libupnp mpcdec)
endif (OMEGA_WIN32)
