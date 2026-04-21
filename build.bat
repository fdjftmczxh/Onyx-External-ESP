@echo off
title Onyx - BUILD
color 0B
setlocal enabledelayedexpansion

:: ============================================================
::  CONFIGURATION — Edit these values to match your setup
:: ============================================================

:: Path to your Android NDK root folder
set "ANDROID_NDK_HOME=C:\Users\YOUR-NAME\AppData\Local\Android\Sdk\ndk\30.0.x

:: Target Android ABI (e.g. x86_64, arm64-v8a, armeabi-v7a)
set "ANDROID_ABI=x86_64"

:: Minimum Android API level
set "ANDROID_API=24"

:: ============================================================
::  INTERNAL — Do not edit below this line
:: ============================================================

set "BUILD_DIR=build"
set "TOOLCHAIN_FILE=%ANDROID_NDK_HOME%\build\cmake\android.toolchain.cmake"
set "NDK_MAKE=%ANDROID_NDK_HOME%\prebuilt\windows-x86_64\bin\make.exe"

:: ============================================================
echo [*] Validating environment...
:: ============================================================
if not exist "%TOOLCHAIN_FILE%" (
    echo [!] ERROR: Toolchain file not found at:
    echo     %TOOLCHAIN_FILE%
    echo.
    echo     Check that ANDROID_NDK_HOME is set correctly in this script.
    pause
    exit /b 1
)
if not exist "%NDK_MAKE%" (
    echo [!] ERROR: NDK make.exe not found at:
    echo     %NDK_MAKE%
    echo.
    echo     Check that ANDROID_NDK_HOME is set correctly in this script.
    pause
    exit /b 1
)
echo [+] Environment OK.
echo     ABI: %ANDROID_ABI%   API: android-%ANDROID_API%   NDK: %ANDROID_NDK_HOME%
echo.

:: ============================================================
echo [*] Preparing build directory...
:: ============================================================
if /I "%~1"=="clean" (
    echo [*] Clean flag detected — removing previous build...
    if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
    echo [+] Old build removed.
)
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"
echo [+] Build directory ready.
echo.

:: ============================================================
echo [*] Configuring CMake...
:: ============================================================
cmake .. -G "MinGW Makefiles" ^
    -DCMAKE_MAKE_PROGRAM="%NDK_MAKE%" ^
    -DCMAKE_TOOLCHAIN_FILE="%TOOLCHAIN_FILE%" ^
    -DANDROID_ABI="%ANDROID_ABI%" ^
    -DANDROID_PLATFORM="android-%ANDROID_API%" ^
    -DANDROID_PIE=ON ^
    -DCMAKE_BUILD_TYPE=Release

if %errorlevel% neq 0 (
    echo.
    echo [!] ERROR: CMake configuration failed. Aborting.
    cd ..
    pause
    exit /b %errorlevel%
)
echo [+] CMake configuration successful.
echo.

:: ============================================================
echo [*] Compiling onyx_external...
:: ============================================================
cmake --build . --config Release -j %NUMBER_OF_PROCESSORS%

if %errorlevel% neq 0 (
    echo.
    echo [!] ERROR: Compilation failed. Aborting.
    cd ..
    pause
    exit /b %errorlevel%
)

cd ..
echo.
echo [+] Build complete. Binary located at: %BUILD_DIR%\src\onyx_external
echo.
echo [*] Run push.bat to deploy to your device.
pause