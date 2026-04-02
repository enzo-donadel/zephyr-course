@echo off
setlocal

set SCRIPT_DIR=%~dp0
set ROOT_DIR=%SCRIPT_DIR%..\..\
set VENV_ACTIVATE=%ROOT_DIR%..\..\.venv\Scripts\activate.bat

if not exist "%VENV_ACTIVATE%" (
    echo ERROR: Could not find venv activate script.
    echo Expected at: %VENV_ACTIVATE%
    exit /b 1
)

call "%VENV_ACTIVATE%"

cd /d "%ROOT_DIR%"

REM Check build output exists
if not exist "%ROOT_DIR%build\zephyr\zephyr.hex" (
    echo ERROR: Build output not found. Run build.bat first.
    exit /b 1
)

echo.
west flash -d build %*
if %ERRORLEVEL% neq 0 (
    echo.
    echo FLASH FAILED.
    exit /b %ERRORLEVEL%
)

echo.
echo FLASH SUCCEEDED.
