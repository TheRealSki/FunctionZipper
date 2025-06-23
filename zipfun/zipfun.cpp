#include "zipfun.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <algorithm>
#include <random>
#include <windows.h>

// Simple XOR encryption key (in production, use a more sophisticated encryption)
const unsigned char ENCRYPTION_KEY[] = {0x5A, 0x3F, 0x8B, 0x2E, 0x7C, 0x1A, 0x9D, 0x4F};

ZipFun::ZipFun(void* func_ptr, size_t func_size) : original_function_ptr(func_ptr), function_size(func_size) {
    function_id = generate_function_id(original_function_ptr);
    
    // Allocate executable memory for function copy
    function_copy_ptr = allocate_executable_memory(function_size);
    
    // Copy the original function to writable memory
    memcpy(function_copy_ptr, original_function_ptr, function_size);
    
    // Encrypt the function and save to disk
    encrypt_function(function_copy_ptr, function_size);
    save_encrypted_function();
}

ZipFun::~ZipFun() {
    // Free allocated memory
    if (function_copy_ptr) {
        VirtualFree(function_copy_ptr, 0, MEM_RELEASE);
    }
}

void* ZipFun::allocate_executable_memory(size_t size) {
    return VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
}

std::string ZipFun::generate_function_id(const void* func_ptr) {
    std::stringstream ss;
    ss << "func_" << std::hex << reinterpret_cast<uintptr_t>(func_ptr);
    return ss.str();
}

std::string ZipFun::get_encrypted_file_path() {
    return function_id + ".enc";
}

void ZipFun::encrypt_function(const void* func_ptr, size_t size) {
    const unsigned char* code = static_cast<const unsigned char*>(func_ptr);
    encrypted_code.resize(size);
    
    // Simple XOR encryption
    for (size_t i = 0; i < size; ++i) {
        encrypted_code[i] = code[i] ^ ENCRYPTION_KEY[i % sizeof(ENCRYPTION_KEY)];
    }
}

void ZipFun::decrypt_function() {
    // Load encrypted function from disk
    load_encrypted_function();
    
    // Decrypt the function in the copy memory
    unsigned char* func_ptr = static_cast<unsigned char*>(function_copy_ptr);
    for (size_t i = 0; i < function_size; ++i) {
        func_ptr[i] = encrypted_code[i] ^ ENCRYPTION_KEY[i % sizeof(ENCRYPTION_KEY)];
    }
}

void ZipFun::re_encrypt_function() {
    // Re-encrypt the function from the copy memory
    unsigned char* func_ptr = static_cast<unsigned char*>(function_copy_ptr);
    for (size_t i = 0; i < function_size; ++i) {
        encrypted_code[i] = func_ptr[i] ^ ENCRYPTION_KEY[i % sizeof(ENCRYPTION_KEY)];
    }
    
    // Save back to disk
    save_encrypted_function();
}

void ZipFun::save_encrypted_function() {
    std::ofstream file(get_encrypted_file_path(), std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(encrypted_code.data()), encrypted_code.size());
        file.close();
    }
}

void ZipFun::load_encrypted_function() {
    std::ifstream file(get_encrypted_file_path(), std::ios::binary);
    if (file.is_open()) {
        file.seekg(0, std::ios::end);
        size_t file_size = file.tellg();
        file.seekg(0, std::ios::beg);
        
        encrypted_code.resize(file_size);
        file.read(reinterpret_cast<char*>(encrypted_code.data()), file_size);
        file.close();
    }
} 