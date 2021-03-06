set(TIGER_INSTALL "${TIGER_ROOT_DIRECTORY}")

if (${CMAKE_GENERATOR} MATCHES "NMake Makefiles")
	set(TIGER_BIN_INSTALL "${TIGER_ROOT_DIRECTORY}/${BINARY_OUTPUT_DIRECTORY}")
else (${CMAKE_GENERATOR} MATCHES "NMake Makefiles")
	if (${TIGER_DEBUG_BUILD})
		set(TIGER_BIN_INSTALL "${TIGER_ROOT_DIRECTORY}/Debug")
	else (${TIGER_DEBUG_BUILD})
		set(TIGER_BIN_INSTALL "${TIGER_ROOT_DIRECTORY}/Release")
	endif (${TIGER_DEBUG_BUILD})
endif (${CMAKE_GENERATOR} MATCHES "NMake Makefiles")

message("Build directory structure")
message("${TIGER_BIN_INSTALL}")

file(MAKE_DIRECTORY "${TIGER_INSTALL}")
file(MAKE_DIRECTORY "${TIGER_BIN_INSTALL}")
file(MAKE_DIRECTORY "${TIGER_INSTALL}/plugins")
file(MAKE_DIRECTORY "${TIGER_INSTALL}/plugins/platforms")
file(MAKE_DIRECTORY "${TIGER_INSTALL}/plugins/imageformats")
file(MAKE_DIRECTORY "${TIGER_INSTALL}/plugins/sqldrivers")
file(MAKE_DIRECTORY "${TIGER_INSTALL}/data")
file(MAKE_DIRECTORY "${TIGER_INSTALL}/data/license")
file(MAKE_DIRECTORY "${TIGER_INSTALL}/help")

if (OMEGA_MSVC12)
	message("Copy Visual C++ 2013 CRT libraries")
	if (OMEGA_IS64BIT)
		set(REDIST_DIR "win64")
	else (OMEGS_IS64BIT)
		set(REDIST_DIR "win32")
	endif (OMEGA_IS64BIT)
	set(REDIST_LIB_DIR "${ATHENA_UTILS}/redist/${REDIST_DIR}/${CMAKE_BUILD_TYPE}/*")
	message("${REDIST_LIB_DIR}")
	file(GLOB MSVC_LIBRARY_FILES "${REDIST_LIB_DIR}/*")
	file(COPY ${MSVC_LIBRARY_FILES} DESTINATION ${TIGER_BIN_INSTALL})
endif (OMEGA_MSVC12)

message("Copy QtCore")
file(GLOB QTCORE_LIBRARY_FILES "${QT_HOME}/bin/${QT_CORE_LIBNAME}.*")
file(COPY ${QTCORE_LIBRARY_FILES} DESTINATION ${TIGER_BIN_INSTALL})

message("Copy QtGui")
file(GLOB QTGUI_LIBRARY_FILES "${QT_HOME}/bin/${QT_GUI_LIBNAME}.*")
file(COPY ${QTGUI_LIBRARY_FILES} DESTINATION ${TIGER_BIN_INSTALL})

message("Copy QtXml")
file(GLOB QTXML_LIBRARY_FILES "${QT_HOME}/bin/${QT_XML_LIBNAME}.*")
file(COPY ${QTXML_LIBRARY_FILES} DESTINATION ${TIGER_BIN_INSTALL})

message("Copy QtWidgets")
file(GLOB QTWIDGETS_LIBRARY_FILES "${QT_HOME}/bin/${QT_WIDGETS_LIBNAME}.*")
file(COPY ${QTWIDGETS_LIBRARY_FILES} DESTINATION ${TIGER_BIN_INSTALL})

message("Copy QtTest")
file(GLOB QTTEST_LIBRARY_FILES "${QT_HOME}/bin/${QT_TEST_LIBNAME}.*")
file(COPY ${QTTEST_LIBRARY_FILES} DESTINATION ${TIGER_BIN_INSTALL})

message("Copy Assistant")
file(COPY "${QT_HOME}/bin/Qt5PrintSupport.dll" DESTINATION ${TIGER_BIN_INSTALL})
file(COPY "${QT_HOME}/bin/Qt5Help.dll" DESTINATION ${TIGER_BIN_INSTALL})
file(COPY "${QT_HOME}/bin/Qt5Core.dll" DESTINATION ${TIGER_BIN_INSTALL})
file(COPY "${QT_HOME}/bin/Qt5Gui.dll" DESTINATION ${TIGER_BIN_INSTALL})
file(COPY "${QT_HOME}/bin/Qt5Widgets.dll" DESTINATION ${TIGER_BIN_INSTALL})
file(COPY "${QT_HOME}/bin/Qt5Sql.dll" DESTINATION ${TIGER_BIN_INSTALL})
file(COPY "${QT_HOME}/bin/Qt5Network.dll" DESTINATION ${TIGER_BIN_INSTALL})
file(COPY "${QT_HOME}/bin/assistant.exe" DESTINATION ${TIGER_BIN_INSTALL})

message("Copy Qt image formats")
if (TIGER_DEBUG_BUILD)
	set(QTPLUGIN_PLATFORM_FILES "qwindowsd.dll" "qwindows.dll")
	set(QTPLUGIN_IMAGE_FILES "qjpegd.dll" "qgifd.dll" )
else (TIGER_DEBUG_BUILD)
	set(QTPLUGIN_PLATFORM_FILES "qwindows.dll")
	set(QTPLUGIN_IMAGE_FILES "qjpeg.dll" "qgif.dll" )
endif (TIGER_DEBUG_BUILD)

foreach (QTPLUGIN_PLATFORM_FILE ${QTPLUGIN_PLATFORM_FILES})
	file(COPY "${QT_HOME}/plugins/platforms/${QTPLUGIN_PLATFORM_FILE}" DESTINATION "${TIGER_INSTALL}/plugins/platforms")
endforeach (QTPLUGIN_PLATFORM_FILE)

foreach (QTPLUGIN_IMAGE_FILE ${QTPLUGIN_IMAGE_FILES})
	file(COPY "${QT_HOME}/plugins/imageformats/${QTPLUGIN_IMAGE_FILE}" DESTINATION "${TIGER_INSTALL}/plugins/imageformats")
endforeach (QTPLUGIN_IMAGE_FILE)

file(COPY "${QT_HOME}/plugins/sqldrivers/qsqlite.dll" DESTINATION "${TIGER_INSTALL}/plugins/sqldrivers")

message("Copy libxml2")
file(COPY "${LIBXML_HOME}/lib/${LIBXML_LIBNAME}.${LIBEXT}" DESTINATION ${TIGER_BIN_INSTALL})

message("Copy UPnP")
file(COPY "${LIBUPNP_HOME}/bin/${LIBUPNP_LIBNAME}.dll" DESTINATION ${TIGER_BIN_INSTALL})
file(COPY "${LIBUPNP_HOME}/bin/${PTHREAD_LIBNAME}.dll" DESTINATION ${TIGER_BIN_INSTALL})

message("Copy Musepack Decoder")
file(COPY "${MUSEPACK_HOME}/lib/${MUSEPACK_LIBNAME}.${LIBEXT}" DESTINATION ${TIGER_BIN_INSTALL})

message("Copy WinPack")
file(COPY "${WAVPACK_HOME}/lib/${WAVPACK_LIBNAME}.${LIBEXT}" DESTINATION ${TIGER_BIN_INSTALL})

message("Copy GTest")
file(COPY "${GTEST_HOME}/lib/${GTEST_LIBNAME}.${LIBEXT}" DESTINATION ${TIGER_BIN_INSTALL})
file(COPY "${GTEST_HOME}/lib/${GTEST_MAIN_LIBNAME}.${LIBEXT}" DESTINATION ${TIGER_BIN_INSTALL})

message("Copy GMock")
file(COPY "${GMOCK_HOME}/lib/${GMOCK_LIBNAME}.${LIBEXT}" DESTINATION ${TIGER_BIN_INSTALL})
file(COPY "${GMOCK_HOME}/lib/${GMOCK_MAIN_LIBNAME}.${LIBEXT}" DESTINATION ${TIGER_BIN_INSTALL})

message("Copy Icon Resource")
file(COPY "${CMAKE_SOURCE_DIR}/source/player/omega.ico" DESTINATION "${TIGER_BIN_INSTALL}")

message("Copy qt.conf")
file(COPY "${CMAKE_SOURCE_DIR}/source/player/platform/windows/etc/qt.conf" DESTINATION "${TIGER_BIN_INSTALL}")
