#pragma once
#include "shake256.hpp"

// Tweakable hash functions, PRFs and keyed hash functions, for SPHINCS+-SHAKE
// instantiation
namespace sphincs_hashing {

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

}
