Third Party Libraries

Black Omega and its associated unit tests and utility programs makes use of various
third party libraries. With the exception of Qt5, the various third party libraries 
are collective stored in the one directory.

For convience I have provided an additional git reposiatory with the libraries pre-built
for both the Windows, MacOSX and Linux enviroments.

https://github.com/bonezuk/athena_utils.git

For Windows the pre-compiled library files are for Windows 64-bit Visual Studio 2013.
For MacOSX they're built using XCode 7 on macOS Sierra.

Unit-Test Frameworks

-----------
Google Test and Mock v1.7.0
Source code : athena_utils/gmock/source/gmock-1.7.0.tar.gz
Cloned from https://github.com/paulsapps/gmock-1.7.0.git

Both Google Mock and Test are built together
MacOSX
	- Use CMake with source directory gmock-1.7.0 and Unix Makefiles.
	- Tick BUILD_SHARED_LIBS
	- In Terminal ls to build directory and run make.
	- Copy Google Test includes from gmock-1.7.0/gtest/include to athena_utils/gmock/gmock-1.7.0-Mac32/include directory.
	- Copy Google Mock includes from gmock-1.7.0/include to athena_utils/gmock/gmock-1.7.0-Mac32/include directory.
	- Copy Google Test dylib library from cmake_build/gtest to athena_utils/gmock/gmock-1.7.0-Mac32/lib directory.
	- Copy Google Mock dylib library from cmake_build to athena_utils/gmock/gmock-1.7.0-Mac32/lib directory.

-----------
libupnp v1.6.22
Webpage : http://pupnp.sourceforge.net

MacOSX
	 - In Terminal cd to unpacked source directory
	 - ./configure --prefix==~/athena_utils/libupnp/libupnp-1.6.25-Mac32
	 - make
	 - make install

-----------
libxml2 
Webpage : http://www.xmlsoft.org/index.html
Source code : ftp://xmlsoft.org/libxml2/libxml2-2.9.8.tar.gz	 

MacOSX
	 - In Terminal cd to unpacked source directory
	 - ./configure --prefix==~/athena_utils/libxml2/libxml2-2.9.8-Mac32
	 - make
	 - make install

------------
MonkeyAudio
Webpage : http://www.monkeysaudio.com/developers.html

The latest version of the Monkey Audio SDK uses VS2017 project. This makes it very
problematic across multiple platforms. There is an associated codec 'monkeyomega' 
that can be used which has disabled by default.

------------
Musepack
Webpage : https://www.musepack.net/index.php?pg=src

MacOSX
	- Use CMake with Unix Makefiles to build libcuefile.
	- Copy dylib and include from libcuefile into ~/athena_utils/musepack/musepack-r475-Mac32
	- Use CMake with Unix Makefiles to build libreplaygain.
	- Copy dylib and include from libreplaygain into ~/athena_utils/musepack/musepack-r475-Mac32
	- Use CMake to build musepack_src
	- Set CUEFILE_INCLUDE_DIR to ~/athena_utils/musepack/musepack-r475-Mac32/include 
	- Set REPLAY_GAIN_INCLUDE_DIR to ~/athena_utils/musepack/musepack-r475-Mac32/include 
	- Set CUEFILE_LIBRARY to ~/athena_utils/musepack/musepack-r475-Mac32/lib/libcuefile.dylib
	- Set REPLAY_GAIN_LIBRARY to ~/athena_utils/musepack/musepack-r475-Mac32/lib/libreplaygain.dylib
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
	
