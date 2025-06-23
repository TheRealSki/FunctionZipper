# FunctionZipper
C/C++ function which wraps a function.  When the function is executed, it performs three things: 1) Decrypts the function, 2) Executes the function, and 3) Re-encrypts the function.

## ZipFun Library

The ZipFun library provides a class that can encrypt, execute, and re-encrypt functions at runtime. This is the core functionality described in the project description.

### Features
- **Function Encryption**: Encrypts function code and stores it on disk
- **Runtime Execution**: Decrypts, executes, and re-encrypts functions on demand
- **Template Support**: Works with any function signature
- **Minimal Overhead**: Optimized for minimal execution time impact
- **Original Function Protection**: Original function is never modified or called directly

### Usage Example
```cpp
#include "zipfun/zipfun.h"

int add_numbers(int a, int b) {
    return a + b;
}

int main() {
    // Create ZipFun instance with function pointer
    ZipFun zip_add(add_numbers);
    
    // Execute the encrypted function
    int result = zip_add.RunFunction(5, 3); // Returns 8
    return 0;
}
```

### How It Works
1. **Initialization**: ZipFun copies and encrypts the function code to disk
2. **Execution**: When RunFunction is called:
   - Function is decrypted from disk
   - Function is executed with provided arguments
   - Function is re-encrypted and saved back to disk
3. **Protection**: Original function remains untouched

## Time Display Application

This repository also includes a simple console application that demonstrates both the ZipFun library and displays a message with the current time updating every second.

### Features
- Displays "You can't read this!" message
- Shows current time in YYYY-MM-DD HH:MM:SS format
- Updates time every second on a single line (overwrites previous time)
- Hides cursor during execution for cleaner display
- Demonstrates ZipFun library functionality
- Displays friendly instructions to press Enter to exit
- Graceful exit with thank you message and cursor restoration
- Runs continuously until user presses Enter

### How to Run

#### On Windows:
1. Make sure you have a C++ compiler installed (like MinGW-w64 or Visual Studio)
2. Double-click `build.bat` to compile and run the application
3. Or manually compile:
   ```bash
   cd zipfun
   g++ -std=c++17 -c zipfun.cpp -o zipfun.o
   ar rcs libzipfun.a zipfun.o
   cd ..
   g++ -std=c++17 -o time_display.exe main.cpp zipfun/libzipfun.a
   ```
4. Run: `time_display.exe`

#### On Linux/macOS:
1. Make sure you have g++ installed
2. Compile: `make` or use the manual commands above
3. Run: `./time_display`

### Using the Application
1. The application will start and display the initial message
2. ZipFun library demonstration will run
3. The cursor will be hidden for a cleaner display
4. The current time will be shown and update every second on the same line
5. Press **Enter** to exit the application
6. The cursor will be restored and a friendly thank you message will be displayed

### Stopping the Application
Press **Enter** to gracefully exit the application. The cursor will be automatically restored.

### Files
- `main.cpp` - Main application source with ZipFun demonstration
- `zipfun/zipfun.h` - ZipFun class header
- `zipfun/zipfun.cpp` - ZipFun class implementation
- `zipfun/Makefile` - Build script for Unix-like systems
- `zipfun/build.bat` - Windows build script for the library
- `Makefile` - For easy compilation on Unix-like systems
- `build.bat` - Windows batch file for compilation and execution
