@echo off

set "ROOT=%~dp0"
set "ROOT=%ROOT:~0,-1%"

set "MAKE=%ROOT%\Toolchain\llvm-mingw\20260602\busybox\bin\make.exe"

call "%MAKE%" -f "%ROOT%\Build\Rules\Makefile" %*
exit /b %ERRORLEVEL%
