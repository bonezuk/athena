set(TIGER_INSTALL "${EXECUTABLE_OUTPUT_PATH}")
set(TIGER_BIN_INSTALL "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}")

set(EXECUTABLE_OUTPUT_PATH "${TIGER_BIN_INSTALL}")
set(LIBRARY_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}")

message("Build directory structure")
message("${TIGER_BIN_INSTALL}")

file(MAKE_DIRECTORY "${TIGER_INSTALL}")
file(MAKE_DIRECTORY "${TIGER_BIN_INSTALL}")
file(MAKE_DIRECTORY "${TIGER_BIN_INSTALL}/imageformats")
file(MAKE_DIRECTORY "${TIGER_BIN_INSTALL}/platforms")
file(MAKE_DIRECTORY "${TIGER_BIN_INSTALL}/printsupport")
file(MAKE_DIRECTORY "${TIGER_INSTALL}/Frameworks")

message("Copy Qt image formats")

function (copy_rename_plugin_files FILENAME PLUGIN_DIR)
	if (TIGER_DEBUG_BUILD)
		set(PLUGIN_FILENAME "${FILENAME}_debug")
	else (TIGER_DEBUG_BUILD)
		set(PLUGIN_FILENAME "${FILENAME}")
	endif (TIGER_DEBUG_BUILD)
	file(COPY "${QT_HOME}/plugins/${PLUGIN_DIR}/${PLUGIN_FILENAME}.${LIBEXT}" DESTINATION "${TIGER_BIN_INSTALL}/${PLUGIN_DIR}")
	if (TIGER_DEBUG_BUILD)
		file(RENAME "${TIGER_BIN_INSTALL}/${PLUGIN_DIR}/${PLUGIN_FILENAME}.${LIBEXT}" "${TIGER_BIN_INSTALL}/${PLUGIN_DIR}/${FILENAME}.${LIBEXT}")
	endif (TIGER_DEBUG_BUILD)
endfunction (copy_rename_plugin_files)

copy_rename_plugin_files("libqcocoa" "platforms")
copy_rename_plugin_files("libqjpeg" "imageformats")
copy_rename_plugin_files("libqgif" "imageformats")
copy_rename_plugin_files("libcocoaprintersupport" "printsupport")

message("Copy Monkey Audio")
file(COPY "${MONKEYAUDIO_HOME}/lib/${MONKEYAUDIO_LIBNAME}.1.0.0.${LIBEXT}" DESTINATION ${TIGER_BIN_INSTALL})
file(COPY "${MONKEYAUDIO_HOME}/lib/${MONKEYAUDIO_LIBNAME}.1.0.${LIBEXT}" DESTINATION ${TIGER_BIN_INSTALL})
file(COPY "${MONKEYAUDIO_HOME}/lib/${MONKEYAUDIO_LIBNAME}.1.${LIBEXT}" DESTINATION ${TIGER_BIN_INSTALL})
file(COPY "${MONKEYAUDIO_HOME}/lib/${MONKEYAUDIO_LIBNAME}.${LIBEXT}" DESTINATION ${TIGER_BIN_INSTALL})

message("Copy Sparkle Framework")
#file(COPY "${SPARKLE_HOME}/${SPARKLE_LIBNAME}.framework/Resources" DESTINATION "${TIGER_INSTALL}/Frameworks/${SPARKLE_LIBNAME}.framework/Resources")
#file(COPY "${SPARKLE_HOME}/${SPARKLE_LIBNAME}.framework/Versions" DESTINATION "${TIGER_INSTALL}/Frameworks/${SPARKLE_LIBNAME}.framework/Versions")

message("Copy license resource")
file(GLOB LICENSE_SOUND_FILES "${CMAKE_SOURCE_DIR}/source/build/Resources/*.ogg")
file(COPY ${LICENSE_SOUND_FILES} DESTINATION "${TIGER_INSTALL}")
