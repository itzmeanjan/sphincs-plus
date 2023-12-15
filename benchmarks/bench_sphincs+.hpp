#pragma once
#include "sphincs+.hpp"
#include "x86_64_cpu_cycles.hpp"
#include <benchmark/benchmark.h>
#include <cassert>

// Benchmark SPHINCS+ Routines
namespace bench_sphincs_plus {

// Benchmark SPHINCS+ keypair generation
template<const size_t n, const uint32_t h, const uint32_t d, const size_t w, const sphincs_plus_hashing::variant v>
static inline void
keygen(benchmark::State& state)
{
  namespace utils = sphincs_plus_utils;
  constexpr size_t pklen = utils::get_sphincs_pkey_len<n>();
  constexpr size_t sklen = utils::get_sphincs_skey_len<n>();

  uint8_t* sk_seed = static_cast<uint8_t*>(std::malloc(n));
  uint8_t* sk_prf = static_cast<uint8_t*>(std::malloc(n));
  uint8_t* pk_seed = static_cast<uint8_t*>(std::malloc(n));
  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));

  sphincs_plus_utils::random_data<uint8_t>(sk_seed, n);
  sphincs_plus_utils::random_data<uint8_t>(sk_prf, n);
  sphincs_plus_utils::random_data<uint8_t>(pk_seed, n);

#ifdef __x86_64__
  uint64_t total_cycles = 0ul;
#endif

  for (auto _ : state) {
#ifdef __x86_64__
    const uint64_t start = cpu_cycles();
#endif

    sphincs_plus::keygen<n, h, d, w, v>(sk_seed, sk_prf, pk_seed, skey, pkey);

    benchmark::DoNotOptimize(sk_seed);
    benchmark::DoNotOptimize(sk_prf);
    benchmark::DoNotOptimize(pk_seed);
    benchmark::DoNotOptimize(skey);
    benchmark::DoNotOptimize(pkey);
    benchmark::ClobberMemory();

#ifdef __x86_64__
    const uint64_t end = cpu_cycles();
    total_cycles += (end - start);
#endif
  }

  state.SetItemsProcessed(state.iterations());

#ifdef __x86_64__
  total_cycles /= static_cast<uint64_t>(state.iterations());
  state.counters["average_cpu_cycles"] = static_cast<double>(total_cycles);
#endif

  std::free(sk_seed);
  std::free(sk_prf);
  std::free(pk_seed);
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
         const sphincs_plus_hashing::variant v,
         const bool randomize = false>
static inline void
sign(benchmark::State& state)
{
  namespace utils = sphincs_plus_utils;
  constexpr size_t pklen = utils::get_sphincs_pkey_len<n>();
  constexpr size_t sklen = utils::get_sphincs_skey_len<n>();
  constexpr size_t siglen = utils::get_sphincs_sig_len<n, h, d, a, k, w>();
  constexpr size_t mlen = 32;

  uint8_t* sk_seed = static_cast<uint8_t*>(std::malloc(n));
  uint8_t* sk_prf = static_cast<uint8_t*>(std::malloc(n));
  uint8_t* pk_seed = static_cast<uint8_t*>(std::malloc(n));
  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));
  uint8_t* msg = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* rand_bytes = static_cast<uint8_t*>(std::malloc(n));
  uint8_t* sig = static_cast<uint8_t*>(std::malloc(siglen));

  sphincs_plus_utils::random_data<uint8_t>(sk_seed, n);
  sphincs_plus_utils::random_data<uint8_t>(sk_prf, n);
  sphincs_plus_utils::random_data<uint8_t>(pk_seed, n);

  sphincs_plus::keygen<n, h, d, w, v>(sk_seed, sk_prf, pk_seed, skey, pkey);

  sphincs_plus_utils::random_data<uint8_t>(msg, mlen);
  sphincs_plus_utils::random_data<uint8_t>(rand_bytes, n);

#ifdef __x86_64__
  uint64_t total_cycles = 0ul;
#endif

  for (auto _ : state) {
#ifdef __x86_64__
    const uint64_t start = cpu_cycles();
#endif

    if constexpr (randomize) {
      sphincs_plus::sign<n, h, d, a, k, w, v, randomize>(msg, mlen, skey, rand_bytes, sig);
    } else {
      sphincs_plus::sign<n, h, d, a, k, w, v, randomize>(msg, mlen, skey, nullptr, sig);
    }

    benchmark::DoNotOptimize(msg);
    benchmark::DoNotOptimize(skey);
    if constexpr (randomize) {
      benchmark::DoNotOptimize(rand_bytes);
    }
    benchmark::DoNotOptimize(sig);
    benchmark::ClobberMemory();

#ifdef __x86_64__
    const uint64_t end = cpu_cycles();
    total_cycles += (end - start);
#endif
  }

  state.SetItemsProcessed(state.iterations());

#ifdef __x86_64__
  total_cycles /= static_cast<uint64_t>(state.iterations());
  state.counters["average_cpu_cycles"] = static_cast<double>(total_cycles);
#endif

  std::free(sk_seed);
  std::free(sk_prf);
  std::free(pk_seed);
  std::free(pkey);
  std::free(skey);
  std::free(msg);
  std::free(rand_bytes);
  std::free(sig);
}

// Benchmark SPHINCS+ signature verification algorithm
template<const size_t n,
         const uint32_t h,
         const uint32_t d,
         const uint32_t a,
         const uint32_t k,
         const size_t w,
         const sphincs_plus_hashing::variant v,
         const bool randomize = false>
static inline void
verify(benchmark::State& state)
{
  namespace utils = sphincs_plus_utils;
  constexpr size_t pklen = utils::get_sphincs_pkey_len<n>();
  constexpr size_t sklen = utils::get_sphincs_skey_len<n>();
  constexpr size_t siglen = utils::get_sphincs_sig_len<n, h, d, a, k, w>();
  constexpr size_t mlen = 32;

  uint8_t* sk_seed = static_cast<uint8_t*>(std::malloc(n));
  uint8_t* sk_prf = static_cast<uint8_t*>(std::malloc(n));
  uint8_t* pk_seed = static_cast<uint8_t*>(std::malloc(n));
  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));
  uint8_t* msg = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* rand_bytes = static_cast<uint8_t*>(std::malloc(n));
  uint8_t* sig = static_cast<uint8_t*>(std::malloc(siglen));

  sphincs_plus_utils::random_data<uint8_t>(sk_seed, n);
  sphincs_plus_utils::random_data<uint8_t>(sk_prf, n);
  sphincs_plus_utils::random_data<uint8_t>(pk_seed, n);
  sphincs_plus_utils::random_data<uint8_t>(msg, mlen);
  sphincs_plus_utils::random_data<uint8_t>(rand_bytes, n);

  sphincs_plus::keygen<n, h, d, w, v>(sk_seed, sk_prf, pk_seed, skey, pkey);
  if constexpr (randomize) {
    sphincs_plus::sign<n, h, d, a, k, w, v, randomize>(msg, mlen, skey, rand_bytes, sig);
  } else {
    sphincs_plus::sign<n, h, d, a, k, w, v, randomize>(msg, mlen, skey, nullptr, sig);
  }

#ifdef __x86_64__
  uint64_t total_cycles = 0ul;
#endif

  bool flag = true;
  for (auto _ : state) {
#ifdef __x86_64__
    const uint64_t start = cpu_cycles();
#endif

    flag &= sphincs_plus::verify<n, h, d, a, k, w, v>(msg, mlen, sig, pkey);

    benchmark::DoNotOptimize(flag);
    benchmark::DoNotOptimize(msg);
    benchmark::DoNotOptimize(sig);
    benchmark::DoNotOptimize(pkey);
    benchmark::ClobberMemory();

#ifdef __x86_64__
    const uint64_t end = cpu_cycles();
    total_cycles += (end - start);
#endif
  }

  assert(flag);
  state.SetItemsProcessed(state.iterations());

#ifdef __x86_64__
  total_cycles /= static_cast<uint64_t>(state.iterations());
  state.counters["average_cpu_cycles"] = static_cast<double>(total_cycles);
#endif

  std::free(sk_seed);
  std::free(sk_prf);
  std::free(pk_seed);
  std::free(pkey);
  std::free(skey);
  std::free(msg);
  std::free(rand_bytes);
  std::free(sig);
}

}
