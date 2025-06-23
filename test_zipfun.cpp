#include <iostream>
#include "zipfun/zipfun.h"

// Example function to be encrypted
int add_numbers(int a, int b) {
    std::cout << "Function executed: " << a << " + " << b << std::endl;
    return a + b;
}
extern "C" void add_numbers_end() {}

int main() {
    std::cout << "Testing ZipFun library..." << std::endl;
    
    // Calculate function size using marker
    size_t add_numbers_size = (size_t)((char*)add_numbers_end - (char*)add_numbers);
    // Create ZipFun instance with the add_numbers function and its size
    ZipFun zip_add(reinterpret_cast<void*>(add_numbers), add_numbers_size);
    
    std::cout << "Function encrypted and saved to disk." << std::endl;
    
    // Test the encrypted function
    std::cout << "Decrypting function..." << std::endl;
    zip_add.decrypt_function();
    
    std::cout << "Executing function..." << std::endl;
    // Cast to function pointer and execute
    auto func = reinterpret_cast<int(*)(int, int)>(zip_add.get_function_ptr());
    int result = func(5, 3);
    
    std::cout << "Re-encrypting function..." << std::endl;
    zip_add.re_encrypt_function();
    
    std::cout << "Result: " << result << std::endl;
    std::cout << "Test completed successfully!" << std::endl;
    
    return 0;
} 