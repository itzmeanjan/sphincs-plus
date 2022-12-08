#pragma once
#include "sphincs_inner.hpp"

// SPHINCS+: Stateless Hash-based Digital Signature Algorithm
namespace sphincs {

// SPHINCS+ keypair generation algorithm, generating 4*n -bytes secret key and
// 2*n -bytes public key, following algorithm 19, as described in section 6.2 of
// specification https://sphincs.org/data/sphincs+-r3.1-specification.pdf
//
// Note, when sampling randomness, system randomness is used i.e. random number
// generator Mersenne Twister is seeded with
// https://en.cppreference.com/w/cpp/numeric/random/random_device which is not
// guaranteed to be non-deterministic on all platforms. And if that's something
// which concerns you, I suggest you to look at underlying key generation API,
// which is wrapped by this routine. There you can sample random bytes outside
// of key generation function context and pass it as an argument to key
// generator.
template<const size_t n,
         const uint32_t h,
         const uint32_t d,
         const size_t w,
         const sphincs_hashing::variant v>
inline static void
keygen(uint8_t* const __restrict skey, // 4*n -bytes SPHINCS+ secret key
       uint8_t* const __restrict pkey  // 2*n -bytes SPHINCS+ public key
)
{
  uint8_t sk_seed[n];
  uint8_t sk_prf[n];
  uint8_t pk_seed[n];

  sphincs_utils::random_data<uint8_t>(sk_seed, n);
  sphincs_utils::random_data<uint8_t>(sk_prf, n);
  sphincs_utils::random_data<uint8_t>(pk_seed, n);

  sphincs_inner::keygen<n, h, d, w, v>(sk_seed, sk_prf, pk_seed, skey, pkey);
}

// Given 4*n -bytes SPHINCS+ secret key, this routine computes SPHINCS+
// signature of (1 + k * (a + 1) + h + d * len) * n -bytes, over mlen -bytes
// message using 4*n -bytes secret key, following algorithm 20, as described in
// section 6.4 of the specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
//
// Deterministic signing is default option, but you may choose to opt-in for
// randomized signing, by passing truth value to template parameter.
//
// Note, when sampling randomness for randomized signing, system randomness is
// used i.e. random number generator Mersenne Twister is seeded with
// https://en.cppreference.com/w/cpp/numeric/random/random_device which is not
// guaranteed to be non-deterministic on all platforms. And if that's something
// which concerns you, I suggest you to look at underlying signing API, which is
// wrapped by this routine. There you can sample random bytes outside of signing
// function context and pass it as an argument to sign routine.
template<const size_t n,
         const uint32_t h,
         const uint32_t d,
         const uint32_t a,
         const uint32_t k,
         const size_t w,
         const sphincs_hashing::variant v,
         const bool randomize = false>
inline static void
sign(const uint8_t* const __restrict msg,  // message to be signed
     const size_t mlen,                    // byte length of message
     const uint8_t* const __restrict skey, // SPHINCS+ secret key of 4*n -bytes
     uint8_t* const __restrict sig         // SPHINCS+ signature
)
{
  constexpr bool r = randomize;
  uint8_t rand_bytes[n];

  if constexpr (r) {
    sphincs_utils::random_data<uint8_t>(rand_bytes, n);
  }

  sphincs_inner::sign<n, h, d, a, k, w, v, r>(msg, mlen, skey, rand_bytes, sig);
}

// Verifies a SPHINCS+ signature on a mlen -bytes message using SPHINCS+ public
// key of length 2*n -bytes, returning truth value ( boolean result ) in case of
// successful signature verification, following algorithm 21, as described in
// section 6.5 of specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<const size_t n,
         const uint32_t h,
         const uint32_t d,
         const uint32_t a,
         const uint32_t k,
         const size_t w,
         const sphincs_hashing::variant v>
inline static bool
verify(const uint8_t* const __restrict msg, // message which was signed
       const size_t mlen,                   // byte length of message
       const uint8_t* const __restrict sig, // SPHINCS+ signature
       const uint8_t* const __restrict pkey // SPHINCS+ public key of 2*n -bytes
)
{
  return sphincs_inner::verify<n, h, d, a, k, w, v>(msg, mlen, sig, pkey);
}

}
