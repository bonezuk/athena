add_definitions(-DORCUS_COMMON_DLL)

if (OMEGA_WIN32)
	add_definitions( /wd4910 )
endif (OMEGA_WIN32)

file(GLOB COMMON_HEADER inc/*.h)
file(GLOB COMMON_SOURCE src/*.cxx)
if (OMEGA_WIN32 OR OMEGA_LINUX)
	list(REMOVE_ITEM COMMON_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/inc/Bundle.h")
elseif (OMEGA_MACOSX)
	file(GLOB COMMON_OBJECTIVE_SOURCE ./src/*.mm)
	list(APPEND COMMON_SOURCE ${COMMON_OBJECTIVE_SOURCE})
endif (OMEGA_WIN32 OR OMEGA_LINUX)

add_library(common SHARED ${COMMON_HEADER} ${COMMON_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(common xml2 Qt5::Core Qt5::Gui Qt5::Xml wbemuuid iphlpapi winmm)
elseif (OMEGA_MACOSX)
	target_link_libraries(common xml2 Qt5::Core Qt5::Gui Qt5::Xml ${COCOA_FRAMEWORK_UNIT} ${FOUNDATION_FRAMEWORK_UNIT})
elseif (OMEGA_LINUX)
	target_link_libraries(common xml2 Qt5::Core Qt5::Gui Qt5::Xml)
endif (OMEGA_WIN32)
