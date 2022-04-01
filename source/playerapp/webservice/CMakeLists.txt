file(GLOB WEBSERVICE_HEADER inc/*.h)
file(GLOB WEBSERVICE_SOURCE src/*.cxx)

add_executable(OmegaWebService ${WEBSERVICE_HEADER} ${WEBSERVICE_SOURCE} )

if (OMEGA_WIN32)
	target_link_libraries(OmegaWebService engine common blackomega silveromega whiteomega network_omega http trackdb trackinfo playercommon widget xml2 ${QTVER}::Core ${QTVER}::Gui ${QTVER}::Quick ${QTVER}::Qml winmm ws2_32 wavpack)
elseif (OMEGA_MACOSX)
	target_link_libraries(OmegaWebService engine common blackomega silveromega whiteomega network_omega http trackdb trackinfo playercommon widget xml2 ${QTVER}::Core ${QTVER}::Gui ${QTVER}::Quick ${QTVER}::Qml wavpack ${COCOA_FRAMEWORK_UNIT} ${APPKIT_FRAMEWORK_UNIT} ${IOKIT_FRAMEWORK_UNIT} ${SPARKLE_FRAMEWORK})
elseif (OMEGA_LINUX)
	target_link_libraries(OmegaWebService engine common blackomega silveromega whiteomega network_omega http trackdb trackinfo playercommon widget xml2 ${QTVER}::Core ${QTVER}::Gui ${QTVER}::Quick ${QTVER}::Qml wavpack mpcdec ${LIBUPNP_LIBRARIES} )
endif (OMEGA_WIN32)

if (${TIGER_LINUX_DISTRO})
	install(TARGETS OmegaWebService DESTINATION bin)
endif (${TIGER_LINUX_DISTRO})
