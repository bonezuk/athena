file(GLOB PLAYERIOS_HEADER inc/*.h)
file(GLOB PLAYERIOS_SOURCE src/*.cxx)

if (TIGER_QT6_FLAG)
	qt_add_resources(PLAYERIOS_RESOURCES playerios.qrc)
else (TIGER_QT6_FLAG)
	qt5_add_resources(PLAYERIOS_RESOURCES playerios.qrc)
endif (TIGER_QT6_FLAG)

set(CODEC_LIBRARIES
	blackomega 
	blueomega 
	cyanomega 
	greenomega 
	redomega 
	silveromega 
	violetomega 
	whiteomega 
	mpcdec
)

add_executable(OmegaPlayerIOS ${PLAYERIOS_HEADER} ${PLAYERIOS_SOURCE} ${PLAYERIOS_RESOURCES})

if (OMEGA_WIN32)
	target_link_libraries(OmegaPlayerIOS audioio common engine network_omega http ftp mime trackdb trackinfo remote widget ${CODEC_LIBRARIES} xml2 ${QTVER}::Core ${QTVER}::Gui ${QTVER}::Quick ${QTVER}::Qml winmm ws2_32)
elseif (OMEGA_MACOSX)
	target_link_libraries(OmegaPlayerIOS audioio common engine network_omega http ftp mime trackdb trackinfo remote widget ${CODEC_LIBRARIES} xml2 ${QTVER}::Core ${QTVER}::Gui ${QTVER}::Quick ${QTVER}::Qml ${COCOA_FRAMEWORK_UNIT} ${APPKIT_FRAMEWORK_UNIT} ${IOKIT_FRAMEWORK_UNIT})
elseif (OMEGA_LINUX)
	target_link_libraries(OmegaPlayerIOS audioio common engine network_omega http ftp mime trackdb trackinfo remote widget ${CODEC_LIBRARIES} xml2 ${QTVER}::Core ${QTVER}::Gui ${QTVER}::Quick ${QTVER}::Qml ${LIBUPNP_LIBRARIES} )
endif (OMEGA_WIN32)

if (${TIGER_LINUX_DISTRO})
	install(TARGETS OmegaPlayerIOS DESTINATION bin)
endif (${TIGER_LINUX_DISTRO})

