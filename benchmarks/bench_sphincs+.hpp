#pragma once
#include "prng.hpp"
#include "sphincs+.hpp"
#include "x86_64_cpu_ticks.hpp"
#include <benchmark/benchmark.h>
#include <cassert>

// Benchmark SPHINCS+ Routines
namespace bench_sphincs_plus {

// Benchmark SPHINCS+ keypair generation
template<const size_t n, const uint32_t h, const uint32_t d, const size_t w, const sphincs_plus_hashing::variant v>
static inline void
keygen(benchmark::State& state)
{
  constexpr size_t pklen = sphincs_plus_utils::get_sphincs_pkey_len<n>();
  constexpr size_t sklen = sphincs_plus_utils::get_sphincs_skey_len<n>();

  std::vector<uint8_t> sk_seed(n, 0);
  std::vector<uint8_t> sk_prf(n, 0);
  std::vector<uint8_t> pk_seed(n, 0);
  std::vector<uint8_t> pkey(pklen, 0);
  std::vector<uint8_t> skey(sklen, 0);

  auto _sk_seed = std::span<uint8_t, n>(sk_seed);
  auto _sk_prf = std::span<uint8_t, n>(sk_prf);
  auto _pk_seed = std::span<uint8_t, n>(pk_seed);
  auto _pkey = std::span<uint8_t, pklen>(pkey);
  auto _skey = std::span<uint8_t, sklen>(skey);

  prng::prng_t prng;
  prng.read(_sk_seed);
  prng.read(_sk_prf);
  prng.read(_pk_seed);

#ifdef __x86_64__
  uint64_t total_ticks = 0ul;
#endif

  for (auto _ : state) {
#ifdef __x86_64__
    const uint64_t start = cpu_ticks();
#endif

    sphincs_plus::keygen<n, h, d, w, v>(_sk_seed, _sk_prf, _pk_seed, _skey, _pkey);

    benchmark::DoNotOptimize(_sk_seed);
    benchmark::DoNotOptimize(_sk_prf);
    benchmark::DoNotOptimize(_pk_seed);
    benchmark::DoNotOptimize(_skey);
    benchmark::DoNotOptimize(_pkey);
    benchmark::ClobberMemory();

#ifdef __x86_64__
    const uint64_t end = cpu_ticks();
    total_ticks += (end - start);
#endif
  }

  state.SetItemsProcessed(state.iterations());

#ifdef __x86_64__
  total_ticks /= static_cast<uint64_t>(state.iterations());
  state.counters["rdtsc"] = static_cast<double>(total_ticks);
#endif
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
  constexpr size_t pklen = sphincs_plus_utils::get_sphincs_pkey_len<n>();
  constexpr size_t sklen = sphincs_plus_utils::get_sphincs_skey_len<n>();
  constexpr size_t siglen = sphincs_plus_utils::get_sphincs_sig_len<n, h, d, a, k, w>();
  const size_t mlen = state.range();

  std::vector<uint8_t> sk_seed(n, 0);
  std::vector<uint8_t> sk_prf(n, 0);
  std::vector<uint8_t> pk_seed(n, 0);
  std::vector<uint8_t> pkey(pklen, 0);
  std::vector<uint8_t> skey(sklen, 0);
  std::vector<uint8_t> msg(mlen, 0);
  std::vector<uint8_t> rand_bytes(n, 0);
  std::vector<uint8_t> sig(siglen, 0);

  auto _sk_seed = std::span<uint8_t, n>(sk_seed);
  auto _sk_prf = std::span<uint8_t, n>(sk_prf);
  auto _pk_seed = std::span<uint8_t, n>(pk_seed);
  auto _pkey = std::span<uint8_t, pklen>(pkey);
  auto _skey = std::span<uint8_t, sklen>(skey);
  auto _msg = std::span(msg);
  auto _rand_bytes = std::span<uint8_t, n>(rand_bytes);
  auto _sig = std::span<uint8_t, siglen>(sig);

  prng::prng_t prng;
  prng.read(_sk_seed);
  prng.read(_sk_prf);
  prng.read(_pk_seed);
  prng.read(_msg);
  prng.read(_rand_bytes);

  sphincs_plus::keygen<n, h, d, w, v>(_sk_seed, _sk_prf, _pk_seed, _skey, _pkey);

#ifdef __x86_64__
  uint64_t total_ticks = 0ul;
#endif

  for (auto _ : state) {
#ifdef __x86_64__
    const uint64_t start = cpu_ticks();
#endif

    if constexpr (randomize) {
      sphincs_plus::sign<n, h, d, a, k, w, v, randomize>(_msg, _skey, _rand_bytes, _sig);
    } else {
      sphincs_plus::sign<n, h, d, a, k, w, v, randomize>(_msg, _skey, {}, _sig);
    }

    benchmark::DoNotOptimize(_msg);
    benchmark::DoNotOptimize(_skey);
    if constexpr (randomize) {
      benchmark::DoNotOptimize(_rand_bytes);
    }
    benchmark::DoNotOptimize(_sig);
    benchmark::ClobberMemory();

#ifdef __x86_64__
    const uint64_t end = cpu_ticks();
    total_ticks += (end - start);
#endif
  }

  state.SetItemsProcessed(state.iterations());

#ifdef __x86_64__
  total_ticks /= static_cast<uint64_t>(state.iterations());
  state.counters["rdtsc"] = static_cast<double>(total_ticks);
#endif
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
  constexpr size_t pklen = sphincs_plus_utils::get_sphincs_pkey_len<n>();
  constexpr size_t sklen = sphincs_plus_utils::get_sphincs_skey_len<n>();
  constexpr size_t siglen = sphincs_plus_utils::get_sphincs_sig_len<n, h, d, a, k, w>();
  const size_t mlen = state.range();

  std::vector<uint8_t> sk_seed(n, 0);
  std::vector<uint8_t> sk_prf(n, 0);
  std::vector<uint8_t> pk_seed(n, 0);
  std::vector<uint8_t> pkey(pklen, 0);
  std::vector<uint8_t> skey(sklen, 0);
  std::vector<uint8_t> msg(mlen, 0);
  std::vector<uint8_t> rand_bytes(n, 0);
  std::vector<uint8_t> sig(siglen, 0);

  auto _sk_seed = std::span<uint8_t, n>(sk_seed);
  auto _sk_prf = std::span<uint8_t, n>(sk_prf);
  auto _pk_seed = std::span<uint8_t, n>(pk_seed);
  auto _pkey = std::span<uint8_t, pklen>(pkey);
  auto _skey = std::span<uint8_t, sklen>(skey);
  auto _msg = std::span(msg);
  auto _rand_bytes = std::span<uint8_t, n>(rand_bytes);
  auto _sig = std::span<uint8_t, siglen>(sig);

  prng::prng_t prng;
  prng.read(_sk_seed);
  prng.read(_sk_prf);
  prng.read(_pk_seed);
  prng.read(_msg);
  prng.read(_rand_bytes);

  sphincs_plus::keygen<n, h, d, w, v>(_sk_seed, _sk_prf, _pk_seed, _skey, _pkey);

  if constexpr (randomize) {
    sphincs_plus::sign<n, h, d, a, k, w, v, randomize>(_msg, _skey, _rand_bytes, _sig);
  } else {
    sphincs_plus::sign<n, h, d, a, k, w, v, randomize>(_msg, _skey, {}, _sig);
  }

#ifdef __x86_64__
  uint64_t total_ticks = 0ul;
#endif

  bool flag = true;
  for (auto _ : state) {
#ifdef __x86_64__
    const uint64_t start = cpu_ticks();
#endif

    flag &= sphincs_plus::verify<n, h, d, a, k, w, v>(_msg, _sig, _pkey);

    benchmark::DoNotOptimize(flag);
    benchmark::DoNotOptimize(_msg);
    benchmark::DoNotOptimize(_sig);
    benchmark::DoNotOptimize(_pkey);
    benchmark::ClobberMemory();

#ifdef __x86_64__
    const uint64_t end = cpu_ticks();
    total_ticks += (end - start);
#endif
  }

  assert(flag);
  state.SetItemsProcessed(state.iterations());

#ifdef __x86_64__
  total_ticks /= static_cast<uint64_t>(state.iterations());
  state.counters["rdtsc"] = static_cast<double>(total_ticks);
#endif
}

}
