# FunctionZipper
C/C++ function which wraps a function.  When the function is executed, it performs three things: 1) Decrypts the function, 2) Executes the function, and 3) Re-encrypts the function.

## Time Display Application

This repository also includes a simple console application that displays a message and shows the current time updating every second.

### Features
- Displays "You can't read this!" message
- Shows current time in YYYY-MM-DD HH:MM:SS format
- Updates time every second on a single line (overwrites previous time)
- Hides cursor during execution for cleaner display
- Displays friendly instructions to press Enter to exit
- Graceful exit with thank you message and cursor restoration
- Runs continuously until user presses Enter

### How to Run

#### On Windows:
1. Make sure you have a C++ compiler installed (like MinGW-w64 or Visual Studio)
2. Double-click `build.bat` to compile and run the application
3. Or manually compile: `g++ -std=c++17 -o time_display.exe main.cpp`
4. Run: `time_display.exe`

#### On Linux/macOS:
1. Make sure you have g++ installed
2. Compile: `make` or `g++ -std=c++17 -o time_display main.cpp`
3. Run: `./time_display`

### Using the Application
1. The application will start and display the initial message
2. The cursor will be hidden for a cleaner display
3. The current time will be shown and update every second on the same line
4. Press **Enter** to exit the application
5. The cursor will be restored and a friendly thank you message will be displayed

### Stopping the Application
Press **Enter** to gracefully exit the application. The cursor will be automatically restored.

### Files
- `main.cpp` - Source code for the time display application
- `Makefile` - For easy compilation on Unix-like systems
- `build.bat` - Windows batch file for compilation and execution
