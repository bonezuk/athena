# Sparkle configuration

set(SPARKLE_VERSION "1.5b6")
set(SPARKLE_LIBNAME "Sparkle")
set(SPARKLE_HOME "${TIGER_UTILS}/sparkle/install/sparkle-${SPARKLE_VERSION}.${TIGER_PLATFORM}")

include_directories(AFTER "${SPARKLE_HOME}/include" )

find_library(SPARKLE_FRAMEWORK Sparkle PATHS ${SPARKLE_HOME})
