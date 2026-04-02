@echo off
setlocal

set SCRIPT_DIR=%~dp0
set ROOT_DIR=%SCRIPT_DIR%..\..\
set VENV_ACTIVATE=%ROOT_DIR%..\..\.venv\Scripts\activate.bat

if not exist "%VIRTUAL_ENV%" (
    echo Trying to enable .venv at zephyr root (asuming West T2/T3 topology of workspace)
    if not exist "%VENV_ACTIVATE%" (
        echo ERROR: Could not find venv activate script.
        exit /b 1
    )
    call "%VENV_ACTIVATE%"
)


cd /d "%ROOT_DIR%"
set Board=sam_v71_xult/samv71q21b
echo.
echo Building...
echo Board: %Board%
echo Output: %ROOT_DIR%build\
echo.

west build -p auto -d build -b %Board% . -- -DCMAKE_EXPORT_COMPILE_COMMANDS=ON %*

if %ERRORLEVEL% neq 0 (
    echo.
    echo BUILD FAILED.
    exit /b %ERRORLEVEL%
)

echo.
echo BUILD SUCCEEDED.