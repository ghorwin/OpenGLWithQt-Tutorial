@echo off

:: cl compiler paths
call "c:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64

:: path to Qt
set CMAKE_PREFIX_PATH=c:\Qt\5.11.3\msvc2015_64

:: configure in release mode, to build with JOM
cmake -G "NMake Makefiles JOM" -DCMAKE_BUILD_TYPE:String="Release" ..  

:: build
jom



