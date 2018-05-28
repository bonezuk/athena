set( OPLAYER_HEADER
	./inc/OPlayerMain.h
)

set( OPLAYER_SOURCE
	./src/OPlayerMain.cxx
)

add_executable(oplayer ${OPLAYER_HEADER} ${OPLAYER_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(oplayer audioio blackomega blueomega common cyanomega dlna engine greenomega http mime monkeyomega toneomega wavpackomega network_omega redomega remote rtp rtp_silveromega silveromega trackdb trackinfo violetomega whiteomega widget xml2 qt5core qt5gui qt5xml qt5widgets winmm ws2_32 winsparkle wavpack)
elseif (OMEGA_MACOSX)
	target_link_libraries(oplayer audioio blackomega blueomega common cyanomega dlna engine greenomega http mime monkeyomega toneomega wavpackomega network_omega redomega remote rtp rtp_silveromega silveromega trackdb trackinfo violetomega whiteomega widget xml2 qt5core qt5gui qt5xml qt5widgets wavpack ${COCOA_FRAMEWORK_UNIT} ${APPKIT_FRAMEWORK_UNIT} ${IOKIT_FRAMEWORK_UNIT} ${SPARKLE_FRAMEWORK})
elseif (OMEGA_LINUX)
	target_link_libraries(oplayer audioio blackomega blueomega common cyanomega dlna engine greenomega http mime toneomega wavpackomega network_omega redomega remote rtp rtp_silveromega silveromega trackdb trackinfo violetomega whiteomega widget xml2 qt5core qt5gui qt5xml qt5widgets wavpack libupnp mpcdec)
endif (OMEGA_WIN32)
