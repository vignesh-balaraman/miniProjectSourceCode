@echo off
REM Quick Start Guide for Bank Transaction System (Windows)
REM This batch file helps compile and run the program

echo =========================================
echo  Bank Transaction System - Quick Start
echo  (Windows Version)
echo =========================================
echo.

REM Check if trans.c exists
if not exist "trans.c" (
    echo ERROR: trans.c not found in current directory!
    echo Please ensure you're in the correct directory:
    echo   cd path\to\miniProjectSourceCode
    pause
    exit /b 1
)

REM Step 1: Compile
echo [STEP 1] Compiling trans.c...
echo Command: gcc -o trans.exe trans.c -Wall -Wextra
echo.

gcc -o trans.exe trans.c -Wall -Wextra
if errorlevel 1 (
    echo ✗ Compilation FAILED!
    echo.
    echo Common solutions:
    echo 1. Ensure GCC is installed on your system
    echo    - Download MinGW: https://www.mingw-w64.org
    echo    - Or use Windows Subsystem for Linux (WSL)
    echo    - Or use Dev-C++ IDE
    echo.
    echo 2. Verify GCC is in your PATH:
    echo    - Open Command Prompt and type: gcc --version
    echo.
    echo 3. Alternative: Use Online GDB
    echo    - Visit: https://www.onlinegdb.com
    echo    - Paste your trans.c code there
    echo.
    pause
    exit /b 1
)

echo ✓ Compilation SUCCESSFUL!
echo.

REM Check if executable was created
if exist "trans.exe" (
    echo [STEP 2] Running the program...
    echo Command: trans.exe
    echo.
    echo =========================================
    echo.
    
    REM Run the program
    trans.exe
    
    echo.
    echo =========================================
    echo Program terminated successfully!
    echo.
    
    REM Check for generated files
    if exist "credit.dat" (
        echo ✓ Data file created: credit.dat
        dir credit.dat
    )
    
    if exist "accounts.txt" (
        echo ✓ Export file created: accounts.txt
        dir accounts.txt
    )
) else (
    echo ERROR: Compilation succeeded but executable not found!
    pause
    exit /b 1
)

pause
