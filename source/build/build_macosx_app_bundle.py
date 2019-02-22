import os
import shutil
import sys
import stat
import subprocess

isDebug = True
isUnitTest = True
isAppStore = False

qt5VersionMajor = 5
qt5VersionMinor = 10
qt5VersionRevision = 1

def get_root_project_directory():
    return os.path.realpath(os.path.join(os.path.dirname(__file__), '..', '..'))

def get_source_directory():
    return os.path.realpath(os.path.join(get_root_project_directory(), "source"))

def get_app_bundle_directory():
    return os.path.realpath(os.path.join(get_root_project_directory(), "Build", "Omega.app"))

def get_app_final_bundle_directory():
    return os.path.realpath(os.path.join(get_root_project_directory(), "Build", "Black Omega.app"))

def get_frameworks_directory():
    return os.path.realpath(os.path.join(get_app_bundle_directory(), "Contents", "Frameworks"))

def get_plugins_directory():
    return os.path.realpath(os.path.join(get_app_bundle_directory(), "Contents", "Plugins"))

def get_plugins_imageformats_directory():
    return os.path.realpath(os.path.join(get_plugins_directory(), "imageformats"))

def get_plugins_platforms_directory():
    return os.path.realpath(os.path.join(get_plugins_directory(), "platforms"))

def get_plugins_printsupport_directory():
    return os.path.realpath(os.path.join(get_plugins_directory(), "printsupport"))

def get_plugins_resources_directory():
    return os.path.realpath(os.path.join(get_app_bundle_directory(), "Contents", "Resources"))

def get_build_lib_path():
    return os.path.realpath(os.path.join(get_root_project_directory(), "Build", "Omega", "lib"))

def get_qt5_libname(libName):
    name = "lib" + libName
    if isDebug:
        name += "_debug"
    name +=  "." + str(qt5VersionMajor) + "." + str(qt5VersionMinor) + "." + str(qt5VersionRevision) + ".dylib"
    return name

def get_qt5_lib_major_minor_name(libName):
    name = "lib" + libName
    if isDebug:
        name += "_debug"
    name +=  "." + str(qt5VersionMajor) + "." + str(qt5VersionMinor) + ".dylib"
    return name

def get_qt5_lib_major_name(libName):
    name = "lib" + libName
    if isDebug:
        name += "_debug"
    name +=  "." + str(qt5VersionMajor) + ".dylib"
    return name

def get_qt5_lib_plain_name(libName):
    name = "lib" + libName
    if isDebug:
        name += "_debug"
    name +=  ".dylib"
    return name

def get_dest_qt5_library(libName):
    dest = get_plugins_directory() + "/" + get_qt5_libname(libName)
    return dest

def get_dest_qt5_major_minor_library(libName):
    dest = get_plugins_directory() + "/" + get_qt5_lib_major_minor_name(libName)
    return dest

def get_dest_qt5_major_library(libName):
    dest = get_plugins_directory() + "/" + get_qt5_lib_major_name(libName)
    return dest

def get_dest_qt5_plain_library(libName):
    dest = get_plugins_directory() + "/" + get_qt5_lib_plain_name(libName)
    return dest

def get_source_qt5_library(libName):
    src = get_build_lib_path() + "/" + get_qt5_libname(libName)
    return src

def copy_and_link_qt5_library(libName):
    print("copy " + libName + ".dylib")
    if os.path.exists(get_source_qt5_library(libName)) is not True:
        print(libName + " library not found '" + get_source_qt5_library(libName) + "'")
        sys.exit(-1)
    shutil.copyfile(get_source_qt5_library(libName), get_dest_qt5_library(libName))
    os.chmod(get_dest_qt5_library(libName), 0o755)
    os.symlink(get_qt5_libname(libName), get_dest_qt5_major_minor_library(libName))
    os.symlink(get_qt5_libname(libName), get_dest_qt5_major_library(libName))
    os.symlink(get_qt5_libname(libName), get_dest_qt5_plain_library(libName))

def copy_qt5_plugin(pluginDir, pluginName):
    libName = "lib" + pluginName + ".dylib"
    srcPlugin = os.path.realpath(os.path.join(get_build_lib_path(), pluginDir, libName))
    print("copy plugin " + libName)
    if os.path.exists(srcPlugin) is not True:
        print(libName + " library not found '" + srcPlugin + "'")
        sys.exit(-1)
    destPlugin = os.path.realpath(os.path.join(get_plugins_directory(), pluginDir, libName))
    shutil.copyfile(srcPlugin, destPlugin)
    os.chmod(destPlugin, 0o755)

def copy_and_link_library(libName, major):
    print("copy " + libName + ".dylib")
    lName = libName + "." + str(major) + ".dylib"
    srcLib = get_build_lib_path() + "/" + lName
    destLib = get_plugins_directory() + "/" + lName
    if os.path.exists(srcLib) is not True:
        print(libName + " library not found '" + srcLib + "'")
        sys.exit(-1)
    shutil.copyfile(srcLib, destLib)
    os.chmod(destLib, 0o755)
    lnLib = get_plugins_directory() + "/" + libName + ".dylib"
    os.symlink(lName, lnLib)

def copy_plain_library(libName):
    print("copy " + libName + ".dylib")
    srcLib = get_build_lib_path() + "/" + libName + ".dylib"
    destLib = get_plugins_directory() + "/" + libName + ".dylib"
    if os.path.exists(srcLib) is not True:
        print(libName + " library not found '" + srcLib + "'")
        sys.exit(-1)
    shutil.copyfile(srcLib, destLib)
    os.chmod(destLib,
             stat.S_IXUSR | stat.S_IWUSR | stat.S_IXUSR | stat.S_IXGRP | stat.S_IRGRP | stat.S_IXOTH | stat.S_IROTH)

def copy_executable(execName):
    print("copy " + execName)
    srcExec = get_build_lib_path() + "/" + execName
    destExec = get_frameworks_directory() + execName
    if os.path.exists(srcExec) is not True:
        print(execName + " library not found '" + srcExec + "'")
        sys.exit(-1)
    shutil.copyfile(srcExec, destExec)

def copy_and_help():
    print("Copy Black Omega.help")
    if isAppStore:
        srcHelpDir = get_source_directory() + "/help/appleStore/Black Omega.help"
    else:
        srcHelpDir = get_source_directory() + "/help/apple/Black Omega.help"
    destHelpDir = get_plugins_resources_directory() + "/Black Omega.help"
    shutil.copytree(srcHelpDir, destHelpDir)

def relink_id_for_library_major(libName, major):
    print("relink " + libName)
    lName = libName + "." + str(major) + ".dylib"
    idPath = "@executable_path/../PlugIns/" + lName
    libPath = get_plugins_directory() + "/" + lName
    subprocess.check_call(["install_name_tool", "-id", idPath, libPath])

def relink_change_library(imprint, libName, targetLib):
    oldPath = imprint + "/" + libName + ".dylib"
    execLibName = "@executable_path/../PlugIns/" + libName + ".dylib"
    targetLibName = get_plugins_directory() + "/" + targetLib + ".dylib"
    subprocess.check_call(["install_name_tool", "-change", oldPath, execLibName, targetLibName])

def relink_id_for_qt5_library(libName):
    print("relink " + libName)
    idPath = "@executable_path/../PlugIns/" + get_qt5_libname(libName)
    libPath = get_plugins_directory() + "/" + get_qt5_libname(libName)
    subprocess.check_call(["install_name_tool", "-id", idPath, libPath])

def relink_change_for_qt5_library(imprint, libName, targetLib):
    oldPath = imprint + "/" + get_qt5_lib_major_name(libName)
    execLibName = "@executable_path/../PlugIns/" + get_qt5_lib_major_name(libName)
    targetLibName = get_plugins_directory() + "/" + get_qt5_libname(targetLib)
    subprocess.check_call(["install_name_tool", "-change", oldPath, execLibName, targetLibName])

# Beginning of main build script
print("Build Black Omega application bundle for MacOSX")
if os.path.exists(get_app_bundle_directory()):
    print("Removing existing app bundle '" + get_app_bundle_directory() + "'")
    shutil.rmtree(get_app_bundle_directory())
if os.path.exists(get_app_final_bundle_directory()):
    print("Removing existing app bundle '" + get_app_final_bundle_directory() + "'")
    shutil.rmtree(get_app_final_bundle_directory())
print("Creating application bundle directory '" + get_app_bundle_directory() + "'")
os.mkdir(get_app_bundle_directory())
os.makedirs(get_frameworks_directory())
os.makedirs(get_plugins_imageformats_directory())
os.makedirs(get_plugins_platforms_directory())
os.makedirs(get_plugins_printsupport_directory())
os.makedirs(get_plugins_resources_directory())

qt5CoreLibName = "Qt5Core"
copy_and_link_qt5_library(qt5CoreLibName)
qt5GuiLibName = "Qt5Gui"
copy_and_link_qt5_library(qt5GuiLibName)
qt5XmlLibName = "Qt5Xml"
copy_and_link_qt5_library(qt5XmlLibName)
qt5WidgetsLibName = "Qt5Widgets"
copy_and_link_qt5_library(qt5WidgetsLibName)
qt5TestLibName = "Qt5Test"
copy_and_link_qt5_library(qt5TestLibName)
qt5PrintSupportLibName = "Qt5PrintSupport"
copy_and_link_qt5_library(qt5PrintSupportLibName)

copy_qt5_plugin("platforms", "qcocoa")
copy_qt5_plugin("printsupport", "cocoaprintersupport")
copy_qt5_plugin("imageformats", "qgif")
copy_qt5_plugin("imageformats", "qjpeg")

copy_and_link_library("libixml", 2)
copy_and_link_library("libthreadutil", 6)
copy_and_link_library("libupnp", 6)

copy_plain_library("libmpcdec")
copy_and_link_library("libwavpack", 1)

if isUnitTest:
    copy_plain_library("libgtest")
    copy_plain_library("libgtest_main")
    copy_plain_library("libgmock")
    copy_plain_library("libgmock_main")

copy_and_help()

copy_plain_library("libcommon")
copy_plain_library("libengine")
copy_plain_library("libblueomega")
copy_plain_library("libsilveromega")
copy_plain_library("libblackomega")
copy_plain_library("libredomega")
copy_plain_library("libwhiteomega")
copy_plain_library("libgreenomega")
copy_plain_library("libvioletomega")
copy_plain_library("libcyanomega")
copy_plain_library("libtoneomega")
copy_plain_library("libwavpackomega")
copy_plain_library("libnetwork_omega")
copy_plain_library("librtp")
copy_plain_library("librtp_silveromega")
copy_plain_library("libhttp")
copy_plain_library("libmime")
copy_plain_library("libaudioio")
copy_plain_library("libtrackinfo")
copy_plain_library("libtrackdb")
copy_plain_library("libtrackmodel")
copy_plain_library("libdlna")
copy_plain_library("libremote")
copy_plain_library("libwidget")

if isUnitTest:
    copy_executable("Test")

copy_and_link_library("libxml2", 2)

#echo "relink libixml"
#install_name_tool -id @executable_path/../PlugIns/libixml.2.dylib "$TIGER_INSTALL_DIR/$OMEGA_APP_NAME.app/Contents/PlugIns/libixml.2.dylib"
relink_id_for_library_major("libixml",2)

#echo "relink libthreadutil"
#install_name_tool -id @executable_path/../PlugIns/libthreadutil.6.dylib "$TIGER_INSTALL_DIR/$OMEGA_APP_NAME.app/Contents/PlugIns/libthreadutil.6.dylib"
relink_id_for_library_major("libthreadutil",6)

#echo "relink libupnp"
#install_name_tool -id @executable_path/../PlugIns/libupnp.6.dylib "$TIGER_INSTALL_DIR/$OMEGA_APP_NAME.app/Contents/PlugIns/libupnp.6.dylib"
#install_name_tool -change $UPNP_IMPRINT/lib/libixml.2.dylib @executable_path/../PlugIns/libixml.2.dylib "$TIGER_INSTALL_DIR/$OMEGA_APP_NAME.app/Contents/PlugIns/libupnp.6.dylib"
#install_name_tool -change $UPNP_IMPRINT/lib/libthreadutil.6.dylib @executable_path/../PlugIns/libthreadutil.6.dylib "$TIGER_INSTALL_DIR/$OMEGA_APP_NAME.app/Contents/PlugIns/libupnp.6.dylib"
relink_id_for_library_major("libupnp",6)
relink_change_library("@rpath", "libixml.2", "libupnp.6")
relink_change_library("@rpath", "libthreadutil.6", "libupnp.6")

#echo "relink Qt5Core"
#install_name_tool -id @executable_path/../PlugIns/libQt5Core$QTLIB_DEBUG_SUFFIX.$QT_VERSION_MAJOR.dylib "$TIGER_INSTALL_DIR/$OMEGA_APP_NAME.app/Contents/PlugIns/libQt5Core$QTLIB_DEBUG_SUFFIX.$QT_VERSION_MAJOR.dylib"
relink_id_for_qt5_library("Qt5Core")

#echo "relink Qt5Gui"
#install_name_tool -id @executable_path/../PlugIns/libQt5Gui$QTLIB_DEBUG_SUFFIX.$QT_VERSION_MAJOR.dylib "$TIGER_INSTALL_DIR/$OMEGA_APP_NAME.app/Contents/PlugIns/libQt5Gui$QTLIB_DEBUG_SUFFIX.$QT_VERSION_MAJOR.dylib"
#install_name_tool -change @rpath/libQt5Core$QTLIB_DEBUG_SUFFIX.$QT_VERSION_MAJOR.dylib @executable_path/../PlugIns/libQt5Core$QTLIB_DEBUG_SUFFIX.$QT_VERSION.dylib "$TIGER_INSTALL_DIR/$OMEGA_APP_NAME.app/Contents/PlugIns/libQt5Gui$QTLIB_DEBUG_SUFFIX.$QT_VERSION_MAJOR.dylib"
relink_id_for_qt5_library("Qt5Gui")
relink_change_for_qt5_library("@rpath", "Qt5Core", "Qt5Gui")

#echo "relink Qt5Xml"
#install_name_tool -id @executable_path/../PlugIns/libQt5Xml$QTLIB_DEBUG_SUFFIX.$QT_VERSION_MAJOR.dylib "$TIGER_INSTALL_DIR/$OMEGA_APP_NAME.app/Contents/PlugIns/libQt5Xml$QTLIB_DEBUG_SUFFIX.$QT_VERSION_MAJOR.dylib"
#install_name_tool -change @rpath/libQt5Core$QTLIB_DEBUG_SUFFIX.$QT_VERSION_MAJOR.dylib @executable_path/../PlugIns/libQt5Core$QTLIB_DEBUG_SUFFIX.$QT_VERSION_MAJOR.dylib "$TIGER_INSTALL_DIR/$OMEGA_APP_NAME.app/Contents/PlugIns/libQt5Xml$QTLIB_DEBUG_SUFFIX.$QT_VERSION_MAJOR.dylib"
relink_id_for_qt5_library("Qt5Xml")
relink_change_for_qt5_library("@rpath", "Qt5Core", "Qt5Xml")

#echo "relink Qt5Widgets"
#install_name_tool -id @executable_path/../PlugIns/libQt5Widgets$QTLIB_DEBUG_SUFFIX.$QT_VERSION_MAJOR.dylib "$TIGER_INSTALL_DIR/$OMEGA_APP_NAME.app/Contents/PlugIns/libQt5Widgets$QTLIB_DEBUG_SUFFIX.$QT_VERSION_MAJOR.dylib"
#install_name_tool -change @rpath/libQt5Core$QTLIB_DEBUG_SUFFIX.$QT_VERSION_MAJOR.dylib @executable_path/../PlugIns/libQt5Core$QTLIB_DEBUG_SUFFIX.$QT_VERSION_MAJOR.dylib "$TIGER_INSTALL_DIR/$OMEGA_APP_NAME.app/Contents/PlugIns/libQt5Widgets$QTLIB_DEBUG_SUFFIX.$QT_VERSION_MAJOR.dylib"
#install_name_tool -change @rpath/libQt5Gui$QTLIB_DEBUG_SUFFIX.$QT_VERSION_MAJOR.dylib @executable_path/../PlugIns/libQt5Gui$QTLIB_DEBUG_SUFFIX.$QT_VERSION_MAJOR.dylib "$TIGER_INSTALL_DIR/$OMEGA_APP_NAME.app/Contents/PlugIns/libQt5Widgets$QTLIB_DEBUG_SUFFIX.$QT_VERSION_MAJOR.dylib"
relink_id_for_qt5_library("Qt5Widgets")
relink_change_for_qt5_library("@rpath", "Qt5Core", "Qt5Widgets")
relink_change_for_qt5_library("@rpath", "Qt5Gui", "Qt5Widgets")
