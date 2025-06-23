#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include "zipfun/zipfun.h"

// Example function to be encrypted
int add_numbers(int a, int b) {
    return a + b;
}
extern "C" void add_numbers_end() {}

void print_time() {
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
}

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
    
    // Demonstrate ZipFun functionality
    std::cout << std::endl << "Testing ZipFun encryption..." << std::endl;
    
    // Create ZipFun instance after function is defined
    size_t add_numbers_size = (size_t)((char*)add_numbers_end - (char*)add_numbers);
    ZipFun add_numbers_zip(reinterpret_cast<void*>(add_numbers), add_numbers_size);
    
    // Test the encrypted function
    add_numbers_zip.decrypt_function();
    auto func = reinterpret_cast<int(*)(int, int)>(add_numbers_zip.get_function_ptr());
    int result = func(5, 3);
    add_numbers_zip.re_encrypt_function();
    
    std::cout << "Encrypted function result: 5 + 3 = " << result << std::endl;
    
    std::cout << "Press Enter to continue to time display..." << std::endl;
    std::cin.get();
    
    print_time();
    
    // Restore the cursor
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
    
    std::cout << std::endl << "Thank you for using the time display application!" << std::endl;
    return 0;
} 