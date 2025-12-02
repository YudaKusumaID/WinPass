@echo off
echo ========================================
echo WinPass-Native Build Script
echo ========================================
echo.

echo [1/3] Compiling source files...
gcc -c src/*.c -Iinclude
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Compilation failed!
    exit /b 1
)

echo [2/3] Linking executable...
gcc *.o main.c -o WinPass.exe -Iinclude -lAdvapi32 -lShell32
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Linking failed!
    del *.o
    exit /b 1
)

echo [3/3] Cleaning up object files...
del *.o

echo.
echo ========================================
echo Build completed successfully!
echo Output: WinPass.exe
echo ========================================
