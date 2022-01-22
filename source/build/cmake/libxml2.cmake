# Libxml2 configuration

if (${TIGER_LINUX_DISTRO})
	set(LIBXML_HOME "${ATHENA_UTILS}/usr/lib")
	include_directories(AFTER "${ATHENA_UTILS}/usr/include/libxml2" )

	add_library(xml2 SHARED IMPORTED)
	set_property(TARGET xml2 PROPERTY IMPORTED_LOCATION "${LIBXML_HOME}/libxml2.so" )
else (${TIGER_LINUX_DISTRO})

	set(LIBXML_VERSION "2.9.8")
	set(LIBXML_LIBNAME "libxml2")

	if (OMEGA_MSVC16)
		set(LIBICONV_LIBNAME "iconv")
	endif (OMEGA_MSVC16)

	if (OMEGA_WIN32)
		set(LIBXML_HOME "${ATHENA_UTILS}/libxml2/libxml2-${LIBXML_VERSION}-${BUILD_SUFFIX}")
	else (OMEGA_WIN32)
		set(LIBXML_HOME "${ATHENA_UTILS}/libxml2/libxml2-${LIBXML_VERSION}-${TIGER_PLATFORM}")
	endif (OMEGA_WIN32)

	if (OMEGA_WIN32)
		include_directories(AFTER "${LIBXML_HOME}/include" )
	else (OMEGA_WIN32)
		include_directories(AFTER "${LIBXML_HOME}/include/libxml2" )
	endif (OMEGA_WIN32)

	add_library(xml2 SHARED IMPORTED)
	set_property(TARGET xml2 PROPERTY IMPORTED_LOCATION "${LIBXML_HOME}/lib/${LIBXML_LIBNAME}.${LIBEXT}" )
	if (OMEGA_WIN32)
		set_property(TARGET xml2 PROPERTY IMPORTED_IMPLIB "${LIBXML_HOME}/lib/${LIBXML_LIBNAME}.lib" )
	endif (OMEGA_WIN32)

	message("${LIBXML_HOME}/lib/${LIBXML_LIBNAME}.${LIBEXT}")
endif (${TIGER_LINUX_DISTRO})
