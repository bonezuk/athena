# LibUPNP configuration

if (${TIGER_LINUX_DISTRO})

	set(LIBUPNP_HOME "${ATHENA_UTILS}/usr/lib")

	add_library(ixml SHARED IMPORTED)
	set_property(TARGET ixml PROPERTY IMPORTED_LOCATION "${LIBUPNP_HOME}/libixml.so" )

	add_library(libupnp SHARED IMPORTED)
	set_property(TARGET libupnp PROPERTY IMPORTED_LOCATION "${LIBUPNP_HOME}/libupnp.so" )

	set ( LIBUPNP_LIBRARIES ixml libupnp )
	
else (${TIGER_LINUX_DISTRO})

	set(LIBUPNP_VERSION "1.6.25")
	set(LIBUPNP_LIBNAME "libupnp")

	if (OMEGA_WIN32)
		set(LIBUPNP_HOME "${ATHENA_UTILS}/libupnp/libupnp-${LIBUPNP_VERSION}-${BUILD_SUFFIX}")
	else (OMEGA_WIN32)
		set(LIBUPNP_HOME "${ATHENA_UTILS}/libupnp/libupnp-${LIBUPNP_VERSION}-${TIGER_PLATFORM}")
	endif (OMEGA_WIN32)

	if (OMEGA_WIN32)
		add_definitions(-DUPNP_USE_MSVCPP)
		if (OMEGA_MSVC12)
			if (TIGER_DEBUG_BUILD)
				set(PTHREAD_LIBNAME "pthreadVC3d")
			else (TIGER_DEBUG_BUILD)
				set(PTHREAD_LIBNAME "pthreadVC3")
			endif (TIGER_DEBUG_BUILD)
		else (OMEGA_MSVC12)
			set(PTHREAD_LIBNAME "PThreadVS2")
		endif (OMEGA_MSVC12)
		set(IXML_LIBNAME "ixml")
		set(THREADUTIL_LIBNAME "threadutil")
	else (OMEGA_WIN32)
		set(IXML_LIBNAME "libixml")
		set(THREADUTIL_LIBNAME "libthreadutil")
	endif (OMEGA_WIN32)

	include_directories(AFTER "${LIBUPNP_HOME}/include" )

	add_library(ixml SHARED IMPORTED)
	if (OMEGA_WIN32)
		set_property(TARGET ixml PROPERTY IMPORTED_IMPLIB "${LIBUPNP_HOME}/lib/${IXML_LIBNAME}.lib" )
	else (OMEGA_WIN32)
		set_property(TARGET ixml PROPERTY IMPORTED_LOCATION "${LIBUPNP_HOME}/lib/${IXML_LIBNAME}.${LIBEXT}" )
	endif (OMEGA_WIN32)

	add_library(libupnp SHARED IMPORTED)
	if (OMEGA_WIN32)
		set_property(TARGET libupnp PROPERTY IMPORTED_IMPLIB "${LIBUPNP_HOME}/lib/${LIBUPNP_LIBNAME}.lib" )
	else (OMEGA_WIN32)
		set_property(TARGET libupnp PROPERTY IMPORTED_LOCATION "${LIBUPNP_HOME}/lib/${LIBUPNP_LIBNAME}.${LIBEXT}" )
	endif (OMEGA_WIN32)

	add_library(threadutil SHARED IMPORTED)
	if (OMEGA_WIN32)
		set_property(TARGET threadutil PROPERTY IMPORTED_IMPLIB "${LIBUPNP_HOME}/lib/${THREADUTIL_LIBNAME}.lib" )
	else (OMEGA_WIN32)
		set_property(TARGET threadutil PROPERTY IMPORTED_LOCATION "${LIBUPNP_HOME}/lib/${THREADUTIL_LIBNAME}.${LIBEXT}" )
	endif (OMEGA_WIN32)

	if (OMEGA_WIN32)
		add_library(pthread SHARED IMPORTED)
		set_property(TARGET pthread PROPERTY IMPORTED_IMPLIB "${LIBUPNP_HOME}/lib/${PTHREAD_LIBNAME}.lib" )
		set ( LIBUPNP_LIBRARIES ixml libupnp threadutil pthread )
	else (OMEGA_WIN32)
		set ( LIBUPNP_LIBRARIES ixml libupnp threadutil )
	endif (OMEGA_WIN32)

	message("${LIBUPNP_HOME}/lib/${LIBUPNP_LIBNAME}.${LIBEXT}")

endif (${TIGER_LINUX_DISTRO})
