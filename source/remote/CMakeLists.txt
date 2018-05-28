add_definitions(-DORCUS_REMOTE_DLL)

file(GLOB REMOTE_HEADER inc/*.h)
file(GLOB REMOTE_SOURCE src/*.cxx)

if (OMEGA_WIN32 OR OMEGA_LINUX)
	list(REMOVE_ITEM REMOTE_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/inc/AppleIRRemoteController.h")
	list(REMOVE_ITEM REMOTE_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/inc/AppleRemoteService.h")
elseif (OMEGA_MACOSX)
	file(GLOB REMOTE_OBJECTIVE_SOURCE src/*.mm)
endif (OMEGA_WIN32 OR OMEGA_LINUX)

qt5_add_resources(REMOTE_UI_RESOURCES remote.qrc)

if (OMEGA_WIN32 OR OMEGA_LINUX)
	add_library(remote SHARED ${REMOTE_HEADER} ${REMOTE_SOURCE} ${REMOTE_UI_RESOURCES})
elseif (OMEGA_MACOSX)
	add_library(remote SHARED ${REMOTE_HEADER} ${REMOTE_SOURCE} ${REMOTE_UI_RESOURCES} ${REMOTE_OBJECTIVE_SOURCE})
endif (OMEGA_WIN32 OR OMEGA_LINUX)

if (OMEGA_WIN32)
	target_link_libraries(remote common dlna network_omega xml2 qt5core qt5gui qt5xml qt5widgets winmm ws2_32)
elseif (OMEGA_MACOSX)
	target_link_libraries(remote common dlna network_omega xml2 qt5core qt5gui qt5xml qt5widgets ${COCOA_FRAMEWORK_UNIT} ${APPKIT_FRAMEWORK_UNIT} ${IOKIT_FRAMEWORK_UNIT})
elseif (OMEGA_LINUX)
	target_link_libraries(remote common dlna network_omega xml2 qt5core qt5gui qt5xml qt5widgets)
endif (OMEGA_WIN32)
