add_definitions(-DORCUS_PLAYERCOMMON_DLL)

file(GLOB PLAYERCOMMON_HEADER inc/*.h)
file(GLOB PLAYERCOMMON_SOURCE src/*.cxx)

qt5_add_resources(PLAYERCOMMON_RESOURCES playercommon.qrc)

add_library(playercommon SHARED ${PLAYERCOMMON_HEADER} ${PLAYERCOMMON_SOURCE} ${PLAYERCOMMON_RESOURCES})

if (OMEGA_WIN32)
	target_link_libraries(playercommon audioio blackomega blueomega common cyanomega dlna engine greenomega http mime toneomega wavpackomega network_omega redomega remote rtp rtp_silveromega silveromega trackdb trackinfo violetomega whiteomega widget xml2 Qt5::Core Qt5::Gui Qt5::Quick Qt5::Qml winmm ws2_32 wavpack)
elseif (OMEGA_MACOSX)
	target_link_libraries(playercommon audioio blackomega blueomega common cyanomega dlna engine greenomega http mime toneomega wavpackomega network_omega redomega remote rtp rtp_silveromega silveromega trackdb trackinfo violetomega whiteomega widget xml2 Qt5::Core Qt5::Gui Qt5::Quick Qt5::Qml wavpack ${COCOA_FRAMEWORK_UNIT} ${APPKIT_FRAMEWORK_UNIT} ${IOKIT_FRAMEWORK_UNIT} ${SPARKLE_FRAMEWORK})
elseif (OMEGA_LINUX)
	target_link_libraries(playercommon audioio blackomega blueomega common cyanomega dlna engine greenomega http mime toneomega wavpackomega network_omega redomega remote rtp rtp_silveromega silveromega trackdb trackinfo violetomega whiteomega widget xml2 Qt5::Core Qt5::Gui Qt5::Quick Qt5::Qml wavpack mpcdec ${LIBUPNP_LIBRARIES} )
endif (OMEGA_WIN32)

if (${TIGER_LINUX_DISTRO})
	install(TARGETS playercommon DESTINATION lib)
endif (${TIGER_LINUX_DISTRO})
