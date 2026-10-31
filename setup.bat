@echo off
setlocal EnableExtensions EnableDelayedExpansion

:main
set "ROOT=%~dp0"
set "ROOT=%ROOT:~0,-1%"

for %%I in ("%ROOT%") do set "ROOT_NAME=%%~nxI"

set "LLVM_VERSION=20260602"
set "LLVM_DST=%ROOT%\Toolchain\llvm-mingw\%LLVM_VERSION%"
set "LLVM_URL=https://github.com/mstorsjo/llvm-mingw/releases/download/%LLVM_VERSION%/llvm-mingw-%LLVM_VERSION%-msvcrt-x86_64.zip"
call :download_and_extract "LLVM" "%LLVM_VERSION%" "%LLVM_URL%" "%LLVM_DST%"
if errorlevel 1 exit /b %ERRORLEVEL%

set "ZIG_VERSION=0.15.2"
set "ZIG_DST=%ROOT%\Toolchain\zig\%ZIG_VERSION%"
set "ZIG_URL=https://ziglang.org/download/%ZIG_VERSION%/zig-x86_64-windows-%ZIG_VERSION%.zip"
call :download_and_extract "Zig" "%ZIG_VERSION%" "%ZIG_URL%" "%ZIG_DST%"
if errorlevel 1 exit /b %ERRORLEVEL%

set "EMSCRIPTEN_VERSION=6.0.0"
set "EMSDK_DST=%ROOT%\Toolchain\emsdk"
set "EMSDK_URL=https://github.com/emscripten-core/emsdk/archive/refs/heads/main.zip"
call :install_emscripten "Emscripten" "%EMSCRIPTEN_VERSION%" "%EMSDK_URL%" "%EMSDK_DST%"
if errorlevel 1 exit /b %ERRORLEVEL%

echo [OK] %ROOT_NAME% Installed.

exit /b 0

:install_emscripten
set "NAME=%~1"
set "VERSION=%~2"
set "URL=%~3"
set "DEST=%~4"

set "VERSION_FILE=%DEST%\.version"
if exist "%VERSION_FILE%" (
    for /f "delims=" %%A in (%VERSION_FILE%) do set "INSTALLED=%%A"
    set "INSTALLED=!INSTALLED: =!"
    if "!INSTALLED!"=="%VERSION%" (
        exit /b 0
    )
)
call :download_and_extract "%NAME%" "%VERSION%" "%URL%" "%DEST%"
pushd "%EMSDK_DST%"
call ./emsdk.bat install %VERSION%
call ./emsdk.bat activate %VERSION%
call ./emsdk_env.bat
call npm install --prefix "%ROOT%/Toolchain/Live-Server" live-server
popd
exit /b 0

:download_and_extract
set "NAME=%~1"
set "VERSION=%~2"
set "URL=%~3"
set "DEST=%~4"

set "VERSION_FILE=%DEST%\.version"
set "ZIP_FILE=%TEMP%\%NAME%.zip"

if exist "%VERSION_FILE%" (
    for /f "delims=" %%A in (%VERSION_FILE%) do set "INSTALLED=%%A"
    set "INSTALLED=!INSTALLED: =!"
    if "!INSTALLED!"=="%VERSION%" (
        exit /b 0
    ) else (
        echo [DEBUG] VERSION MISMATCH
    )
)

echo [INFO] Downloading %NAME% %VERSION%...
curl -L --progress-bar "%URL%" -o "%ZIP_FILE%"
if errorlevel 1 (
    echo [ERROR] Download failed.
    exit /b 1
)

echo [INFO] Extracting %NAME%...
if exist "%DEST%" rmdir /s /q "%DEST%"
mkdir "%DEST%"
tar -xf "%ZIP_FILE%" -C "%DEST%" --strip-components=1
if errorlevel 1 (
    echo [ERROR] Extraction failed.
    exit /b 1
)
echo %VERSION% > "%DEST%\.version"
del "%ZIP_FILE%"
exit /b 0