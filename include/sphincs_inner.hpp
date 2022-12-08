#pragma once
#include "fors.hpp"
#include "hypertree.hpp"

// SPHINCS+ Signature Scheme, with generic API
namespace sphincs_inner {

// SPHINCS+ key generation algorithm, where one can provide n -bytes secret key
// seed, n -bytes secret key PRF and n -bytes public key seed as input, which
// are random sampled outside of context of this function and this routine
// generates 4*n -bytes SPHINCS+ secret key and 2*n -bytes SPHINCS+ public key.
//
// Note, SPHINCS+ secret key keeps a copy of SPHINCS+ public key, which is used
// during signing.
//
// This routine is targeted for users who might be interested in providing
// their own random input for secure key generation. There will be higher level
// API exposed, where randomness sampling will be abstracted away, using system
// randomness ( read <random> header ).
//
// This form of SPHINCS+ key generation API will be useful when testing
// conformance with SPHINCS+ standard, using known answer tests (KATs).
//
// See section 6.2 and figure 14 of specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf for SPHINCS+ key
// generation algorithm and key format, respectively.
template<const size_t n,
         const uint32_t h,
         const uint32_t d,
         const size_t w,
         const sphincs_hashing::variant v>
inline static void
keygen(const uint8_t* const __restrict sk_seed, // n -bytes secret key seed
       const uint8_t* const __restrict sk_prf,  // n -bytes secret key PRF
       const uint8_t* const __restrict pk_seed, // n -bytes public key seed
       uint8_t* const __restrict skey, // 4*n -bytes SPHINCS+ secret key
       uint8_t* const __restrict pkey  // 2*n -bytes SPHINCS+ public key
       )
  requires(sphincs_utils::check_keygen_params<n, h, d, w, v>())
{
  uint8_t pk_root[n];
  sphincs_ht::pkgen<h, d, n, w, v>(sk_seed, pk_seed, pk_root);

  // prepare 2*n -bytes public key
  std::memcpy(pkey + 0, pk_seed, n);
  std::memcpy(pkey + n, pk_root, n);

  // prepare 4*n -bytes secret key, which also keeps a copy of public key ( used
  // during signing )
  std::memcpy(skey + 0, sk_seed, n);
  std::memcpy(skey + n, sk_prf, n);
  std::memcpy(skey + 2 * n, pkey, 2 * n);
}

// Computes SPHINCS+ signature over message of length mlen -bytes, while using
// 4*n -bytes secret key, following algorithm 20, as described in section 6.4 of
// specification https://sphincs.org/data/sphincs+-r3.1-specification.pdf
//
// For understandig structure of SPHINCS+ signature, read section 6.3 and look
// at figure 15.
//
// This routine allows optional randomized signing, while supplying your n
// -bytes randomness, which is sampled outside of this routine's context.
// Note, if you opt-in for randomized signing, it's expected of you to supply
// random byte array of length n, else you may pass nullptr as value for
// rand_bytes parameter. That being said, deterministic signing is the default
// option.
//
// This form of SPHINCS+ signing API will be useful when testing conformance
// with SPHINCS+ standard, using known answer tests (KATs).
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
     const uint8_t* const __restrict rand_bytes, // Optional n -bytes randomness
     uint8_t* const __restrict sig               // SPHINCS+ signature
     )
  requires(sphincs_utils::check_sign_verify_params<n, h, d, a, k, w, v>())
{
  constexpr size_t md_len = static_cast<size_t>((k * a + 7) >> 3);
  constexpr size_t itree_len = static_cast<size_t>((h - (h / d) + 7) >> 3);
  constexpr size_t ileaf_len = static_cast<size_t>(((h / d) + 7) >> 3);
  constexpr size_t m = md_len + itree_len + ileaf_len;

  constexpr size_t fors_sl = sphincs_utils::compute_fors_sig_len<n, a, k>();

  uint8_t* const sig0 = sig;            // Randomness portion
  uint8_t* const sig1 = sig0 + n;       // FORS signature portion
  uint8_t* const sig2 = sig1 + fors_sl; // HT signature portion

  const uint8_t* const sk_seed = skey;
  const uint8_t* const sk_prf = sk_seed + n;
  const uint8_t* const pk_seed = sk_prf + n;
  const uint8_t* const pk_root = pk_seed + n;

  constexpr uint32_t h_ = h - (h / d);
  constexpr bool flg = h_ == 64u;

  constexpr uint64_t mask0 = (1ul << (h_ - 1u * flg)) + (1ul << 63) * flg - 1ul;
  constexpr uint32_t mask1 = (1u << (h / d)) - 1ul;

  uint8_t opt[n];
  uint8_t dig[m];

  if constexpr (randomize) {
    std::memcpy(opt, rand_bytes, n);
  } else {
    std::memcpy(opt, pk_seed, n);
  }

  sphincs_hashing::prf_msg<n>(sk_prf, opt, msg, mlen, sig0);
  sphincs_hashing::h_msg<n, m>(sig0, pk_seed, pk_root, msg, mlen, dig);

  const uint8_t* const md = dig;
  const uint8_t* const tmp_itree = md + md_len;
  const uint8_t* const tmp_ileaf = tmp_itree + itree_len;

  uint64_t itree = 0ul;
  for (size_t i = 0; i < itree_len; i++) {
    itree |= static_cast<uint64_t>(tmp_itree[i]) << ((itree_len - 1 - i) << 3);
  }

  uint32_t ileaf = 0u;
  for (size_t i = 0; i < ileaf_len; i++) {
    ileaf |= static_cast<uint32_t>(tmp_ileaf[i]) << ((ileaf_len - 1 - i) << 3);
  }

  itree &= mask0;
  ileaf &= mask1;

  sphincs_adrs::fors_tree_t adrs{};

  adrs.set_layer_address(0u);
  adrs.set_tree_address(itree);
  adrs.set_type(sphincs_adrs::type_t::FORS_TREE);
  adrs.set_keypair_address(ileaf);

  uint8_t tmp[n];

  sphincs_fors::sign<n, a, k, v>(md, sk_seed, pk_seed, adrs, sig1);
  sphincs_fors::pk_from_sig<n, a, k, v>(sig1, md, pk_seed, adrs, tmp);
  sphincs_ht::sign<h, d, n, w, v>(tmp, sk_seed, pk_seed, itree, ileaf, sig2);
}

// Verifies a SPHINCS+ signature on a message of mlen -bytes using SPHINCS+
// public key of length 2*n -bytes, returning truth value ( boolean result ) in
// case of successful signature verification, following algorithm 21, as
// described in section 6.5 of specification
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
  requires(sphincs_utils::check_sign_verify_params<n, h, d, a, k, w, v>())
{
  constexpr size_t md_len = static_cast<size_t>((k * a + 7) >> 3);
  constexpr size_t itree_len = static_cast<size_t>((h - (h / d) + 7) >> 3);
  constexpr size_t ileaf_len = static_cast<size_t>(((h / d) + 7) >> 3);
  constexpr size_t m = md_len + itree_len + ileaf_len;

  constexpr size_t fors_sl = sphincs_utils::compute_fors_sig_len<n, a, k>();

  const uint8_t* const sig0 = sig;            // Randomness portion
  const uint8_t* const sig1 = sig0 + n;       // FORS signature portion
  const uint8_t* const sig2 = sig1 + fors_sl; // HT signature portion

  const uint8_t* const pk_seed = pkey;
  const uint8_t* const pk_root = pk_seed + n;

  constexpr uint32_t h_ = h - (h / d);
  constexpr bool flg = h_ == 64u;

  constexpr uint64_t mask0 = (1ul << (h_ - 1u * flg)) + (1ul << 63) * flg - 1ul;
  constexpr uint32_t mask1 = (1u << (h / d)) - 1ul;

  uint8_t dig[m];
  sphincs_hashing::h_msg<n, m>(sig0, pk_seed, pk_root, msg, mlen, dig);

  const uint8_t* const md = dig;
  const uint8_t* const tmp_itree = md + md_len;
  const uint8_t* const tmp_ileaf = tmp_itree + itree_len;

  uint64_t itree = 0ul;
  for (size_t i = 0; i < itree_len; i++) {
    itree |= static_cast<uint64_t>(tmp_itree[i]) << ((itree_len - 1 - i) << 3);
  }

  uint32_t ileaf = 0u;
  for (size_t i = 0; i < ileaf_len; i++) {
    ileaf |= static_cast<uint32_t>(tmp_ileaf[i]) << ((ileaf_len - 1 - i) << 3);
  }

  itree &= mask0;
  ileaf &= mask1;

  sphincs_adrs::fors_tree_t adrs{};

  adrs.set_layer_address(0u);
  adrs.set_tree_address(itree);
  adrs.set_type(sphincs_adrs::type_t::FORS_TREE);
  adrs.set_keypair_address(ileaf);

  uint8_t tmp[n];

  sphincs_fors::pk_from_sig<n, a, k, v>(sig1, md, pk_seed, adrs, tmp);

  namespace ht = sphincs_ht;
  return ht::verify<h, d, n, w, v>(tmp, sig2, pk_seed, itree, ileaf, pk_root);
}

}
