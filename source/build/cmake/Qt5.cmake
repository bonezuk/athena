# Qt5 configuration

set(QT_MAJOR_VERSION 5)
set(QT_MINOR_VERSION 10)
set(QT_REVISION_VERSION 1)

if (OMEGA_WIN32)
	
	set(QT_VERSION "${QT_MAJOR_VERSION}_${QT_MINOR_VERSION}_${QT_REVISION_VERSION}")

	set(QT_HOME "${TIGER_UTILS}/qt/install/qt-${QT_VERSION}-${TIGER_PLATFORM}.${TIGER_COMPILER}" CACHE PATH "Qt Location")
	
	message("Qt Set")
	
	if (${TIGER_DEBUG_BUILD})
		message("Qt Set Debug")
		set(QT_CORE_LIBNAME "Qt5Cored")
		set(QT_GUI_LIBNAME "Qt5Guid")
		set(QT_XML_LIBNAME "Qt5Xmld")
		set(QT_WIDGETS_LIBNAME "Qt5Widgetsd")
		set(QT_TEST_LIBNAME "Qt5Testd")
		
	else (${TIGER_DEBUG_BUILD})
		set(QT_CORE_LIBNAME "Qt5Core")
		set(QT_GUI_LIBNAME "Qt5Gui")
		set(QT_XML_LIBNAME "Qt5Xml")
		set(QT_WIDGETS_LIBNAME "Qt5Widgets")
		set(QT_TEST_LIBNAME "Qt5Test")
		
	endif (${TIGER_DEBUG_BUILD})

	if (OMEGA_MSVC12)
		if (${TIGER_DEBUG_BUILD})
			set(LIB_EGL_LIBNAME "libEGLd")
			set(LIB_GLES_LIBNAME "libGLESv2d")
		else (${TIGER_DEBUG_BUILD})
			set(LIB_EGL_LIBNAME "libEGL")
			set(LIB_GLES_LIBNAME "libGLESv2")
		endif (${TIGER_DEBUG_BUILD})
	endif (OMEGA_MSVC12)
	
elseif (OMEGA_MACOSX)

	set(QT_VERSION "${QT_MAJOR_VERSION}.${QT_MINOR_VERSION}.${QT_REVISION_VERSION}")

	set(QT_HOME "${TIGER_UTILS}/qt/install/qt-${QT_VERSION}.${TIGER_PLATFORM}" CACHE PATH "Qt Location")

	if (${TIGER_DEBUG_BUILD})
		set(QT_CORE_LIBNAME "libQt5Core_debug")
		set(QT_GUI_LIBNAME "libQt5Gui_debug")
		set(QT_XML_LIBNAME "libQt5Xml_debug")
		set(QT_WIDGETS_LIBNAME "libQt5Widgets_debug")
		set(QT_TEST_LIBNAME "libQt5Test_debug")
		
	else (${TIGER_DEBUG_BUILD})
		set(QT_CORE_LIBNAME "libQt5Core")
		set(QT_GUI_LIBNAME "libQt5Gui")
		set(QT_XML_LIBNAME "libQt5Xml")
		set(QT_WIDGETS_LIBNAME "libQt5Widgets")
		set(QT_TEST_LIBNAME "libQt5Test")
		
	endif (${TIGER_DEBUG_BUILD})

elseif (OMEGA_LINUX)

	set(QT_VERSION "${QT_MAJOR_VERSION}.${QT_MINOR_VERSION}.${QT_REVISION_VERSION}")

	if (${TIGER_DEBUG_BUILD})
		set(QT_HOME "/usr/local/qt/qt${QT_VERSION}d" CACHE PATH "Qt Location")
	else (${TIGER_DEBUG_BUILD})
		set(QT_HOME "/usr/local/qt/qt${QT_VERSION}" CACHE PATH "Qt Location")
	endif (${TIGER_DEBUG_BUILD})

	message("qt home - ${QT_HOME}")

	set(QT_CORE_LIBNAME "libQt5Core")
	set(QT_GUI_LIBNAME "libQt5Gui")
	set(QT_XML_LIBNAME "libQt5Xml")
	set(QT_WIDGETS_LIBNAME "libQt5Widgets")
	set(QT_TEST_LIBNAME "libQt5Test")

endif (OMEGA_WIN32)

set(CMAKE_PREFIX_PATH ${QT_HOME})

if (OMEGA_WIN32)
	set(CMAKE_PREFIX_PATH "C:\\Program Files (x86)\\Microsoft SDKs\\Windows\\v7.0A\\Lib" ${QT_HOME})
else (OMEGA_WIN32)
	set(CMAKE_PREFIX_PATH ${QT_HOME})
endif (OMEGA_WIN32)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Xml)
find_package(Qt5Widgets)
find_package(Qt5Test)

add_library(qt5core SHARED IMPORTED)
set_property(TARGET qt5core PROPERTY IMPORTED_LOCATION "${QT_HOME}/lib/${QT_CORE_LIBNAME}.${LIBEXT}")
if (OMEGA_WIN32)
	set_property(TARGET qt5core PROPERTY IMPORTED_IMPLIB "${QT_HOME}/lib/${QT_CORE_LIBNAME}.lib")
endif (OMEGA_WIN32)

add_library(qt5gui SHARED IMPORTED)
set_property(TARGET qt5gui PROPERTY IMPORTED_LOCATION "${QT_HOME}/lib/${QT_GUI_LIBNAME}.${LIBEXT}")
if (OMEGA_WIN32)
	set_property(TARGET qt5gui PROPERTY IMPORTED_IMPLIB "${QT_HOME}/lib/${QT_GUI_LIBNAME}.lib")
endif (OMEGA_WIN32)

add_library(qt5xml SHARED IMPORTED)
set_property(TARGET qt5xml PROPERTY IMPORTED_LOCATION "${QT_HOME}/lib/${QT_XML_LIBNAME}.${LIBEXT}")
if (OMEGA_WIN32)
	set_property(TARGET qt5xml PROPERTY IMPORTED_IMPLIB "${QT_HOME}/lib/${QT_XML_LIBNAME}.lib")
endif (OMEGA_WIN32)

add_library(qt5widgets SHARED IMPORTED)
set_property(TARGET qt5widgets PROPERTY IMPORTED_LOCATION "${QT_HOME}/lib/${QT_WIDGETS_LIBNAME}.${LIBEXT}")
if (OMEGA_WIN32)
	set_property(TARGET qt5widgets PROPERTY IMPORTED_IMPLIB "${QT_HOME}/lib/${QT_WIDGETS_LIBNAME}.lib")
endif (OMEGA_WIN32)

add_library(qt5test SHARED IMPORTED)
set_property(TARGET qt5test PROPERTY IMPORTED_LOCATION "${QT_HOME}/lib/${QT_TEST_LIBNAME}.${LIBEXT}")
if (OMEGA_WIN32)
	set_property(TARGET qt5test PROPERTY IMPORTED_IMPLIB "${QT_HOME}/lib/${QT_TEST_LIBNAME}.lib")
endif (OMEGA_WIN32)

include_directories(AFTER "${QT_HOME}/include" )
include_directories(AFTER "${QT_HOME}/include/QtCore" )
include_directories(AFTER "${QT_HOME}/include/QtGui" )
include_directories(AFTER "${QT_HOME}/include/QtXml" )
include_directories(AFTER "${QT_HOME}/include/QtWidgets" )
include_directories(AFTER "${QT_HOME}/include/QtTest" )
