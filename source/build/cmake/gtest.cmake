# Google Test configuration

set(GTEST_VERSION "1.7.0")
set(GTEST_LIBNAME "gtest")
set(GTEST_MAIN_LIBNAME "gtest_main")
if (OMEGA_WIN32)
	set(GTEST_HOME "${ATHENA_UTILS}/gtest/gtest-${GTEST_VERSION}-${BUILD_SUFFIX}")
else (OMEGA_WIN32)
	set(GTEST_HOME "${ATHENA_UTILS}/gtest/gtest-${GTEST_VERSION}-${TIGER_PLATFORM}")
endif (OMEGA_WIN32)

include_directories(AFTER "${GTEST_HOME}/include" )

add_library(gtest SHARED IMPORTED)
if (OMEGA_WIN32)
	set_property(TARGET gtest PROPERTY IMPORTED_LOCATION "${GTEST_HOME}/lib/${GTEST_LIBNAME}.${LIBEXT}" )
	set_property(TARGET gtest PROPERTY IMPORTED_IMPLIB "${GTEST_HOME}/lib/${GTEST_LIBNAME}.lib" )
else (OMEGA_WIN32)
	set_property(TARGET gtest PROPERTY IMPORTED_LOCATION "${GTEST_HOME}/lib/lib${GTEST_LIBNAME}.${LIBEXT}" )
endif (OMEGA_WIN32)

add_library(gtest_main SHARED IMPORTED)
if (OMEGA_WIN32)
	set_property(TARGET gtest_main PROPERTY IMPORTED_LOCATION "${GTEST_HOME}/lib/${GTEST_MAIN_LIBNAME}.${LIBEXT}" )
	set_property(TARGET gtest_main PROPERTY IMPORTED_IMPLIB "${GTEST_HOME}/lib/${GTEST_MAIN_LIBNAME}.lib" )
else (OMEGA_WIN32)
	set_property(TARGET gtest_main PROPERTY IMPORTED_LOCATION "${GTEST_HOME}/lib/lib${GTEST_MAIN_LIBNAME}.${LIBEXT}" )
endif (OMEGA_WIN32)

message("${GTEST_HOME}/lib/lib${GTEST_LIBNAME}.${LIBEXT}")
