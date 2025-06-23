#ifndef ZIPFUN_H
#define ZIPFUN_H

#include <cstddef>
#include <string>
#include <vector>

class ZipFun {
private:
    std::string function_id;
    std::vector<unsigned char> encrypted_code;
    std::vector<unsigned char> function_copy;
    void* original_function_ptr;
    void* function_copy_ptr;
    size_t function_size;
    
    // Helper methods
    std::string generate_function_id(const void* func_ptr);
    std::string get_encrypted_file_path();
    void save_encrypted_function();
    void load_encrypted_function();
    void* allocate_executable_memory(size_t size);

public:
    // Constructor takes a function pointer and function size, then encrypts it
    ZipFun(void* func_ptr, size_t func_size);
    
    // Destructor
    ~ZipFun();
    
    // Encryption/decryption methods
    void encrypt_function(const void* func_ptr, size_t size);
    void decrypt_function();
    void re_encrypt_function();
    
    // Get the copy function pointer for casting
    void* get_function_ptr() const { return function_copy_ptr; }
};

// Macro to declare a function with ZipFun wrapper
#define ZIPFUN_DECLARE(name, ret_type, ...) \
    ret_type name(__VA_ARGS__); \
    extern "C" void name##_end(); \
    extern ZipFun name##_zip

// Macro to define a function with ZipFun wrapper
#define ZIPFUN_DEFINE(name, ret_type, ...) \
    ret_type name(__VA_ARGS__); \
    extern "C" void name##_end() {} \
    ZipFun name##_zip(reinterpret_cast<void*>(name), (size_t)((char*)name##_end - (char*)name)); \
    ret_type name(__VA_ARGS__)

// Simple macro for single-file usage
#define ZIPFUN(name, ret_type, ...) \
    ret_type name(__VA_ARGS__); \
    extern "C" void name##_end() {} \
    ret_type name(__VA_ARGS__)

#endif // ZIPFUN_H 