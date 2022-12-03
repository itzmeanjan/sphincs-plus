#pragma once
#include "sphincs.hpp"
#include "x86_64_cpu_cycles.hpp"
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

#if defined __x86_64__
  uint64_t total_cycles = 0ul;
#endif

  for (auto _ : state) {
#if defined __x86_64__
    const uint64_t start = cpu_cycles();
#endif

    sphincs::keygen<n, h, d, w, v>(skey, pkey);

#if defined __x86_64__
    const uint64_t end = cpu_cycles();
    total_cycles += (end - start);
#endif

    benchmark::DoNotOptimize(skey);
    benchmark::DoNotOptimize(pkey);
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(state.iterations());

#if defined __x86_64__
  total_cycles /= static_cast<uint64_t>(state.iterations());
  state.counters["average_cpu_cycles"] = static_cast<double>(total_cycles);
#endif

  std::free(pkey);
  std::free(skey);
}

// Benchmark SPHINCS+ signing algorithm
template<const size_t n,
         const uint32_t h,
         const uint32_t d,
         const uint32_t a,
         const uint32_t k,
         const size_t w,
         const sphincs_hashing::variant v,
         const bool randomize = false>
inline static void
sign(benchmark::State& state)
{
  namespace utils = sphincs_utils;
  constexpr size_t pklen = utils::get_sphincs_pkey_len<n>();
  constexpr size_t sklen = utils::get_sphincs_skey_len<n>();
  constexpr size_t siglen = utils::get_sphincs_sig_len<n, h, d, a, k, w>();
  constexpr size_t mlen = 32;

  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));
  uint8_t* msg = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* sig = static_cast<uint8_t*>(std::malloc(siglen));

  sphincs_utils::random_data<uint8_t>(msg, mlen);
  sphincs::keygen<n, h, d, w, v>(skey, pkey);

#if defined __x86_64__
  uint64_t total_cycles = 0ul;
#endif

  for (auto _ : state) {
#if defined __x86_64__
    const uint64_t start = cpu_cycles();
#endif

    sphincs::sign<n, h, d, a, k, w, v, randomize>(msg, mlen, skey, sig);

#if defined __x86_64__
    const uint64_t end = cpu_cycles();
    total_cycles += (end - start);
#endif

    benchmark::DoNotOptimize(msg);
    benchmark::DoNotOptimize(skey);
    benchmark::DoNotOptimize(sig);
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(state.iterations());

#if defined __x86_64__
  total_cycles /= static_cast<uint64_t>(state.iterations());
  state.counters["average_cpu_cycles"] = static_cast<double>(total_cycles);
#endif

  std::free(pkey);
  std::free(skey);
  std::free(msg);
  std::free(sig);
}

// Benchmark SPHINCS+ signature verification algorithm
template<const size_t n,
         const uint32_t h,
         const uint32_t d,
         const uint32_t a,
         const uint32_t k,
         const size_t w,
         const sphincs_hashing::variant v,
         const bool randomize = false>
inline static void
verify(benchmark::State& state)
{
  namespace utils = sphincs_utils;
  constexpr size_t pklen = utils::get_sphincs_pkey_len<n>();
  constexpr size_t sklen = utils::get_sphincs_skey_len<n>();
  constexpr size_t siglen = utils::get_sphincs_sig_len<n, h, d, a, k, w>();
  constexpr size_t mlen = 32;

  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));
  uint8_t* msg = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* sig = static_cast<uint8_t*>(std::malloc(siglen));

  sphincs_utils::random_data<uint8_t>(msg, mlen);
  sphincs::keygen<n, h, d, w, v>(skey, pkey);
  sphincs::sign<n, h, d, a, k, w, v, randomize>(msg, mlen, skey, sig);

#if defined __x86_64__
  uint64_t total_cycles = 0ul;
#endif

  for (auto _ : state) {
#if defined __x86_64__
    const uint64_t start = cpu_cycles();
#endif

    const bool flg = sphincs::verify<n, h, d, a, k, w, v>(msg, mlen, sig, pkey);

#if defined __x86_64__
    const uint64_t end = cpu_cycles();
    total_cycles += (end - start);
#endif

    benchmark::DoNotOptimize(flg);
    benchmark::DoNotOptimize(msg);
    benchmark::DoNotOptimize(sig);
    benchmark::DoNotOptimize(pkey);
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(state.iterations());

#if defined __x86_64__
  total_cycles /= static_cast<uint64_t>(state.iterations());
  state.counters["average_cpu_cycles"] = static_cast<double>(total_cycles);
#endif

  std::free(pkey);
  std::free(skey);
  std::free(msg);
  std::free(sig);
}

}
