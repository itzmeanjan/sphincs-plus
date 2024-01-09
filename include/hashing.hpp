#pragma once
#include "shake256.hpp"
#include <array>
#include <cstring>
#include <span>

// Tweakable hash functions, PRFs and keyed hash functions, for SPHINCS+-SHAKE
// instantiation
namespace sphincs_plus_hashing {

// Used for parameterization of tweakable hash functions, in Sphincs+
enum class variant : uint8_t
{
  robust = 1,
  simple = 2
};

// Given n -bytes root, n -bytes public key seed, n -bytes public key root and
// mlen -bytes message ( to be signed ), this routine uses SHAKE256, as a keyed
// hash function, for compressing message, while extracting out m -bytes output.
//
// See section 7.2.1 of Sphincs+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n, size_t m>
static inline constexpr void
h_msg(std::span<const uint8_t, n> r,
      std::span<const uint8_t, n> pk_seed,
      std::span<const uint8_t, n> pk_root,
      std::span<const uint8_t> msg,
      std::span<uint8_t, m> dig)
{
  std::array<uint8_t, r.size() + pk_seed.size() + pk_root.size()> tmp{};
  auto _tmp = std::span(tmp);

  std::copy(r.begin(), r.end(), _tmp.template subspan<0, r.size()>().begin());
  std::copy(pk_seed.begin(), pk_seed.end(), _tmp.template subspan<r.size(), pk_seed.size()>().begin());
  std::copy(pk_root.begin(), pk_root.end(), _tmp.template subspan<r.size() + pk_seed.size(), pk_root.size()>().begin());

  shake256::shake256_t hasher;

  hasher.absorb(tmp);
  hasher.absorb(msg);
  hasher.finalize();
  hasher.squeeze(dig);
}

// Given n -bytes public key seed, n -bytes secret key seed and 32 -bytes
// address, this routine makes use of SHAKE256, as pseudorandom function, for
// generating pseudorandom key of byte length n.
//
// See section 7.2.1 of Sphincs+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n>
static inline constexpr void
prf(std::span<const uint8_t, n> pk_seed, std::span<const uint8_t, n> sk_seed, std::span<const uint8_t, 32> adrs, std::span<uint8_t, n> dig)
{
  std::array<uint8_t, pk_seed.size() + adrs.size() + sk_seed.size()> tmp{};
  auto _tmp = std::span(tmp);

  std::copy(pk_seed.begin(), pk_seed.end(), _tmp.template subspan<0, pk_seed.size()>().begin());
  std::copy(adrs.begin(), adrs.end(), _tmp.template subspan<pk_seed.size(), adrs.size()>().begin());
  std::copy(sk_seed.begin(), sk_seed.end(), _tmp.template subspan<pk_seed.size() + adrs.size(), sk_seed.size()>().begin());

  shake256::shake256_t hasher;

  hasher.absorb(tmp);
  hasher.finalize();
  hasher.squeeze(dig);
}

// Given n -bytes secret key prf, n -bytes OptRand and mlen -bytes message ( to
// be signed ), this routine uses SHAKE256, as pseudorandom function, for
// generating randomness so that message can be compressed, before signing.
//
// See section 7.2.1 of Sphincs+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n>
static inline constexpr void
prf_msg(std::span<const uint8_t, n> sk_prf, std::span<const uint8_t, n> opt_rand, std::span<const uint8_t> msg, std::span<uint8_t, n> dig)
{
  std::array<uint8_t, sk_prf.size() + opt_rand.size()> tmp{};
  auto _tmp = std::span(tmp);

  std::copy(sk_prf.begin(), sk_prf.end(), _tmp.template subspan<0, sk_prf.size()>().begin());
  std::copy(opt_rand.begin(), opt_rand.end(), _tmp.template subspan<sk_prf.size(), opt_rand.size()>().begin());

  shake256::shake256_t hasher;

  hasher.absorb(tmp);
  hasher.absorb(msg);
  hasher.finalize();
  hasher.squeeze(dig);
}

// Given n -bytes public key seed, 32 -bytes address and n * l -bytes message,
// this routine uses SHAKE256, in XOF mode, for generating a bit mask of length
// n * l -bytes, which is XOR-ed into the message for producing masked message.
//
// See section 7.2.1 of Sphincs+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n, size_t l>
static inline void
gen_mask(std::span<const uint8_t, n> pk_seed, std::span<const uint8_t, 32> adrs, std::span<const uint8_t, n * l> msg, std::span<uint8_t, n * l> dig)
{
  std::array<uint8_t, pk_seed.size() + adrs.size()> tmp{};
  auto _tmp = std::span(tmp);

  std::copy(pk_seed.begin(), pk_seed.end(), _tmp.template subspan<0, pk_seed.size()>().begin());
  std::copy(adrs.begin(), adrs.end(), _tmp.template subspan<pk_seed.size(), adrs.size()>().begin());

  shake256::shake256_t hasher;

  hasher.absorb(tmp);
  hasher.finalize();
  hasher.squeeze(dig);

#if defined __clang__
#pragma clang loop unroll(enable)
#pragma clang loop vectorize(enable)
#elif defined __GNUG__
#pragma GCC unroll 16
#pragma GCC ivdep
#endif
  for (size_t i = 0; i < n * l; i++) {
    dig[i] ^= msg[i];
  }
}

// Given n -bytes public key seed, 32 -bytes address and n * l -bytes message,
// this routines uses SHAKE256, for constructing a tweakable hash function,
// producing n -bytes output.
//
// Note, this routine supports compile-time parameterization, to be used as
// robust or simple variant of T_l routine.
//
// See section 7.2.1 of Sphincs+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n, size_t l, variant v>
static inline constexpr void
t_l(std::span<const uint8_t, n> pk_seed, std::span<const uint8_t, 32> adrs, std::span<const uint8_t, n * l> msg, std::span<uint8_t, n> dig)
{
  std::array<uint8_t, pk_seed.size() + adrs.size()> tmp{};
  auto _tmp = std::span(tmp);

  std::copy(pk_seed.begin(), pk_seed.end(), _tmp.template subspan<0, pk_seed.size()>().begin());
  std::copy(adrs.begin(), adrs.end(), _tmp.template subspan<pk_seed.size(), adrs.size()>().begin());

  shake256::shake256_t hasher;

  hasher.absorb(tmp);

  if constexpr (v == variant::robust) {
    std::array<uint8_t, msg.size()> masked{};
    gen_mask<n, l>(pk_seed, adrs, msg, masked);

    hasher.absorb(masked);
  } else {
    hasher.absorb(msg);
  }

  hasher.finalize();
  hasher.squeeze(dig);
}

// Given n -bytes public key seed, 32 -bytes address and n -bytes message,
// this routines uses SHAKE256, for constructing a tweakable hash function,
// producing n -bytes output.
//
// Note, this routine supports compile-time parameterization, to be used as
// robust or simple variant of F routine. Also notice that,
//
// F == T_l | l = 1 ( see section 2.7.1 )
//
// See section 7.2.1 of Sphincs+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n, variant v>
static inline constexpr void
f(std::span<const uint8_t, n> pk_seed, std::span<const uint8_t, 32> adrs, std::span<const uint8_t, n> msg, std::span<uint8_t, n> dig)
{
  t_l<n, 1, v>(pk_seed, adrs, msg, dig);
}

// Given n -bytes public key seed, 32 -bytes address and 2*n -bytes message,
// this routines uses SHAKE256, for constructing a tweakable hash function,
// producing n -bytes output.
//
// Note, this routine supports compile-time parameterization, to be used as
// robust or simple variant of H routine. Also notice that,
//
// H == T_l | l = 2 ( see section 2.7.1 )
//
// See section 7.2.1 of Sphincs+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n, variant v>
static inline constexpr void
h(std::span<const uint8_t, n> pk_seed, std::span<const uint8_t, 32> adrs, std::span<const uint8_t, 2 * n> msg, std::span<uint8_t, n> dig)
{
  t_l<n, 2, v>(pk_seed, adrs, msg, dig);
}

}
