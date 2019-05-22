set( OPLAYER_HEADER
	./inc/OPlayerMain.h
)

set( OPLAYER_SOURCE
	./src/OPlayerMain.cxx
)

add_executable(oplayer ${OPLAYER_HEADER} ${OPLAYER_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(oplayer ${OMEGA_LIBRARIES} Qt5::Core Qt5::Gui Qt5::Xml Qt5::Widgets winmm ws2_32)
elseif (OMEGA_MACOSX)
	target_link_libraries(oplayer ${OMEGA_LIBRARIES} Qt5::Core Qt5::Gui Qt5::Xml Qt5::Widgets ${COCOA_FRAMEWORK_UNIT} ${APPKIT_FRAMEWORK_UNIT} ${IOKIT_FRAMEWORK_UNIT} ${SPARKLE_FRAMEWORK})
elseif (OMEGA_LINUX)
	target_link_libraries(oplayer ${OMEGA_LIBRARIES} Qt5::Core Qt5::Gui Qt5::Xml Qt5::Widgets libupnp mpcdec)
endif (OMEGA_WIN32)
