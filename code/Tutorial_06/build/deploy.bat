@echo off

:: path to Qt
set CMAKE_PREFIX_PATH=c:\Qt\5.11.3\msvc2015_64

:: deployment

:: copy dependencies
%CMAKE_PREFIX_PATH%\bin\windeployqt Tutorial_06.exe


