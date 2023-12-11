#pragma once

#if defined __x86_64__

#include <cstdint>
#include <emmintrin.h>
#include <x86intrin.h>

// x86_64 architecture specific compiler built-in routine for computing CPU
// cycles spent executing some instructions. For example
//
// start = cpu_cycles()
// {
// ... bunch of instructions ...
// }
// end = cpu_cycles()
//
// CPU cycles spent executing code block = end - start
static inline uint64_t
cpu_cycles()
{
  _mm_mfence();
  return __rdtsc();
}

#endif
