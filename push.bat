@echo off
title Onyx - PUSH
color 0B

:: ============================================================
::  CONFIGURATION — Edit these values to match your setup
:: ============================================================

:: Path to your Android SDK platform-tools folder
:: Default (Windows): C:\Users\YOUR_NAME\AppData\Local\Android\Sdk\platform-tools
set "ADB_PATH=C:\Users\YOUR-NAME\AppData\Local\Android\Sdk\platform-tools"

:: ============================================================
::  INTERNAL — Do not edit below this line
:: ============================================================

set "DEVICE_DIR=/data/local/tmp"
set "BINARY_NAME=onyx_external"
set "BINARY_PATH=%~dp0build\src\%BINARY_NAME%"
set "PATH=%ADB_PATH%;%PATH%"

:: ============================================================
echo [*] Validating configuration...
:: ============================================================
if not exist "%ADB_PATH%\adb.exe" (
    echo [!] ERROR: adb.exe not found at:
    echo     %ADB_PATH%
    echo.
    echo     Check that ADB_PATH is set correctly in this script.
    pause
    exit /b 1
)
echo [+] Configuration OK.
echo.

:: ============================================================
echo [*] Initializing ADB server...
:: ============================================================
adb kill-server >nul 2>&1
timeout /t 1 /nobreak >nul
adb start-server >nul 2>&1

echo [*] Waiting for device/emulator...
adb wait-for-device
timeout /t 2 /nobreak >nul

:: ============================================================
echo [*] Verifying root access...
:: ============================================================
adb shell "su -c 'echo ROOT_OK'" 2>&1 | findstr "ROOT_OK" >nul
if errorlevel 1 (
    echo [!] ERROR: Root access not available. Aborting.
    pause
    exit /b 1
)
echo [+] Root access confirmed.
echo.

:: ============================================================
echo [*] Killing ALL running instances of %BINARY_NAME%...
:: ============================================================

:: Pass 1: graceful SIGTERM
adb shell "su -c 'pkill -15 -f %BINARY_NAME%'" >nul 2>&1
timeout /t 1 /nobreak >nul

:: Pass 2: force SIGKILL by name
adb shell "su -c 'pkill -9 -f %BINARY_NAME%'" >nul 2>&1
timeout /t 1 /nobreak >nul

:: Pass 3: kill by PID (catches renamed/zombie processes)
adb shell "su -c 'for pid in $(pidof %BINARY_NAME%); do kill -9 $pid; done'" >nul 2>&1
timeout /t 1 /nobreak >nul

:: Pass 4: nuclear — grep /proc for any remnant
adb shell "su -c 'for pid in $(ls /proc | grep -E ^[0-9]+$); do if [ -f /proc/$pid/cmdline ]; then cmd=$(cat /proc/$pid/cmdline 2>/dev/null); case $cmd in *%BINARY_NAME%*) kill -9 $pid 2>/dev/null;; esac; fi; done'" >nul 2>&1
timeout /t 1 /nobreak >nul

:: Verify no instances remain
adb shell "su -c 'pidof %BINARY_NAME%'" 2>&1 | findstr /r "[0-9]" >nul
if not errorlevel 1 (
    echo [!] WARNING: Could not terminate all instances. Continuing anyway...
) else (
    echo [+] All instances terminated.
)
echo.

:: ============================================================
echo [*] Removing old binary from device...
:: ============================================================
adb shell "su -c 'rm -f %DEVICE_DIR%/%BINARY_NAME%'" >nul 2>&1
echo [+] Old binary removed.
echo.

:: ============================================================
echo [*] Pushing fresh %BINARY_NAME% to %DEVICE_DIR%...
:: ============================================================
if not exist "%BINARY_PATH%" (
    echo [!] ERROR: Binary not found at:
    echo     %BINARY_PATH%
    echo.
    echo     Build the project first, then run this script again.
    pause
    exit /b 1
)
adb push "%BINARY_PATH%" %DEVICE_DIR%/
if errorlevel 1 (
    echo [!] ERROR: Push failed. Aborting.
    pause
    exit /b 1
)
echo [+] Push successful.
echo.

:: ============================================================
echo [*] Setting executable permissions...
:: ============================================================
adb shell "su -c 'chmod 755 %DEVICE_DIR%/%BINARY_NAME%'"
echo [+] Permissions set.
echo.

:: ============================================================
echo [*] Launching %BINARY_NAME% as root...
:: ============================================================
adb shell "su -c '%DEVICE_DIR%/%BINARY_NAME%'"
echo.
echo [*] Session ended.
pause