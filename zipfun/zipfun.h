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
    size_t function_size;
    void* original_function_ptr;
    void* function_copy_ptr;
    
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

#endif // ZIPFUN_H 