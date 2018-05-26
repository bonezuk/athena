# WinSparkle configuration

if (OMEGA_MSVC12)
	set(WINSPARKLE_VERSION "0.4")
else (OMEGA_MSVC12)
	set(WINSPARKLE_VERSION "0.3")
endif (OMEGA_MSVC12)

set(WINSPARKLE_LIBNAME "WinSparkle")
set(WINSPARKLE_HOME "${TIGER_UTILS}/winsparkle/winsparkle-${WINSPARKLE_VERSION}-${BUILD_SUFFIX}")

include_directories(AFTER "${WINSPARKLE_HOME}/inc" )

add_library(winsparkle SHARED IMPORTED)
set_property(TARGET winsparkle PROPERTY IMPORTED_LOCATION "${WINSPARKLE_HOME}/lib/${WINSPARKLE_LIBNAME}.${LIBEXT}" )
set_property(TARGET winsparkle PROPERTY IMPORTED_IMPLIB "${WINSPARKLE_HOME}/lib/${WINSPARKLE_LIBNAME}.lib" )
