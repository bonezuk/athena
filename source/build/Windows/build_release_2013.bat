set QTDIR=D:\Development\depot\utils\qt\install\qt-5_4_1-WIN32.VC12
set QT_PLUGIN_PATH=D:\Development\depot\utils\qt\install\qt-5_4_1-WIN32.VC12\plugins

set P4PORT=192.168.5.9:1666
set P4USER=stuart
set P4CLIENT=IcestormMk3

"D:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat"

git clone https://github.com/bonezuk/orcusversioner.git
cd orcusversioner
git checkout master
git pull

p4 sync

cmake "-DTIGER_DEBUG_BUILD:BOOL=OFF" "-DQT_HOME=D:\Development\depot\utils\qt\install\qt-5_4_1-WIN32.VC12" -G "NMake Makefiles"
nmake

git clone https://github.com/bonezuk/orcus.git
cd orcus
git fetch
git checkout main
git pull

"%WORKSPACE%\install\%BUILD_TYPE%\orcus\bin\versioner.exe" "%WORKSPACE%\source\player\Resources\buildInfo.xml" %BUILD_NUMBER% "%WORKSPACE%\source\player\Info.plist" "%WORKSPACE%\source\player\Info.plist" "%WORKSPACE%\source\player\player.rc" "%WORKSPACE%\source\installer\Version.nsh" "%WORKSPACE%\source\help\apple\Black Omega.help\Contents\Info.plist" "%WORKSPACE%\source\help\apple\Black Omega.help\Contents\Info.plist"

cmake "-DTIGER_DEBUG_BUILD:BOOL=OFF" "-DQT_HOME=D:\Development\depot\utils\qt\install\qt-5_4_1-WIN32.VC12" -G "NMake Makefiles"
nmake

cd source\help
D:\Development\depot\utils\qt\install\qt-5_4_1-WIN32.VC12\bin\qcollectiongenerator.exe ".\windows\help.qhcp" -o ".\windows\BlackOmega.qhc"
xcopy /Y ".\windows\BlackOmega.qch" "..\..\Build\Omega\help"
xcopy /Y ".\windows\BlackOmega.qhc" "..\..\Build\Omega\help"

cd ..\..\Build\Omega\bin
move "BlackOmega.exe" "Black Omega.exe"

cd ..\..\..\source\installer
"%PROGRAMFILES%\NSIS\makensis.exe" BlackOmega_2013.nsi

