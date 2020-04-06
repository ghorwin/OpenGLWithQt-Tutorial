@echo off

:: path to Qt
set CMAKE_PREFIX_PATH=c:\Qt\5.11.3\msvc2015_64

:: deployment
mkdir vc_x64_deploy
copy vc_x64_release\Tutorial_06.exe vc_x64_deploy

:: copy dependencies
%CMAKE_PREFIX_PATH%\bin\windeployqt vc_x64_deploy\Tutorial_06.exe


