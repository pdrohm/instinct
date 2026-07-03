@echo off
setlocal EnableDelayedExpansion

REM ============================================================
REM  FirstLife - Update & Build
REM  Pulls the latest from git, compiles the editor target,
REM  and (optionally) launches the Unreal Editor.
REM
REM  Usage:
REM    update-and-build.bat          pull + build
REM    update-and-build.bat run      pull + build + launch editor
REM    update-and-build.bat regen    pull + regenerate project files + build
REM
REM  Override the engine location if it ever moves:
REM    set UE_ROOT=D:\Epic\UE_5.6 & update-and-build.bat
REM ============================================================

REM --- Paths -------------------------------------------------
set "PROJECT_ROOT=%~dp0.."
pushd "%PROJECT_ROOT%"

if not defined UE_ROOT set "UE_ROOT=S:\Program Files\Epic Games\UE_5.6"
set "BUILD_BAT=%UE_ROOT%\Engine\Build\BatchFiles\Build.bat"
set "UBT=%UE_ROOT%\Engine\Build\BatchFiles\RunUBT.bat"
set "EDITOR=%UE_ROOT%\Engine\Binaries\Win64\UnrealEditor.exe"
set "UPROJECT=%PROJECT_ROOT%\FirstLife.uproject"
set "TARGET=FirstLifeEditor"
set "PLATFORM=Win64"
set "CONFIG=Development"

if not exist "%BUILD_BAT%" (
	echo [ERROR] Could not find the engine build tool at:
	echo         "%BUILD_BAT%"
	echo         Set UE_ROOT to your UE 5.6 install and retry.
	goto :fail
)

REM --- 1. Pull latest ----------------------------------------
echo.
echo === [1/3] Pulling latest from git ===
git pull --ff-only
if errorlevel 1 (
	echo.
	echo [ERROR] git pull failed ^(local changes or diverged branch^).
	echo         Commit/stash your work, or resolve manually, then retry.
	goto :fail
)

REM --- 2. (optional) Regenerate project files ----------------
if /I "%~1"=="regen" (
	echo.
	echo === Regenerating project files ===
	"%UBT%" -projectfiles -project="%UPROJECT%" -game -engine
)

REM --- 3. Build ----------------------------------------------
echo.
echo === [2/3] Building %TARGET% ^| %PLATFORM% ^| %CONFIG% ===
call "%BUILD_BAT%" %TARGET% %PLATFORM% %CONFIG% -Project="%UPROJECT%" -WaitMutex -FromMsBuild
if errorlevel 1 (
	echo.
	echo [ERROR] Build failed. Scroll up for the first compiler error.
	goto :fail
)

echo.
echo === [3/3] Build succeeded ===

REM --- Optional: launch the editor ---------------------------
if /I "%~1"=="run" (
	echo Launching Unreal Editor...
	start "" "%EDITOR%" "%UPROJECT%"
)

popd
echo.
echo Done.
endlocal
exit /b 0

:fail
popd
echo.
pause
endlocal
exit /b 1
