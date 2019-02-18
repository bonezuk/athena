set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(QT_HOME "" CACHE PATH "Qt install")
message("${QT_HOME}")
set(CMAKE_PREFIX_PATH "${QT_HOME}")
message("${CMAKE_PREFIX_PATH}")

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Xml)
find_package(Qt5Widgets)
find_package(Qt5Test)

if (OMEGA_WIN32)

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

	set(QT_CORE_LIBNAME "libQt5Core")
	set(QT_GUI_LIBNAME "libQt5Gui")
	set(QT_XML_LIBNAME "libQt5Xml")
	set(QT_WIDGETS_LIBNAME "libQt5Widgets")
	set(QT_TEST_LIBNAME "libQt5Test")

endif (OMEGA_WIN32)