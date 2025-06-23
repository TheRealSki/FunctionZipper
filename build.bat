@echo off
echo Compiling the application...

echo Trying standard C++ version...
g++ -std=c++17 -Wall -Wextra -o time_display.exe main.cpp
if %ERRORLEVEL% EQU 0 (
    echo Standard version compilation successful!
    echo Running the application...
    echo.
    time_display.exe
    goto :end
)

echo Standard version failed, trying Windows-specific version...
g++ -std=c++17 -Wall -Wextra -o time_display.exe main_windows.cpp
if %ERRORLEVEL% EQU 0 (
    echo Windows version compilation successful!
    echo Running the application...
    echo.
    time_display.exe
    goto :end
)

echo.
echo Both compilation attempts failed!
echo.
echo To troubleshoot, try:
echo 1. Update your MSYS2/MinGW installation
echo 2. Check if g++ is properly installed
echo 3. Try running: g++ --version
echo.
pause

:end 