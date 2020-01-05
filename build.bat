@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"
pushd build
cl -nologo -Zi -FC ../win32_platform.c /link user32.lib gdi32.lib -incremental:no
call win32_platform.exe
popd