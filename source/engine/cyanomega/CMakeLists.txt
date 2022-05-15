add_definitions(-DOMEGA_ENGINE_CYANOMEGA_DLL)

file(GLOB CYANOMEGA_ENGINE_HEADER inc/*.h)
file(GLOB CYANOMEGA_ENGINE_SOURCE src/*.cxx)

add_library(cyanomega SHARED ${CYANOMEGA_ENGINE_HEADER} ${CYANOMEGA_ENGINE_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(cyanomega engine common xml2 mpcdec ${QTVER}::Core ${QTVER}::Gui ${QTVER}::Xml winmm)
else (OMEGA_WIN32)
	target_link_libraries(cyanomega engine common xml2 mpcdec ${QTVER}::Core ${QTVER}::Gui ${QTVER}::Xml)
endif (OMEGA_WIN32)

if (${TIGER_LINUX_DISTRO})
	install(TARGETS cyanomega DESTINATION lib)
endif (${TIGER_LINUX_DISTRO})
