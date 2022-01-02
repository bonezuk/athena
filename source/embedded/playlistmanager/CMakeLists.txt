file(GLOB PLAYLISTMANAGER_HEADER inc/*.h)
file(GLOB PLAYLISTMANAGER_SOURCE src/*.cxx)

qt5_add_resources(PLAYLISTMANAGER_RESOURCES tableview.qrc)

add_executable(OmegaPlaylistManager ${PLAYLISTMANAGER_HEADER} ${PLAYLISTMANAGER_SOURCE} ${PLAYLISTMANAGER_RESOURCES} )

target_link_libraries(OmegaPlaylistManager Qt5::Core Qt5::Gui Qt5::Quick Qt5::Qml)

if (${TIGER_LINUX_DISTRO})
	install(TARGETS OmegaPlaylistManager DESTINATION bin)
endif (${TIGER_LINUX_DISTRO})
