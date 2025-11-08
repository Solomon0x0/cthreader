#pragma once
#include <thread>

#if defined(_MSC_VER)
#include <immintrin.h>
inline void CpuRelax() noexcept { _mm_pause(); } // x86/x64 MSVC
#elif defined(__x86_64__) || defined(__i386__)
#include <immintrin.h>
inline void CpuRelax() noexcept { _mm_pause(); } // GCC/Clang: _mm_pause
#elif defined(__aarch64__) || defined(__arm__)
inline void CpuRelax() noexcept { __asm__ __volatile__("yield"); }
#else
inline void CpuRelax() noexcept { std::this_thread::yield(); }
#endif
