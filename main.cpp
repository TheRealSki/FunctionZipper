#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <ctime>
#include <conio.h>
#include <windows.h>

int main() {
    // Hide the cursor
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
    
    // Display the initial message
    std::cout << "You can't read this!" << std::endl;
    std::cout << "Press Enter to exit the application." << std::endl;
    std::cout << "Current time: ";
    
    // Loop to update time every second
    while (true) {
        // Get current time using time_t (more compatible)
        std::time_t now = std::time(nullptr);
        
        // Format and display the time on the same line
        std::cout << "\rCurrent time: " << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S") << std::flush;
        
        // Check if Enter key is pressed (non-blocking)
        if (_kbhit()) {
            int key = _getch();
            if (key == 13) { // Enter key
                break;
            }
        }
        
        // Wait for 1 second using sleep
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    // Restore the cursor
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
    
    std::cout << std::endl << "Thank you for using the time display application!" << std::endl;
    return 0;
} 