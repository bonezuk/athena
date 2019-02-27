# Script for Black Omega installer

!include "MUI2.nsh"

!define APPNAME "Black Omega"
!define COMPANYNAME "Stuart A. MacLean"

!include "Version.nsh"

!define HELPURL "http://www.blackomega.co.uk"
!define UPDATEURL "http://www.blackomega.co.uk"
!define ABOUTURL "http://www.blackomega.co.uk"

!define INSTALLSIZE 36046

!define ORCUSPATH "..\..\Build\Omega"

InstallDir "$PROGRAMFILES\${APPNAME}"
InstallDirRegKey HKCU "Software\Black Omega 2\InstallDir" ""

LicenseData "license.rtf"

Name "${APPNAME}"
OutFile "${ORCUSPATH}\BlackOmega-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_REVISION}.${VERSION_BUILD}-Installer.exe"

ShowInstDetails "nevershow"
ShowUninstDetails "nevershow"

;--------------------------------
;Interface Settings

!define MUI_ABORTWARNING
!define MUI_ICON "..\player\omega.ico"
!define MUI_UNICON "..\player\omega.ico"

;--------------------------------
;Pages

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "license.rtf"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH
  
;--------------------------------
;Languages
 
!insertmacro MUI_LANGUAGE "English"

;--------------------------------

section  "Black Omega" SecDummy
	SetOverwrite on
	setOutPath "$INSTDIR\bin"
	file "${ORCUSPATH}\bin\msvcp120.dll"
	file "${ORCUSPATH}\bin\msvcr120.dll"
	file "${ORCUSPATH}\bin\vccorlib120.dll"
	file "${ORCUSPATH}\bin\assistant.exe"
	file "${ORCUSPATH}\bin\audioio.dll"
	file "${ORCUSPATH}\bin\Black Omega.exe"
	file "${ORCUSPATH}\bin\blackomega.dll"
	file "${ORCUSPATH}\bin\blueomega.dll"
	file "${ORCUSPATH}\bin\common.dll"
	file "${ORCUSPATH}\bin\cyanomega.dll"
	file "${ORCUSPATH}\bin\dlna.dll"
	file "${ORCUSPATH}\bin\engine.dll"
	file "${ORCUSPATH}\bin\ftp.dll"
	file "${ORCUSPATH}\bin\greenomega.dll"
	file "${ORCUSPATH}\bin\http.dll"
	file "${ORCUSPATH}\bin\libiconv.dll"
	file "${ORCUSPATH}\bin\libupnp.dll"
	file "${ORCUSPATH}\bin\libxml2.dll"
	file "${ORCUSPATH}\bin\mime.dll"
	file "${ORCUSPATH}\bin\monkey.dll"
	file "${ORCUSPATH}\bin\monkeyomega.dll"
	file "${ORCUSPATH}\bin\mpcdec.dll"
	file "${ORCUSPATH}\bin\network.dll"
	file "${ORCUSPATH}\bin\pthreadVC2.dll"
	file "${ORCUSPATH}\bin\Qt5CLucene.dll"
	file "${ORCUSPATH}\bin\Qt5Core.dll"
	file "${ORCUSPATH}\bin\Qt5Gui.dll"
	file "${ORCUSPATH}\bin\Qt5Help.dll"
	file "${ORCUSPATH}\bin\Qt5Network.dll"
	file "${ORCUSPATH}\bin\Qt5PrintSupport.dll"
	file "${ORCUSPATH}\bin\Qt5Sql.dll"
	file "${ORCUSPATH}\bin\Qt5Widgets.dll"
	file "${ORCUSPATH}\bin\Qt5Xml.dll"
	file "${ORCUSPATH}\bin\redomega.dll"
	file "${ORCUSPATH}\bin\remote.dll"
	file "${ORCUSPATH}\bin\rtp.dll"
	file "${ORCUSPATH}\bin\rtp_silveromega.dll"
	file "${ORCUSPATH}\bin\silveromega.dll"
	file "${ORCUSPATH}\bin\trackdb.dll"
	file "${ORCUSPATH}\bin\trackinfo.dll"
	file "${ORCUSPATH}\bin\trackmodel.dll"
	file "${ORCUSPATH}\bin\violetomega.dll"
	file "${ORCUSPATH}\bin\wavpackdll.dll"
	file "${ORCUSPATH}\bin\wavpackomega.dll"
	file "${ORCUSPATH}\bin\whiteomega.dll"
	file "${ORCUSPATH}\bin\omega.ico"
	file "${ORCUSPATH}\bin\widget.dll"
	file "${ORCUSPATH}\bin\WinSparkle.dll"
	file "${ORCUSPATH}\bin\qt.conf"
	setOutPath "$INSTDIR\data\license"
	file "${ORCUSPATH}\data\license\license8.ogg"
	file "${ORCUSPATH}\data\license\license11.ogg"
	file "${ORCUSPATH}\data\license\license12.ogg"
	file "${ORCUSPATH}\data\license\license16.ogg"
	file "${ORCUSPATH}\data\license\license22.ogg"
	file "${ORCUSPATH}\data\license\license24.ogg"
	file "${ORCUSPATH}\data\license\license32.ogg"
	file "${ORCUSPATH}\data\license\license44.ogg"
	file "${ORCUSPATH}\data\license\license48.ogg"
	file "${ORCUSPATH}\data\license\license64.ogg"
	file "${ORCUSPATH}\data\license\license96.ogg"
	setOutPath "$INSTDIR\help"
	file "${ORCUSPATH}\help\BlackOmega.qch"
	file "${ORCUSPATH}\help\BlackOmega.qhc"
	setOutPath "$INSTDIR\plugins\platforms"
	file "${ORCUSPATH}\plugins\platforms\qwindows.dll"
	setOutPath "$INSTDIR\plugins\imageformats"
	file "${ORCUSPATH}\plugins\imageformats\qgif.dll"
	file "${ORCUSPATH}\plugins\imageformats\qjpeg.dll"
	file "${ORCUSPATH}\plugins\imageformats\qmng.dll"
	setOutPath "$INSTDIR\plugins\sqldrivers"
	file "${ORCUSPATH}\plugins\sqldrivers\qsqlite.dll"
	
	writeUninstaller "$INSTDIR\uninstall.exe"
	
	createDirectory "$SMPROGRAMS\${APPNAME}"
	createShortCut "$SMPROGRAMS\${APPNAME}\${APPNAME}.lnk" "$INSTDIR\bin\${APPNAME}.exe" "" "$INSTDIR\bin\omega.ico" 0

	WriteRegStr HKCU "Software\Black Omega 2\InstallDir" "" $INSTDIR
	
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "QuietUninstallString" "$\"$INSTDIR\uninstall.exe$\" /S"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "InstallLocation" "$\"$INSTDIR$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayIcon" "$\"$INSTDIR\bin\Black Omega.exe$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "Publisher" "${COMPANYNAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "HelpLink" "$\"${HELPURL}$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "URLUpdateInfo" "$\"${UPDATEURL}$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "URLInfoAbout" "$\"${ABOUTURL}$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayVersion" "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_REVISION}"
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "VersionMajor" ${VERSION_MAJOR}
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "VersionMinor" ${VERSION_MINOR}
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "VersionRevision" ${VERSION_REVISION}
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "VersionBuild" ${VERSION_BUILD}
	# There is no option for modifying or repairing the install
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "NoModify" 1
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "NoRepair" 1
	# Set the INSTALLSIZE constant (!defined at the top of this script) so Add/Remove Programs can accurately report the size
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "EstimatedSize" ${INSTALLSIZE}
sectionEnd

 ;Language strings
 LangString DESC_SecDummy ${LANG_ENGLISH} "A test section."

 ;Assign language strings to sections
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${SecDummy} $(DESC_SecDummy)
!insertmacro MUI_FUNCTION_DESCRIPTION_END


section "Uninstall"
	delete "$SMPROGRAMS\${APPNAME}\${APPNAME}.lnk"
	rmDir "$SMPROGRAMS\${APPNAME}"

	delete "$INSTDIR\bin\msvcp120.dll"
	delete "$INSTDIR\bin\msvcr120.dll"
	delete "$INSTDIR\bin\vccorlib120.dll"
	delete "$INSTDIR\bin\assistant.exe"
	delete "$INSTDIR\bin\audioio.dll"
	delete "$INSTDIR\bin\Black Omega.exe"
	delete "$INSTDIR\bin\blackomega.dll"
	delete "$INSTDIR\bin\blueomega.dll"
	delete "$INSTDIR\bin\common.dll"
	delete "$INSTDIR\bin\cyanomega.dll"
	delete "$INSTDIR\bin\dlna.dll"
	delete "$INSTDIR\bin\engine.dll"
	delete "$INSTDIR\bin\ftp.dll"
	delete "$INSTDIR\bin\greenomega.dll"
	delete "$INSTDIR\bin\http.dll"
	delete "$INSTDIR\bin\libiconv.dll"
	delete "$INSTDIR\bin\libupnp.dll"
	delete "$INSTDIR\bin\libxml2.dll"
	delete "$INSTDIR\bin\mime.dll"
	delete "$INSTDIR\bin\monkey.dll"
	delete "$INSTDIR\bin\monkeyomega.dll"
	delete "$INSTDIR\bin\mpcdec.dll"
	delete "$INSTDIR\bin\network.dll"
	delete "$INSTDIR\bin\pthreadVC2.dll"
	delete "$INSTDIR\bin\Qt5CLucene.dll"
	delete "$INSTDIR\bin\Qt5Core.dll"
	delete "$INSTDIR\bin\Qt5Gui.dll"
	delete "$INSTDIR\bin\Qt5Help.dll"
	delete "$INSTDIR\bin\Qt5Network.dll"
	delete "$INSTDIR\bin\Qt5PrintSupport.dll"
	delete "$INSTDIR\bin\Qt5Sql.dll"
	delete "$INSTDIR\bin\Qt5Widgets.dll"
	delete "$INSTDIR\bin\Qt5Xml.dll"
	delete "$INSTDIR\bin\redomega.dll"
	delete "$INSTDIR\bin\remote.dll"
	delete "$INSTDIR\bin\rtp.dll"
	delete "$INSTDIR\bin\rtp_silveromega.dll"
	delete "$INSTDIR\bin\silveromega.dll"
	delete "$INSTDIR\bin\trackdb.dll"
	delete "$INSTDIR\bin\trackinfo.dll"
	delete "$INSTDIR\bin\trackmodel.dll"
	delete "$INSTDIR\bin\violetomega.dll"
	delete "$INSTDIR\bin\wavpackdll.dll"
	delete "$INSTDIR\bin\wavpackomega.dll"
	delete "$INSTDIR\bin\whiteomega.dll"
	delete "$INSTDIR\bin\omega.ico"
	delete "$INSTDIR\bin\widget.dll"
	delete "$INSTDIR\bin\WinSparkle.dll"
	delete "$INSTDIR\bin\qt.conf"
	delete "$INSTDIR\data\license\license8.ogg"
	delete "$INSTDIR\data\license\license11.ogg"
	delete "$INSTDIR\data\license\license12.ogg"
	delete "$INSTDIR\data\license\license16.ogg"
	delete "$INSTDIR\data\license\license22.ogg"
	delete "$INSTDIR\data\license\license24.ogg"
	delete "$INSTDIR\data\license\license32.ogg"
	delete "$INSTDIR\data\license\license44.ogg"
	delete "$INSTDIR\data\license\license48.ogg"
	delete "$INSTDIR\data\license\license64.ogg"
	delete "$INSTDIR\data\license\license96.ogg"
	delete "$INSTDIR\data\license\(01)-center.wav"
	delete "$INSTDIR\data\license\(02)-left_front.wav"
	delete "$INSTDIR\data\license\(03)-right_front.wav"
	delete "$INSTDIR\data\license\(04)-left_surround.wav"
	delete "$INSTDIR\data\license\(05)-right_surround.wav"
	delete "$INSTDIR\data\license\(06)-left_rear.wav"
	delete "$INSTDIR\data\license\(07)-right_rear.wav"
	delete "$INSTDIR\data\license\(08)-subwoofer.wav"
	delete "$INSTDIR\data\surround\(01)-center.wav"
	delete "$INSTDIR\data\surround\(02)-left_front.wav"
	delete "$INSTDIR\data\surround\(03)-right_front.wav"
	delete "$INSTDIR\data\surround\(04)-left_surround.wav"
	delete "$INSTDIR\data\surround\(05)-right_surround.wav"
	delete "$INSTDIR\data\surround\(06)-left_rear.wav"
	delete "$INSTDIR\data\surround\(07)-right_rear.wav"
	delete "$INSTDIR\data\surround\(08)-subwoofer.wav"
	delete "$INSTDIR\help\BlackOmega.qch"
	delete "$INSTDIR\help\BlackOmega.qhc"
	delete "$INSTDIR\plugins\imageformats\qgif4.dll"
	delete "$INSTDIR\plugins\imageformats\qjpeg4.dll"
	delete "$INSTDIR\plugins\imageformats\qmng4.dll"
	delete "$INSTDIR\plugins\imageformats\qgif.dll"
	delete "$INSTDIR\plugins\imageformats\qjpeg.dll"
	delete "$INSTDIR\plugins\imageformats\qmng.dll"
	delete "$INSTDIR\plugins\platforms\qwindows.dll"
	delete "$INSTDIR\plugins\sqldrivers\qsqlite.dll"
	delete "$INSTDIR\uninstall.exe"
	
	rmDir "$INSTDIR\bin\"
	rmDir "$INSTDIR\data\license"
	rmDir "$INSTDIR\data\"
	rmDir "$INSTDIR\help\"
	rmDir "$INSTDIR\plugins\sqldrivers"
	rmDir "$INSTDIR\plugins\imageformats"
	rmDir "$INSTDIR\plugins\platforms"
	rmDir "$INSTDIR\plugins"
	rmDir "$INSTDIR"
	
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"
sectionEnd
