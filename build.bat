@echo off
echo Building ZipFun library...
cd zipfun
g++ -std=c++17 -Wall -Wextra -c zipfun.cpp -o zipfun.o
ar rcs libzipfun.a zipfun.o
cd ..

echo.
echo Compiling the main application...
g++ -std=c++17 -Wall -Wextra -o time_display.exe main.cpp zipfun/libzipfun.a
if %ERRORLEVEL% EQU 0 (
    echo Compilation successful!
    echo Running the application...
    echo.
    time_display.exe
) else (
    echo.
    echo Compilation failed!
    echo.
    echo To troubleshoot, try:
    echo 1. Check if all source files exist
    echo 2. Verify g++ is properly installed
    echo 3. Try running: g++ --version
    echo.
    pause
) 