add_definitions(-DORCUS_WIDGET_DLL)

file(GLOB WIDGET_HEADER inc/*.h)
if (OMEGA_WIN32 OR OMEGA_LINUX)
	file(GLOB WIDGET_SOURCE src/*.cxx)
	list(REMOVE_ITEM WIDGET_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/inc/ImportPlaylistDialog.h" "${CMAKE_CURRENT_SOURCE_DIR}/inc/SandBoxMac.h")
	qt5_wrap_ui(WIDGET_UI_HEADERS )
elseif (OMEGA_MACOSX)
	file(GLOB WIDGET_SOURCE src/*.cxx src/*.mm)
	list(REMOVE_ITEM WIDGET_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/inc/SandBoxWindows.h")
	list(REMOVE_ITEM WIDGET_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/SandBoxWindows.cxx")
	qt5_wrap_ui(WIDGET_UI_HEADERS inc/ImportPlaylistDialog.ui)
endif (OMEGA_WIN32 OR OMEGA_LINUX)

qt5_add_resources(WIDGET_UI_RESOURCES widget.qrc)

add_library(widget SHARED ${WIDGET_HEADER} ${WIDGET_SOURCE} ${WIDGET_UI_HEADERS} ${WIDGET_UI_RESOURCES})

if (OMEGA_WIN32)
	target_link_libraries(widget common http mime network_omega xml2 Qt5::Core Qt5::Gui Qt5::Xml Qt5::Widgets winmm ws2_32)
elseif (OMEGA_MACOSX OR OMEGA_LINUX)
	target_link_libraries(widget common http mime network_omega trackdb xml2 Qt5::Core Qt5::Gui Qt5::Xml Qt5::Widgets)
endif (OMEGA_WIN32)
