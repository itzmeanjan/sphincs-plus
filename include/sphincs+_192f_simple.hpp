#pragma once
#include "sphincs+.hpp"
#include "utils.hpp"

// SPHINCS+-192f-simple Signature Scheme
namespace sphincs_plus_192f_simple {

// SPHINCS+-192f-simple parameter set, as specified in table 3 of https://sphincs.org/data/sphincs+-r3.1-specification.pdf.
constexpr size_t n = 24;
constexpr uint32_t h = 66;
constexpr uint32_t d = 22;
constexpr uint32_t a = 8;
constexpr uint32_t k = 33;
constexpr size_t w = 16;
constexpr auto v = sphincs_plus_hashing::variant::simple;

// = 48 -bytes SPHINCS+-192f-simple public key
constexpr size_t PubKeyLen = sphincs_plus_utils::get_sphincs_pkey_len<n>();

// = 96 -bytes SPHINCS+-192f-simple secret key
constexpr size_t SecKeyLen = sphincs_plus_utils::get_sphincs_skey_len<n>();

// = 35664 -bytes SPHINCS+-192f-simple signature
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

inline bool
verify(const uint8_t* const __restrict msg, // message which was signed
       const size_t mlen,                   // byte length of message
       const uint8_t* const __restrict sig, // SPHINCS+ signature
       const uint8_t* const __restrict pkey // SPHINCS+ public key of 2*n -bytes
)
{
  return sphincs_plus::verify<n, h, d, a, k, w, v>(msg, mlen, sig, pkey);
}

}
