add_definitions(-DOMEGA_ENGINE_REDOMEGA_DLL)

file(GLOB REDOMEGA_ENGINE_HEADER inc/*.h)
file(GLOB REDOMEGA_ENGINE_SOURCE src/*.cxx)

add_library(redomega SHARED ${REDOMEGA_ENGINE_HEADER} ${REDOMEGA_ENGINE_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(redomega engine common xml2 ${QTVER}::Core ${QTVER}::Gui ${QTVER}::Xml winmm)
else (OMEGA_WIN32)
	target_link_libraries(redomega engine common xml2 ${QTVER}::Core ${QTVER}::Gui ${QTVER}::Xml)
endif (OMEGA_WIN32)

if (${TIGER_LINUX_DISTRO})
	install(TARGETS redomega DESTINATION lib)
endif (${TIGER_LINUX_DISTRO})
