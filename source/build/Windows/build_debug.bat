
set WORKSPACE=C:\Development\Orcus
set BUILD_TYPE=WIN32.DEBUG.VC9
set BUILD_NUMBER=153

rmdir /S /Q %WORKSPACE%\install
mkdir %WORKSPACE%\install
msbuild.exe %WORKSPACE%\source\build\Windows\Orcus_2008\Orcus_2008.sln /t:setup\third_party_libraries:rebuild /p:Configuration=BuildDebug /p:Platform=Win32

msbuild.exe %WORKSPACE%\source\build\Windows\Orcus_2008\Orcus_2008.sln /t:setup\versioner:clean /p:Configuration=BuildDebug /p:Platform=Win32
msbuild.exe %WORKSPACE%\source\build\Windows\Orcus_2008\Orcus_2008.sln /t:setup\versioner:rebuild /p:Configuration=BuildDebug /p:Platform=Win32

"%WORKSPACE%\install\%BUILD_TYPE%\orcus\bin\versioner.exe" "%WORKSPACE%\source\player\Resources\buildInfo.xml" %BUILD_NUMBER% "%WORKSPACE%\source\player\Info.plist" "%WORKSPACE%\source\player\Info.plist" "%WORKSPACE%\source\player\player.rc" "%WORKSPACE%\source\installer\Version.nsh" "%WORKSPACE%\source\help\apple\Black Omega.help\Contents\Info.plist" "%WORKSPACE%\source\help\apple\Black Omega.help\Contents\Info.plist"

msbuild.exe %WORKSPACE%\source\build\Windows\Orcus_2008\Orcus_2008.sln /t:Clean,Build /p:Configuration=Debug /p:Platform=Win32

msbuild.exe %WORKSPACE%\source\build\Windows\Orcus_2008\Orcus_2008.sln /t:setup\help:rebuild /p:Configuration=BuildDebug /p:Platform=Win32
