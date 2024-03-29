cmake_minimum_required(VERSION 3.10)

if (POLICY CMP0020)
	cmake_policy(SET CMP0020 NEW)
endif (POLICY CMP0020)

project(BlackOmega)

set(ROOT_PROJECT_PATH "${CMAKE_SOURCE_DIR}")
set(CMAKE_MODULE_PATH "${ROOT_PROJECT_PATH}/source/build/cmake")

include(OmegaEnviroment)

set(TIGER_ROOT_DIRECTORY "${PROJECT_SOURCE_DIR}/Build/Omega")

if (${CMAKE_GENERATOR} MATCHES "NMake Makefiles")
	set(BINARY_OUTPUT_DIRECTORY "bin")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${TIGER_ROOT_DIRECTORY}/${BINARY_OUTPUT_DIRECTORY}")
elseif (${CMAKE_GENERATOR} MATCHES "Unix Makefiles")
	set(BINARY_OUTPUT_DIRECTORY "lib")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${TIGER_ROOT_DIRECTORY}/${BINARY_OUTPUT_DIRECTORY}")
else (${CMAKE_GENERATOR} MATCHES "NMake Makefiles")
	set(BINARY_OUTPUT_DIRECTORY "${CMAKE_BUILD_TYPE}")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${TIGER_ROOT_DIRECTORY}")
endif (${CMAKE_GENERATOR} MATCHES "NMake Makefiles")

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
set(LIBRARY_OUTPUT_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

message(${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
message(${EXECUTABLE_OUTPUT_PATH})
message(${LIBRARY_OUTPUT_PATH})

include(Qt6)
include(libxml2)
include(libupnp)
include(musepack)
include(wavpack)
include(gmock)

if (OMEGA_WIN32)
	include(ThirdPartyWindows)
	include(winsdk)
elseif (OMEGA_MACOSX)
	include(ThirdPartyMacOSX)
elseif (OMEGA_LINUX)
	include(ThirdPartyLinux)
endif (OMEGA_WIN32)

add_subdirectory(source)
