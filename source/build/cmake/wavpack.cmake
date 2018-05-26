# WavPack configuration

set(WAVPACK_VERSION "4.70.0")
set(WAVPACK_HOME "${TIGER_UTILS}/wavpack/wavpack-${WAVPACK_VERSION}-${BUILD_SUFFIX}")

include_directories(AFTER "${WAVPACK_HOME}/include" )

if (OMEGA_WIN32)
	set(WAVPACK_LIBNAME "wavpackdll")
else (OMEGA_WIN32)
	set(WAVPACK_LIBNAME "libwavpack")
endif (OMEGA_WIN32)

add_library(wavpack SHARED IMPORTED)
set_property(TARGET wavpack PROPERTY IMPORTED_LOCATION "${WAVPACK_HOME}/lib/${WAVPACK_LIBNAME}.${LIBEXT}" )
if (OMEGA_WIN32)
	set_property(TARGET wavpack PROPERTY IMPORTED_IMPLIB "${WAVPACK_HOME}/lib/${WAVPACK_LIBNAME}.lib" )
endif (OMEGA_WIN32)

message("${WAVPACK_HOME}/lib/${WAVPACK_LIBNAME}.${LIBEXT}")
