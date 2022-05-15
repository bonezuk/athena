set BUILD_TYPE=WIN32.OPT.VC9

rmdir /S /Q "%WORKSPACE%\install"
mkdir "%WORKSPACE%\install"

C:\Windows\Microsoft.NET\Framework\v3.5\msbuild.exe "%WORKSPACE%\source\build\Windows\Omega_2008\Omega_2008.sln" /t:Rebuild /p:Configuration=ThirdPartyRelease /p:Platform=Win32

C:\Windows\Microsoft.NET\Framework\v3.5\msbuild.exe "%WORKSPACE%\source\build\Windows\Omega_2008\Omega_2008.sln" /t:Clean /p:Configuration=VersionerRelease /p:Platform=Win32

C:\Windows\Microsoft.NET\Framework\v3.5\msbuild.exe "%WORKSPACE%\source\build\Windows\Omega_2008\Omega_2008.sln" /t:Rebuild /p:Configuration=VersionerRelease /p:Platform=Win32

"%WORKSPACE%\install\%BUILD_TYPE%\omega\bin\versioner.exe" "%WORKSPACE%\source\player\Resources\buildInfo.xml" %BUILD_NUMBER% "%WORKSPACE%\source\player\Info.plist" "%WORKSPACE%\source\player\Info.plist" "%WORKSPACE%\source\player\player.rc" "%WORKSPACE%\source\installer\Version.nsh" "%WORKSPACE%\source\help\apple\Black Omega.help\Contents\Info.plist" "%WORKSPACE%\source\help\apple\Black Omega.help\Contents\Info.plist"

C:\Windows\Microsoft.NET\Framework\v3.5\msbuild.exe "%WORKSPACE%\source\build\Windows\Omega_2008\Omega_2008.sln" /t:Clean /p:Configuration=Release /p:Platform=Win32

C:\Windows\Microsoft.NET\Framework\v3.5\msbuild.exe "%WORKSPACE%\source\build\Windows\Omega_2008\Omega_2008.sln" /t:Rebuild /p:Configuration=Release /p:Platform=Win32

C:\Windows\Microsoft.NET\Framework\v3.5\msbuild.exe "%WORKSPACE%\source\build\Windows\Omega_2008\Omega_2008.sln" /t:Rebuild /p:Configuration=HelpRelease /p:Platform=Win32

rmdir /S /Q "%WORKSPACE%\test"
mkdir "%WORKSPACE%\test"
"%WORKSPACE%\install\%BUILD_TYPE%\omega\bin\test.exe" "--gtest_output=xml:%WORKSPACE%\test\unit_test_results.xml"

"C:\Program Files\Microsoft SDKs\Windows\v7.1\Bin\signtool.exe" sign /f "%WORKSPACE%\source\certificates\digiCert2014.pfx" /p 1234 /t http://timestamp.verisign.com/scripts/timstamp.dll "%WORKSPACE%\install\WIN32.OPT.VC9\omega\bin\Black Omega.exe"

"C:\Program Files\NSIS\makensis.exe" "%WORKSPACE%\source\installer\BlackOmega.nsi"

"C:\Program Files\Microsoft SDKs\Windows\v7.1\Bin\signtool.exe" sign /f "%WORKSPACE%\source\certificates\digiCert2014.pfx" /p 1234 /t http://timestamp.verisign.com/scripts/timstamp.dll "%WORKSPACE%\install\WIN32.OPT.VC9\omega\BlackOmega*.exe"
