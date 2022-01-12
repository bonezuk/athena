file(GLOB PLAYLISTMANAGER_HEADER inc/*.h)
file(GLOB PLAYLISTMANAGER_SOURCE src/*.cxx)

qt5_add_resources(PLAYLISTMANAGER_RESOURCES tableview.qrc)

add_executable(OmegaPlaylistManager ${PLAYLISTMANAGER_HEADER} ${PLAYLISTMANAGER_SOURCE} ${PLAYLISTMANAGER_RESOURCES} )

if (OMEGA_WIN32)
	target_link_libraries(OmegaPlaylistManager audioio blackomega blueomega common cyanomega dlna engine greenomega http mime toneomega wavpackomega network_omega redomega remote rtp rtp_silveromega silveromega trackdb trackinfo violetomega whiteomega widget omegapicommon xml2 Qt5::Core Qt5::Gui Qt5::Quick Qt5::Qml Qt5::DBus winmm ws2_32 wavpack)
elseif (OMEGA_MACOSX)
	target_link_libraries(OmegaPlaylistManager audioio blackomega blueomega common cyanomega dlna engine greenomega http mime toneomega wavpackomega network_omega redomega remote rtp rtp_silveromega silveromega trackdb trackinfo violetomega whiteomega widget omegapicommon xml2 Qt5::Core Qt5::Gui Qt5::Quick Qt5::Qml Qt5::DBus wavpack ${COCOA_FRAMEWORK_UNIT} ${APPKIT_FRAMEWORK_UNIT} ${IOKIT_FRAMEWORK_UNIT} ${SPARKLE_FRAMEWORK})
elseif (OMEGA_LINUX)
	target_link_libraries(OmegaPlaylistManager audioio blackomega blueomega common cyanomega dlna engine greenomega http mime toneomega wavpackomega network_omega redomega remote rtp rtp_silveromega silveromega trackdb trackinfo violetomega whiteomega widget omegapicommon xml2 Qt5::Core Qt5::Gui Qt5::Quick Qt5::Qml Qt5::DBus wavpack mpcdec ${LIBUPNP_LIBRARIES} )
endif (OMEGA_WIN32)

if (${TIGER_LINUX_DISTRO})
	install(TARGETS OmegaPlaylistManager DESTINATION bin)
endif (${TIGER_LINUX_DISTRO})
