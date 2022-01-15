file(GLOB AUDIODAEMON_HEADER inc/*.h)
file(GLOB AUDIODAEMON_SOURCE src/*.cxx)

add_executable(OmegaAudioDaemon ${AUDIODAEMON_HEADER} ${AUDIODAEMON_SOURCE} )

if (OMEGA_WIN32)
	target_link_libraries(OmegaAudioDaemon audioio blackomega blueomega common cyanomega dlna engine greenomega http mime toneomega wavpackomega network_omega redomega remote rtp rtp_silveromega silveromega trackdb trackinfo violetomega whiteomega widget omegapicommon xml2 Qt5::Core Qt5::Gui Qt5::Quick Qt5::Qml Qt5::DBus winmm ws2_32 wavpack)
elseif (OMEGA_MACOSX)
	target_link_libraries(OmegaAudioDaemon audioio blackomega blueomega common cyanomega dlna engine greenomega http mime toneomega wavpackomega network_omega redomega remote rtp rtp_silveromega silveromega trackdb trackinfo violetomega whiteomega widget omegapicommon xml2 Qt5::Core Qt5::Gui Qt5::Quick Qt5::Qml Qt5::DBus wavpack ${COCOA_FRAMEWORK_UNIT} ${APPKIT_FRAMEWORK_UNIT} ${IOKIT_FRAMEWORK_UNIT} ${SPARKLE_FRAMEWORK})
elseif (OMEGA_LINUX)
	target_link_libraries(OmegaAudioDaemon audioio blackomega blueomega common cyanomega dlna engine greenomega http mime toneomega wavpackomega network_omega redomega remote rtp rtp_silveromega silveromega trackdb trackinfo violetomega whiteomega widget omegapicommon xml2 Qt5::Core Qt5::Gui Qt5::Quick Qt5::Qml Qt5::DBus wavpack mpcdec ${LIBUPNP_LIBRARIES} )
endif (OMEGA_WIN32)

if (${TIGER_LINUX_DISTRO})
	install(TARGETS OmegaAudioDaemon DESTINATION bin)
endif (${TIGER_LINUX_DISTRO})
