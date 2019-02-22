import os
import shutil
import sys
import stat

isDebug = True
isUnitTest = True

qt5VersionMajor = 5
qt5VersionMinor = 10
qt5VersionRevision = 1

def get_root_project_directory():
    return os.path.realpath(os.path.join(os.path.dirname(__file__), '..', '..'))

def get_app_bundle_directory():
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
    os.chmod(get_dest_qt5_library(libName),
             stat.S_IXUSR | stat.S_IWUSR | stat.S_IXUSR | stat.S_IXGRP | stat.S_IRGRP | stat.S_IXOTH | stat.S_IROTH)
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
    os.chmod(destPlugin,
             stat.S_IXUSR | stat.S_IWUSR | stat.S_IXUSR | stat.S_IXGRP | stat.S_IRGRP | stat.S_IXOTH | stat.S_IROTH)

def copy_and_link_library(libName, major):
    print("copy " + libName + ".dylib")
    lName = libName + "." + str(major) + ".dylib"
    srcLib = get_build_lib_path() + "/" + lName
    destLib = get_plugins_directory() + "/" + lName
    if os.path.exists(srcLib) is not True:
        print(libName + " library not found '" + srcLib + "'")
        sys.exit(-1)
    shutil.copyfile(srcLib, destLib)
    os.chmod(destLib,
             stat.S_IXUSR | stat.S_IWUSR | stat.S_IXUSR | stat.S_IXGRP | stat.S_IRGRP | stat.S_IXOTH | stat.S_IROTH)
    lnLib = get_plugins_directory() + "/" + libName + ".dylib"
    os.symlink(lName, lnLib)

def copy_and_link_plain_library(libName):
    print("copy " + libName + ".dylib")
    srcLib = get_build_lib_path() + "/" + libName + ".dylib"
    destLib = get_plugins_directory() + "/" + libName + ".dylib"
    if os.path.exists(srcLib) is not True:
        print(libName + " library not found '" + srcLib + "'")
        sys.exit(-1)
    shutil.copyfile(srcLib, destLib)
    os.chmod(destLib,
             stat.S_IXUSR | stat.S_IWUSR | stat.S_IXUSR | stat.S_IXGRP | stat.S_IRGRP | stat.S_IXOTH | stat.S_IROTH)

# Beginning of main build script
print("Build Black Omega application bundle for MacOSX")
if os.path.exists(get_app_bundle_directory()):
    print("Removing existing app bundle '" + get_app_bundle_directory() + "'")
    shutil.rmtree(get_app_bundle_directory())
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

copy_and_link_plain_library("libmpcdec")
copy_and_link_library("libwavpack", 1)

if isUnitTest:
    copy_and_link_plain_library("libgtest")
    copy_and_link_plain_library("libgtest_main")
    copy_and_link_plain_library("libgmock")
    copy_and_link_plain_library("libgmock_main")
