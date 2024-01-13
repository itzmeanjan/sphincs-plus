#pragma once
#include "sphincs+.hpp"
#include "utils.hpp"

// SPHINCS+-192f-robust Signature Scheme
namespace sphincs_plus_192f_robust {

// SPHINCS+-192f-robust parameter set, as specified in table 3 of https://sphincs.org/data/sphincs+-r3.1-specification.pdf.
constexpr size_t n = 24;
constexpr uint32_t h = 66;
constexpr uint32_t d = 22;
constexpr uint32_t a = 8;
constexpr uint32_t k = 33;
constexpr size_t w = 16;
constexpr auto v = sphincs_plus_hashing::variant::robust;

// = 48 -bytes SPHINCS+-192f-robust public key
constexpr size_t PubKeyLen = sphincs_plus_utils::get_sphincs_pkey_len<n>();

// = 96 -bytes SPHINCS+-192f-robust secret key
constexpr size_t SecKeyLen = sphincs_plus_utils::get_sphincs_skey_len<n>();

// = 35664 -bytes SPHINCS+-192f-robust signature
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
  sphincs_plus::sign<n, h, d, a, k, w, v, randomize>(msg, skey, rand_bytes, sig);
}

inline bool
verify(std::span<const uint8_t> msg, std::span<const uint8_t, SigLen> sig, std::span<const uint8_t, PubKeyLen> pkey)
{
  return sphincs_plus::verify<n, h, d, a, k, w, v>(msg, sig, pkey);
}

}
