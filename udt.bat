@echo off

set "ROOT=%~dp0"
set "ROOT=%ROOT:~0,-1%"

call make.exe -f "%ROOT%\Build\Rules\Makefile" %*
exit /b %ERRORLEVEL%
