#pragma once
#include "sphincs+.hpp"
#include "utils.hpp"

// SPHINCS+-128s-simple Signature Scheme
namespace sphincs_plus_128s_simple {

// SPHINCS+-128s-simple parameter set, as specified in table 3 of https://sphincs.org/data/sphincs+-r3.1-specification.pdf.
constexpr size_t n = 16;
constexpr uint32_t h = 63;
constexpr uint32_t d = 7;
constexpr uint32_t a = 12;
constexpr uint32_t k = 14;
constexpr size_t w = 16;
constexpr auto v = sphincs_plus_hashing::variant::simple;

// = 32 -bytes SPHINCS+-128s-simple public key
constexpr size_t PubKeyLen = sphincs_plus_utils::get_sphincs_pkey_len<n>();

// = 64 -bytes SPHINCS+-128s-simple secret key
constexpr size_t SecKeyLen = sphincs_plus_utils::get_sphincs_skey_len<n>();

// = 7856 -bytes SPHINCS+-128s-simple signature
constexpr size_t SigLen = sphincs_plus_utils::get_sphincs_sig_len<n, h, d, a, k, w>();

inline void
keygen(std::span<const uint8_t, n> sk_seed,
       std::span<const uint8_t, n> sk_prf,
       std::span<const uint8_t, n> pk_seed,
       std::span<uint8_t, SecKeyLen> skey,
       std::span<uint8_t, PubKeyLen> pkey)
{
  sphincs_plus::keygen<n, h, d, w, v>(sk_seed, sk_prf, pk_seed, skey, pkey);
}

template<bool randomize = false>
inline void
sign(std::span<const uint8_t> msg, std::span<const uint8_t, SecKeyLen> skey, std::span<const uint8_t, n * randomize> rand_bytes, std::span<uint8_t, SigLen> sig)
{
  sphincs_plus::sign<n, h, d, a, k, w, v>(msg, skey, rand_bytes, sig);
}

inline bool
verify(std::span<const uint8_t> msg, std::span<const uint8_t, SigLen> sig, std::span<const uint8_t, PubKeyLen> pkey)
{
  return sphincs_plus::verify<n, h, d, a, k, w, v>(msg, sig, pkey);
}

}
