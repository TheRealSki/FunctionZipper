#ifndef ZIPFUN_H
#define ZIPFUN_H

#include <cstddef>
#include <string>
#include <vector>
#include <thread>
#include <functional>
#include <future>
#include <mutex>
#include <type_traits>

// Helper for function size (must be provided by user or macro)
constexpr size_t get_func_size(const void* start, const void* end) {
    return static_cast<const char*>(end) - static_cast<const char*>(start);
}

// Templated ZipFun class
// Signature: R(Args...)
template<typename FuncSig>
class ZipFun;

// Specialization for functions returning non-void
// (R != void)
template<typename R, typename... Args>
class ZipFun<R(Args...)> {
    std::vector<unsigned char> encrypted_code;
    void* exec_mem = nullptr;
    size_t func_size;
    std::mutex mtx;
    using FuncPtr = R(*)(Args...);
    FuncPtr original_func;

    static constexpr unsigned char ENCRYPTION_KEY[8] = {0x5A, 0x3F, 0x8B, 0x2E, 0x7C, 0x1A, 0x9D, 0x4F};

    void* allocate_executable_memory(size_t size);
    void encrypt(const void* code, size_t size);
    void decrypt();
    void re_encrypt();
    void clear_exec_mem();

public:
    ZipFun(FuncPtr func, size_t size);
    ~ZipFun();
    R call(Args... args);
};

// Specialization for void-returning functions
// (R == void)
template<typename... Args>
class ZipFun<void(Args...)> {
    std::vector<unsigned char> encrypted_code;
    void* exec_mem = nullptr;
    size_t func_size;
    std::mutex mtx;
    using FuncPtr = void(*)(Args...);
    FuncPtr original_func;

    static constexpr unsigned char ENCRYPTION_KEY[8] = {0x5A, 0x3F, 0x8B, 0x2E, 0x7C, 0x1A, 0x9D, 0x4F};

    void* allocate_executable_memory(size_t size);
    void encrypt(const void* code, size_t size);
    void decrypt();
    void re_encrypt();
    void clear_exec_mem();

public:
    ZipFun(FuncPtr func, size_t size);
    ~ZipFun();
    void call(Args... args);
};

// Macro to help declare a function and its size for ZipFun
#define ZIPFUN_WRAP(name, ret_type, ...) \
    ret_type name(__VA_ARGS__); \
    extern "C" void name##_end(); \
    static ZipFun<ret_type(__VA_ARGS__)> name##_zip(name, get_func_size((void*)name, (void*)name##_end)); \
    ret_type name(__VA_ARGS__)

#endif // ZIPFUN_H 