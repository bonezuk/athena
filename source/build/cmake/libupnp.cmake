# LibUPNP configuration

if (OMEGA_WIN32)
	if (OMEGA_MSVC12)
		set(LIBUPNP_VERSION "1.6.19")
	else (OMEGA_MSVC12)
		set(LIBUPNP_VERSION "1.6.18")
	endif (OMEGA_MSVC12)
else (OMEGA_WIN32)
	set(LIBUPNP_VERSION "1.6.19")
endif (OMEGA_WIN32)

set(LIBUPNP_LIBNAME "libupnp")

set(LIBUPNP_HOME "${TIGER_UTILS}/libupnp/install/libupnp-${LIBUPNP_VERSION}-${BUILD_SUFFIX}")

if (OMEGA_WIN32)
	add_definitions(-DUPNP_USE_MSVCPP)
	if (OMEGA_MSVC12)
		set(PTHREAD_LIBNAME "pthreadVC2")
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
endif (OMEGA_WIN32)

message("${LIBUPNP_HOME}/lib/${LIBUPNP_LIBNAME}.${LIBEXT}")
