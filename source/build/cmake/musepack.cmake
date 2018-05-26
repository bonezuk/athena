# Musepack configuration

set(MUSEPACK_VERSION "r475")
set(MUSEPACK_HOME "${TIGER_UTILS}/musepack/musepack-${MUSEPACK_VERSION}-${BUILD_SUFFIX}")

include_directories(AFTER "${MUSEPACK_HOME}/include" )

if (OMEGA_WIN32)
	set(MUSEPACK_LIBNAME "mpcdec")
else (OMEGA_WIN32)
	set(MUSEPACK_LIBNAME "libmpcdec")
endif (OMEGA_WIN32)

add_library(mpcdec SHARED IMPORTED)
set_property(TARGET mpcdec PROPERTY IMPORTED_LOCATION "${MUSEPACK_HOME}/lib/${MUSEPACK_LIBNAME}.${LIBEXT}" )
if (OMEGA_WIN32)
	set_property(TARGET mpcdec PROPERTY IMPORTED_IMPLIB "${MUSEPACK_HOME}/lib/${MUSEPACK_LIBNAME}.lib" )
endif (OMEGA_WIN32)

message("${MUSEPACK_HOME}/lib/${MUSEPACK_LIBNAME}.${LIBEXT}")
