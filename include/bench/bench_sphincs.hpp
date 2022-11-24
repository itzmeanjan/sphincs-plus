#pragma once
#include "sphincs.hpp"
#include <benchmark/benchmark.h>

// Benchmark SPHINCS+ Routines
namespace bench_sphincs {

// Benchmark SPHINCS+ keypair generation
template<const size_t n,
         const uint32_t h,
         const uint32_t d,
         const size_t w,
         const sphincs_hashing::variant v>
inline static void
keygen(benchmark::State& state)
{
  namespace utils = sphincs_utils;
  constexpr size_t pklen = utils::get_sphincs_pkey_len<n>();
  constexpr size_t sklen = utils::get_sphincs_skey_len<n>();

  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));

  for (auto _ : state) {
    sphincs::keygen<n, h, d, w, v>(skey, pkey);

    benchmark::DoNotOptimize(skey);
    benchmark::DoNotOptimize(pkey);
    benchmark::ClobberMemory();
  }

  std::free(pkey);
  std::free(skey);
}

}
