#pragma once
#include "sphincs+.hpp"
#include "utils.hpp"

// SPHINCS+-256s-simple Signature Scheme
namespace sphincs_plus_256s_simple {

// SPHINCS+-256s-simple parameter set, as specified in table 3 of https://sphincs.org/data/sphincs+-r3.1-specification.pdf.
constexpr size_t n = 32;
constexpr uint32_t h = 64;
constexpr uint32_t d = 8;
constexpr uint32_t a = 14;
constexpr uint32_t k = 22;
constexpr size_t w = 16;
constexpr auto v = sphincs_plus_hashing::variant::simple;

// = 64 -bytes SPHINCS+-256s-simple public key
constexpr size_t PubKeyLen = sphincs_plus_utils::get_sphincs_pkey_len<n>();

// = 128 -bytes SPHINCS+-256s-simple secret key
constexpr size_t SecKeyLen = sphincs_plus_utils::get_sphincs_skey_len<n>();

// = 29792 -bytes SPHINCS+-256s-simple signature
constexpr size_t SigLen = sphincs_plus_utils::get_sphincs_sig_len<n, h, d, a, k, w>();

inline void
keygen(const uint8_t* const __restrict sk_seed, // n -bytes secret key seed
       const uint8_t* const __restrict sk_prf,  // n -bytes secret key PRF
       const uint8_t* const __restrict pk_seed, // n -bytes public key seed
       uint8_t* const __restrict skey,          // 4*n -bytes SPHINCS+ secret key
       uint8_t* const __restrict pkey           // 2*n -bytes SPHINCS+ public key
)
{
  sphincs_plus::keygen<n, h, d, w, v>(sk_seed, sk_prf, pk_seed, skey, pkey);
}

template<bool randomize = false>
inline void
sign(const uint8_t* const __restrict msg,        // message to be signed
     const size_t mlen,                          // byte length of message
     const uint8_t* const __restrict skey,       // SPHINCS+ secret key of 4*n -bytes
     const uint8_t* const __restrict rand_bytes, // Optional n -bytes randomness
     uint8_t* const __restrict sig               // SPHINCS+ signature
)
{
  sphincs_plus::sign<n, h, d, a, k, w, v>(msg, mlen, skey, rand_bytes, sig);
}

inline void
verify(const uint8_t* const __restrict msg, // message which was signed
       const size_t mlen,                   // byte length of message
       const uint8_t* const __restrict sig, // SPHINCS+ signature
       const uint8_t* const __restrict pkey // SPHINCS+ public key of 2*n -bytes
)
{
  sphincs_plus::verify<n, h, d, a, k, w, v>(msg, mlen, sig, pkey);
}

}