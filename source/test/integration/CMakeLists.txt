set ( INTEGRATION_HEADERS
	../../remote/test/integration/WinLIRCDummyServer.h
	../../remote/test/integration/QKeyLineEditTester.h
	../../engine/violetomega/test/integration/VioletPlayback.h
	../../engine/cyanomega/test/integration/CyanPlayback.h
)

set ( INTEGRATION_SOURCE
	../../engine/test/integration/FFTRadix2Test.cxx
	../../remote/test/integration/WinLIRCDummyServer.cxx
	../../remote/test/integration/QKeyLineEditTester.cxx
	../../engine/violetomega/test/integration/VioletPlayback.cxx
	../../engine/cyanomega/test/integration/CyanPlayback.cxx
	TestIntegrationMain.cxx
)

if (OMEGA_MACOSX)
	set ( INTEGRATION_MAC_HEADER
		../../audioio/test/integration/AOCoreHALInteger.h
		../../remote/test/integration/AppleRemoteServiceIntegration.h
	)
	set ( INTEGRATION_MAC_SOURCE
		../../audioio/test/integration/AOCoreHALInteger.mm
		../../remote/test/integration/AppleRemoteServiceIntegration.mm
	)
endif (OMEGA_MACOSX)

if (OMEGA_WIN32)
	set ( INTEGRATION_WIN32_SOURCE
		../../audioio/test/integration/WasAPITest.cxx
	)
endif (OMEGA_WIN32)

if (OMEGA_LINUX)
	set ( INTEGRATION_LINUX_HEADER
		../../audioio/test/integration/ALSATest.h
	)

	set ( INTEGRATION_LINUX_SOURCE
		../../audioio/test/integration/ALSATest.cxx
	)
endif (OMEGA_LINUX)

if (OMEGA_WIN32)
	add_executable(Integration ${INTEGRATION_HEADERS} ${INTEGRATION_SOURCE} ${INTEGRATION_WIN32_SOURCE} )
elseif (OMEGA_MACOSX)
	add_executable(Integration ${INTEGRATION_HEADERS} ${INTEGRATION_SOURCE} ${INTEGRATION_MAC_HEADER} ${INTEGRATION_MAC_SOURCE} )
elseif (OMEGA_LINUX)
	add_executable(Integration ${INTEGRATION_HEADERS} ${INTEGRATION_SOURCE} ${INTEGRATION_LINUX_HEADER} ${INTEGRATION_LINUX_SOURCE})
endif (OMEGA_WIN32)

if (OMEGA_WIN32)
	target_link_libraries(Integration audioio blackomega blueomega common cyanomega dlna engine greenomega http mime wavpackomega network_omega redomega remote rtp rtp_silveromega silveromega trackdb trackinfo trackmodel violetomega whiteomega widget xml2 Qt5::Core Qt5::Gui Qt5::Xml Qt5::Widgets Qt5::Test winmm ws2_32 wavpack gtest gtest_main gmock gmock_main)
elseif (OMEGA_MACOSX)
	target_link_libraries(Integration audioio blackomega blueomega common cyanomega dlna engine greenomega http mime wavpackomega network_omega redomega remote rtp rtp_silveromega silveromega trackdb trackinfo trackmodel violetomega whiteomega widget xml2 Qt5::Core Qt5::Gui Qt5::Xml Qt5::Widgets Qt5::Test wavpack gtest gtest_main gmock gmock_main ${COCOA_FRAMEWORK_UNIT} ${APPKIT_FRAMEWORK_UNIT} ${IOKIT_FRAMEWORK_UNIT})
elseif (OMEGA_LINUX)
	target_link_libraries(Integration audioio blackomega blueomega common cyanomega dlna engine greenomega http mime wavpackomega network_omega redomega remote rtp rtp_silveromega silveromega trackdb trackinfo trackmodel violetomega whiteomega widget xml2 Qt5::Core Qt5::Gui Qt5::Xml Qt5::Widgets Qt5::Test wavpack libupnp mpcdec gtest gtest_main gmock gmock_main asound)
endif (OMEGA_WIN32)
