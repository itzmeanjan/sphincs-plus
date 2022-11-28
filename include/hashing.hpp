#pragma once
#include "shake256.hpp"

// Tweakable hash functions, PRFs and keyed hash functions, for SPHINCS+-SHAKE
// instantiation
namespace sphincs_hashing {

// Used for parameterization of tweakable hash functions, in Sphincs+
enum class variant : uint8_t
{
  robust = 1,
  simple = 2
};

// Given n -bytes root, n -bytes public key seed, n -bytes public key root and
// mlen -bytes message ( to be signed ), this routine uses SHAKE256, as a keyed
// hash function, for compressing message, while extracting out m -bytes output
//
// See section 7.2.1 of Sphincs+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<const size_t n, const size_t m>
inline static void
h_msg(const uint8_t* const __restrict r,
      const uint8_t* const __restrict pk_seed,
      const uint8_t* const __restrict pk_root,
      const uint8_t* const __restrict msg,
      const size_t mlen,
      uint8_t* const __restrict dig)
{
  shake256::shake256<true> hasher{};

  hasher.absorb(r, n);
  hasher.absorb(pk_seed, n);
  hasher.absorb(pk_root, n);
  hasher.absorb(msg, mlen);

  hasher.finalize();

  hasher.read(dig, m);
}

// Given n -bytes public key seed, n -bytes secret key seed and 32 -bytes
// address, this routine makes use of SHAKE256, as pseudorandom function, for
// generating pseudorandom key of byte length n
//
// See section 7.2.1 of Sphincs+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<const size_t n>
inline static void
prf(const uint8_t* const __restrict pk_seed,
    const uint8_t* const __restrict sk_seed,
    const uint8_t* const __restrict adrs,
    uint8_t* const __restrict dig)
{
  shake256::shake256<true> hasher{};

  hasher.absorb(pk_seed, n);
  hasher.absorb(adrs, 32);
  hasher.absorb(sk_seed, n);

  hasher.finalize();

  hasher.read(dig, n);
}

// Given n -bytes secret key prf, n -bytes OptRand and mlen -bytes message ( to
// be signed ), this routine uses SHAKE256, as pseudorandom function, for
// generating randomness so that message can be compressed, before signing.
//
// See section 7.2.1 of Sphincs+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<const size_t n>
inline static void
prf_msg(const uint8_t* const __restrict sk_prf,
        const uint8_t* const __restrict opt_rand,
        const uint8_t* const __restrict msg,
        const size_t mlen,
        uint8_t* const __restrict dig)
{
  shake256::shake256<true> hasher{};

  hasher.absorb(sk_prf, n);
  hasher.absorb(opt_rand, n);
  hasher.absorb(msg, mlen);

  hasher.finalize();

  hasher.read(dig, n);
}

// Given n -bytes public key seed, 32 -bytes address and n * l -bytes message,
// this routine uses SHAKE256, in XOF mode, for generating a bit mask of length
// n * l -bytes, which is XOR-ed into the message for producing masked message.
//
// See section 7.2.1 of Sphincs+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<const size_t n, const size_t l>
inline static void
gen_mask(const uint8_t* const __restrict pk_seed,
         const uint8_t* const __restrict adrs,
         const uint8_t* const __restrict msg,
         uint8_t* const __restrict dig)
{
  constexpr size_t mlen = n * l;

  shake256::shake256<true> hasher{};

  hasher.absorb(pk_seed, n);
  hasher.absorb(adrs, 32);

  hasher.finalize();

  hasher.read(dig, mlen);

  for (size_t i = 0; i < mlen; i++) {
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
template<const size_t n, const size_t l, const variant v>
inline static void
t_l(const uint8_t* const __restrict pk_seed,
    const uint8_t* const __restrict adrs,
    const uint8_t* const __restrict msg,
    uint8_t* const __restrict dig)
{
  constexpr size_t mlen = n * l;

  shake256::shake256<true> hasher{};

  hasher.absorb(pk_seed, n);
  hasher.absorb(adrs, 32);

  if constexpr (v == variant::robust) {
    uint8_t masked[mlen]{};
    gen_mask<n, l>(pk_seed, adrs, msg, masked);

    hasher.absorb(masked, mlen);
  } else {
    hasher.absorb(msg, mlen);
  }

  hasher.finalize();

  hasher.read(dig, n);
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
template<const size_t n, const variant v>
inline static void
f(const uint8_t* const __restrict pk_seed,
  const uint8_t* const __restrict adrs,
  const uint8_t* const __restrict msg,
  uint8_t* const __restrict dig)
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
template<const size_t n, const variant v>
inline static void
h(const uint8_t* const __restrict pk_seed,
  const uint8_t* const __restrict adrs,
  const uint8_t* const __restrict msg,
  uint8_t* const __restrict dig)
{
  t_l<n, 2, v>(pk_seed, adrs, msg, dig);
}

}
