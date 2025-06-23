@echo off
echo Building ZipFun library for Windows...
g++ -std=c++17 -Wall -Wextra -c zipfun.cpp -o zipfun.o
ar rcs libzipfun.a zipfun.o
echo Library built successfully!
echo.
pause 