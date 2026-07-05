@echo off
setlocal

REM Qt 6.10.2\msvc2022_64\bin\lupdate.exe is broken (Entry Point Not Found - DLL mismatch).
REM Reinstall "Qt Linguist Tools" for 6.10.2 via Qt Maintenance Tool to fix it.
REM Until then, use a working lupdate/lrelease from another kit.

set "QT_BIN="
if defined QT_LINGUIST_BIN if exist "%QT_LINGUIST_BIN%\lupdate.exe" set "QT_BIN=%QT_LINGUIST_BIN%"
if not defined QT_BIN if exist "C:\Development\Qt\5.15.2\msvc2019\bin\lupdate.exe" set "QT_BIN=C:\Development\Qt\5.15.2\msvc2019\bin"
if not defined QT_BIN if exist "C:\Development\Qt\Tools\QtDesignStudio\qt6_design_studio_reduced_version\bin\lupdate.exe" set "QT_BIN=C:\Development\Qt\Tools\QtDesignStudio\qt6_design_studio_reduced_version\bin"
if not defined QT_BIN if exist "C:\Development\Qt\6.10.2\msvc2022_64\bin\lupdate.exe" set "QT_BIN=C:\Development\Qt\6.10.2\msvc2022_64\bin"

if not defined QT_BIN (
    echo Qt Linguist tools not found. Install Qt or set QT_LINGUIST_BIN.
    pause
    exit /b 1
)

echo Using Qt tools from: %QT_BIN%
set "PATH=%QT_BIN%;%PATH%"
set "ROOT=%~dp0"

echo.
echo [Restaurant2] lupdate...
cd /d "%ROOT%Restaurant2"
lupdate -noobsolete Restaurant.pro -ts Restaurant.am.ts
if errorlevel 1 goto :error

echo [Restaurant2] lrelease...
lrelease Restaurant.am.ts -qm Restaurant.am.qm
if errorlevel 1 goto :error

echo.
echo [Resort] lupdate...
cd /d "%ROOT%Resort"
lupdate -noobsolete Resort.pro -ts Resort.ts
if errorlevel 1 goto :error

echo [Resort] lrelease...
lrelease Resort.ts -qm Resort.qm
if errorlevel 1 goto :error

echo.
echo Translations updated successfully.
exit /b 0

:error
echo.
echo Translation update failed.
pause
exit /b 1
