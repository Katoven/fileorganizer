@echo off
@set SETUP_MSVC_2019="C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvarsall.bat"

call %SETUP_MSVC_2019% x64
cmake -DCMAKE_TOOLCHAIN_FILE=toolchain.msvc.cmake -G"Visual Studio 16 2019" -A x64 -H. -Bbuild

pause
