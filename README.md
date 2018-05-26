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
