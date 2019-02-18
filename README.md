Third Party Libraries

Black Omega and its associated unit tests and utility programs makes use of various
third party libraries. With the exception of Qt5, the various third party libraries 
are collective stored in the one directory.

For convience I have provided an additional git reposiatory with the libraries pre-built
for both Windows, MacOSX and Linux enviroments.

https://github.com/bonezuk/athena_utils.git

For Windows the pre-compiled library files are for Windows 32-bit and 64-bit Visual Studio 2013.
	- Debug and release versions of both the win32 and x64 libraries are in their own directory
	- e.g. for gmock these are
		gmock-1.7.0-WIN32.DEBUG.VC12
		gmock-1.7.0-WIN32.OPT.VC12
		gmock-1.7.0-WIN64.DEBUG.VC12
		gmock-1.7.0-WIN64.OPT.VC12
	- In this document these directories are referenced as : gmock-1.7.0-BUILD_TYPE
		
For MacOSX they're built using XCode 7 on macOS Sierra.

Unit-Test Frameworks

-----------
Google Test and Mock v1.7.0
Source code : athena_utils/gmock/source/gmock-1.7.0.tar.gz
Cloned from https://github.com/paulsapps/gmock-1.7.0.git

Both Google Mock and Test are built together

Windows
	- Use CMake with source directory gmock-1.7.0 and Visual Studio generator.
	- Tick BUILD_SHARED_LIBS
	- Compile libraries with MSVC.
	- Copy Google Test includes from gmock-1.7.0/gtest/include to athena_utils/gmock/gmock-1.7.0-BUILD_TYPE/include directory.
	- Copy Google Mock includes from gmock-1.7.0/include to athena_utils/gmock/gmock-1.7.0-BUILD_TYPE/include directory.
	- Copy Google Test dll library from cmake_build/gtest to athena_utils/gmock/gmock-1.7.0-BUILD_TYPE/lib directory.
	- Copy Google Mock dll library from cmake_build to athena_utils/gmock/gmock-1.7.0-BUILD_TYPE/lib directory.


MacOSX
	- Use CMake with source directory gmock-1.7.0 and Unix Makefiles generator.
	- Tick BUILD_SHARED_LIBS
	- In Terminal ls to build directory and run make.
	- Copy Google Test includes from gmock-1.7.0/gtest/include to athena_utils/gmock/gmock-1.7.0-Mac64/include directory.
	- Copy Google Mock includes from gmock-1.7.0/include to athena_utils/gmock/gmock-1.7.0-Mac64/include directory.
	- Copy Google Test dylib library from cmake_build/gtest to athena_utils/gmock/gmock-1.7.0-Mac64/lib directory.
	- Copy Google Mock dylib library from cmake_build to athena_utils/gmock/gmock-1.7.0-Mac64/lib directory.

-----------
libupnp v1.6.22
Webpage : http://pupnp.sourceforge.net

Windows
	- Requires POSIX Threads for Windows https://sourceforge.net/projects/pthreads4w/
		- Use MSVC command prompt to build posix e.g. "namke clean VC-debug"
		- Copy pthread and libraries into libupnp-1.6.25-BUILD_TYPE
	- Open and upgrade libupnp-1.6.25\build\vc9\libupnp.sln in Visual Studio
	- Adjust to point at new POSIX thread library
	- Build and copy to athena_utils\libupnp\libupnp-1.6.19-BUILD_TYPE
	
MacOSX
	 - In Terminal cd to unpacked source directory
	 - ./configure --prefix==~/athena_utils/libupnp/libupnp-1.6.25-Mac64
	 - make
	 - make install

-----------
libxml2 
Webpage : http://www.xmlsoft.org/index.html
Source code : ftp://xmlsoft.org/libxml2/libxml2-2.9.8.tar.gz	 

Windows : Use alternative build for WIN32 https://github.com/kiyolee/libxml2-win-build.git
	- Set Runtime Library to Multi-threaded DLL

MacOSX
	 - In Terminal cd to unpacked source directory
	 - ./configure --prefix==~/athena_utils/libxml2/libxml2-2.9.8-Mac64
	 - make
	 - make install

------------
Musepack
Webpage : https://www.musepack.net/index.php?pg=src

The mpcdec.h header file is modified such that the internal structure of type mpc_dec_t is defined.

MacOSX
	- Use CMake with Unix Makefiles to build libcuefile.
	- Copy dylib and include from libcuefile into ~/athena_utils/musepack/musepack-r475-Mac64
	- Use CMake with Unix Makefiles to build libreplaygain.
	- Copy dylib and include from libreplaygain into ~/athena_utils/musepack/musepack-r475-Mac64
	- Use CMake to build musepack_src
	- Set CUEFILE_INCLUDE_DIR to ~/athena_utils/musepack/musepack-r475-Mac64/include 
	- Set REPLAY_GAIN_INCLUDE_DIR to ~/athena_utils/musepack/musepack-r475-Mac64/include 
	- Set CUEFILE_LIBRARY to ~/athena_utils/musepack/musepack-r475-Mac64/lib/libcuefile.dylib
	- Set REPLAY_GAIN_LIBRARY to ~/athena_utils/musepack/musepack-r475-Mac64/lib/libreplaygain.dylib
	- Use Unix Makefiles to build and copy includes and built libraries into place.



------------
Wavpack
Webpage : http://www.wavpack.com

MacOSX
	- In Terminal cd to unpacked source directory
	- ./autogen.sh
	- ./configure --prefix=~/athena_utils/wavpack/wavpack-5.1.0-Mac64
	- make
	- make install
	
