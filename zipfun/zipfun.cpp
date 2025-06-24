#include "zipfun.h"
#include <cstring>
#include <windows.h>
#include <future>

// Simple XOR encryption key (in production, use a more sophisticated encryption)
const unsigned char ENCRYPTION_KEY[] = {0x5A, 0x3F, 0x8B, 0x2E, 0x7C, 0x1A, 0x9D, 0x4F};

// --- Helper: allocate executable memory ---
template<typename R, typename... Args>
void* ZipFun<R(Args...)>::allocate_executable_memory(size_t size) {
    return VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
}
template<typename... Args>
void* ZipFun<void(Args...)>::allocate_executable_memory(size_t size) {
    return VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
}

// --- Helper: encrypt ---
template<typename R, typename... Args>
void ZipFun<R(Args...)>::encrypt(const void* code, size_t size) {
    encrypted_code.resize(size);
    for (size_t i = 0; i < size; ++i)
        encrypted_code[i] = static_cast<const unsigned char*>(code)[i] ^ ENCRYPTION_KEY[i % 8];
}
template<typename... Args>
void ZipFun<void(Args...)>::encrypt(const void* code, size_t size) {
    encrypted_code.resize(size);
    for (size_t i = 0; i < size; ++i)
        encrypted_code[i] = static_cast<const unsigned char*>(code)[i] ^ ENCRYPTION_KEY[i % 8];
}

// --- Helper: decrypt ---
template<typename R, typename... Args>
void ZipFun<R(Args...)>::decrypt() {
    std::lock_guard<std::mutex> lock(mtx);
    if (!exec_mem) exec_mem = allocate_executable_memory(func_size);
    for (size_t i = 0; i < func_size; ++i)
        static_cast<unsigned char*>(exec_mem)[i] = encrypted_code[i] ^ ENCRYPTION_KEY[i % 8];
}
template<typename... Args>
void ZipFun<void(Args...)>::decrypt() {
    std::lock_guard<std::mutex> lock(mtx);
    if (!exec_mem) exec_mem = allocate_executable_memory(func_size);
    for (size_t i = 0; i < func_size; ++i)
        static_cast<unsigned char*>(exec_mem)[i] = encrypted_code[i] ^ ENCRYPTION_KEY[i % 8];
}

// --- Helper: re-encrypt and clear ---
template<typename R, typename... Args>
void ZipFun<R(Args...)>::re_encrypt() {
    std::lock_guard<std::mutex> lock(mtx);
    for (size_t i = 0; i < func_size; ++i)
        encrypted_code[i] = static_cast<unsigned char*>(exec_mem)[i] ^ ENCRYPTION_KEY[i % 8];
    clear_exec_mem();
}
template<typename... Args>
void ZipFun<void(Args...)>::re_encrypt() {
    std::lock_guard<std::mutex> lock(mtx);
    for (size_t i = 0; i < func_size; ++i)
        encrypted_code[i] = static_cast<unsigned char*>(exec_mem)[i] ^ ENCRYPTION_KEY[i % 8];
    clear_exec_mem();
}

template<typename R, typename... Args>
void ZipFun<R(Args...)>::clear_exec_mem() {
    if (exec_mem) {
        VirtualFree(exec_mem, 0, MEM_RELEASE);
        exec_mem = nullptr;
    }
}
template<typename... Args>
void ZipFun<void(Args...)>::clear_exec_mem() {
    if (exec_mem) {
        VirtualFree(exec_mem, 0, MEM_RELEASE);
        exec_mem = nullptr;
    }
}

// --- Constructor/Destructor ---
template<typename R, typename... Args>
ZipFun<R(Args...)>::ZipFun(FuncPtr func, size_t size)
    : func_size(size), original_func(func) {
    encrypt(reinterpret_cast<const void*>(func), size);
}
template<typename... Args>
ZipFun<void(Args...)>::ZipFun(FuncPtr func, size_t size)
    : func_size(size), original_func(func) {
    encrypt(reinterpret_cast<const void*>(func), size);
}

template<typename R, typename... Args>
ZipFun<R(Args...)>::~ZipFun() { clear_exec_mem(); }
template<typename... Args>
ZipFun<void(Args...)>::~ZipFun() { clear_exec_mem(); }

// --- Threaded call: decrypt, execute, re-encrypt ---
template<typename R, typename... Args>
R ZipFun<R(Args...)>::call(Args... args) {
    std::promise<void> dec_done, exec_done;
    auto fut_dec = dec_done.get_future();
    auto fut_exec = exec_done.get_future();
    std::thread t_dec([&]{ decrypt(); dec_done.set_value(); });
    fut_dec.wait();
    R result;
    std::thread t_exec([&]{
        using F = R(*)(Args...);
        result = reinterpret_cast<F>(exec_mem)(std::forward<Args>(args)...);
        exec_done.set_value();
    });
    fut_exec.wait();
    std::thread t_enc([&]{ re_encrypt(); });
    t_dec.join(); t_exec.join(); t_enc.join();
    return result;
}
template<typename... Args>
void ZipFun<void(Args...)>::call(Args... args) {
    std::promise<void> dec_done, exec_done;
    auto fut_dec = dec_done.get_future();
    auto fut_exec = exec_done.get_future();
    std::thread t_dec([&]{ decrypt(); dec_done.set_value(); });
    fut_dec.wait();
    std::thread t_exec([&]{
        using F = void(*)(Args...);
        reinterpret_cast<F>(exec_mem)(std::forward<Args>(args)...);
        exec_done.set_value();
    });
    fut_exec.wait();
    std::thread t_enc([&]{ re_encrypt(); });
    t_dec.join(); t_exec.join(); t_enc.join();
}

// Explicit template instantiations for linker
#define INSTANTIATE_ZIPFUN(R, ...) \
template class ZipFun<R(__VA_ARGS__)>;

// You can add explicit instantiations here as needed for your function types 