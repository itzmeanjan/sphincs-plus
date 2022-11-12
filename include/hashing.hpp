#pragma once
#include "shake256.hpp"

// Tweakable hash functions, PRFs and keyed hash functions, for SPHINCS+-SHAKE
// instantiation
namespace sphincs_hashing {

// Given n -bytes root, n -bytes public key seed, n -bytes public key root and
// mlen -bytes message ( to be signed ), this routine uses SHAKE256, as a keyed
// hash function, for compressing message, while extracting out m -bytes output
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

}
