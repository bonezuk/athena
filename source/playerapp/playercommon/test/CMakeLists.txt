set ( IPCSOCKCOMMSTEST_PROG_SOURCE ./IPCSocketCommsTest_ProgA.cxx )

add_executable( ipcsocketcomms_test_prog_a ${IPCSOCKCOMMSTEST_PROG_SOURCE} )
if (OMEGA_WIN32)
	target_link_libraries(ipcsocketcomms_test_prog_a playercommon engine common blackomega silveromega whiteomega network_omega trackdb trackinfo xml2 ${QTVER}::Core ${QTVER}::Gui ${QTVER}::Quick ${QTVER}::Qml winmm ws2_32 wavpack)
elseif (OMEGA_MACOSX)
	target_link_libraries(ipcsocketcomms_test_prog_a playercommon engine common blackomega silveromega whiteomega network_omega trackdb trackinfo xml2 ${QTVER}::Core ${QTVER}::Gui ${QTVER}::Quick ${QTVER}::Qml wavpack ${COCOA_FRAMEWORK_UNIT} ${APPKIT_FRAMEWORK_UNIT} ${IOKIT_FRAMEWORK_UNIT} ${SPARKLE_FRAMEWORK})
elseif (OMEGA_LINUX)
	target_link_libraries(ipcsocketcomms_test_prog_a playercommon engine common blackomega silveromega whiteomega network_omega trackdb trackinfo xml2 ${QTVER}::Core ${QTVER}::Gui ${QTVER}::Quick ${QTVER}::Qml wavpack mpcdec ${LIBUPNP_LIBRARIES} )
endif (OMEGA_WIN32)

if (${TIGER_LINUX_DISTRO})
	install(TARGETS ipcsocketcomms_test_prog_a DESTINATION bin)
endif (${TIGER_LINUX_DISTRO})


set ( IPCSERVICETEST_PROG_SOURCE ./IPCServiceTest_ProgB.cxx  ./IPCServiceTest_ProgB.H)

add_executable( ipcservice_test_prog_b ${IPCSERVICETEST_PROG_SOURCE} )
if (OMEGA_WIN32)
	target_link_libraries(ipcservice_test_prog_b playercommon engine common blackomega silveromega whiteomega network_omega trackdb trackinfo xml2 ${QTVER}::Core ${QTVER}::Gui ${QTVER}::Quick ${QTVER}::Qml winmm ws2_32 wavpack)
elseif (OMEGA_MACOSX)
	target_link_libraries(ipcservice_test_prog_b playercommon engine common blackomega silveromega whiteomega network_omega trackdb trackinfo xml2 ${QTVER}::Core ${QTVER}::Gui ${QTVER}::Quick ${QTVER}::Qml wavpack ${COCOA_FRAMEWORK_UNIT} ${APPKIT_FRAMEWORK_UNIT} ${IOKIT_FRAMEWORK_UNIT} ${SPARKLE_FRAMEWORK})
elseif (OMEGA_LINUX)
	target_link_libraries(ipcservice_test_prog_b playercommon engine common blackomega silveromega whiteomega network_omega trackdb trackinfo xml2 ${QTVER}::Core ${QTVER}::Gui ${QTVER}::Quick ${QTVER}::Qml wavpack mpcdec ${LIBUPNP_LIBRARIES} )
endif (OMEGA_WIN32)

if (${TIGER_LINUX_DISTRO})
	install(TARGETS ipcservice_test_prog_b DESTINATION bin)
endif (${TIGER_LINUX_DISTRO})
