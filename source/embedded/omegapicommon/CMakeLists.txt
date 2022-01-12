add_definitions(-DORCUS_OMEGAPICOMMON_DLL)

file(GLOB OMEGAPICOMMON_HEADER inc/*.h)
file(GLOB OMEGAPICOMMON_SOURCE src/*.cxx)

add_library(omegapicommon SHARED ${OMEGAPICOMMON_HEADER} ${OMEGAPICOMMON_SOURCE})

if (OMEGA_WIN32)
	target_link_libraries(omegapicommon engine common blackomega silveromega whiteomega network_omega xml2 Qt5::Core Qt5::Gui Qt5::Xml winmm)
else (OMEGA_WIN32)
	target_link_libraries(omegapicommon engine common blackomega silveromega whiteomega network_omega xml2 Qt5::Core Qt5::Gui Qt5::Xml)
endif (OMEGA_WIN32)

if (${TIGER_LINUX_DISTRO})
	install(TARGETS omegapicommon DESTINATION lib)
endif (${TIGER_LINUX_DISTRO})
