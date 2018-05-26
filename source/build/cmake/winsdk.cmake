# Windows 7.1 SDK configuration

if (${CMAKE_GENERATOR} MATCHES "NMake Makefiles")
	set(WIN_SDK_HOME "D:\\Program Files\\Microsoft SDKs\\Windows\\v7.1" CACHE PATH "Windows SDK 7.1")
	message("Set ${WIN_SDK_HOME}")
	
	set_property(TARGET wbemuuid PROPERTY IMPORTED_IMPLIB "${WIN_SDK_HOME}/Lib/WbemUuid.Lib" )
	set_property(TARGET iphlpapi PROPERTY IMPORTED_IMPLIB "${WIN_SDK_HOME}/Lib/IPHlpApi.Lib" )
	set_property(TARGET winmm PROPERTY IMPORTED_IMPLIB "${WIN_SDK_HOME}/Lib/WinMM.Lib" )
endif (${CMAKE_GENERATOR} MATCHES "NMake Makefiles")
