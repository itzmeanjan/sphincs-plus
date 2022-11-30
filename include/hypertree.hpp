#pragma once
#include "xmss.hpp"

// HT: The Hypertree, used in SPHINCS+
namespace sphincs_ht {

// Computes n -bytes hypertree public key, which is the root of the single XMSS
// tree living on top layer ( i.e. layer index d - 1 ) of the hypertree, given n
// -bytes secret key seed & n -bytes public key seed, using algorithm 11,
// described in section 4.2.2 of SPHINCS+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<const uint32_t h,
         const uint32_t d,
         const size_t n,
         const size_t w,
         const sphincs_hashing::variant v>
inline static void
pkgen(const uint8_t* const __restrict sk_seed, // n -bytes secret key seed
      const uint8_t* const __restrict pk_seed, // n -bytes public key seed
      uint8_t* const __restrict pkey           // n -bytes HT public key
      )
  requires(sphincs_utils::check_ht_height_and_layer(h, d))
{
  sphincs_adrs::adrs_t adrs{};

  adrs.set_layer_address(d - 1u);
  adrs.set_tree_address(0ul);

  sphincs_xmss::pkgen<h / d, n, w, v>(sk_seed, pk_seed, adrs, pkey);
}

// Computes (h + d * len) * n -bytes HyperTree signature, consisting of d
// -many XMSS signatures each of (h/ d + len) * n -bytes, given a n -bytes
// message, n -bytes secret key seed, n -bytes public key seed, 8 -bytes XMSS
// tree address and 4 -bytes leaf index of that XMSS tree, using algorithm 12,
// described in section 4.2.4 of SPHINCS+ specification.
//
// For understanding HyperTree signature structure, I suggest you look at figure
// 11 and read section 4.2.3 of SPHINCS+ specification.
//
// Find the specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<const uint32_t h,
         const uint32_t d,
         const size_t n,
         const size_t w,
         const sphincs_hashing::variant v>
inline static void
sign(const uint8_t* const __restrict msg, // n -bytes message ( to be signed )
     const uint8_t* const __restrict sk_seed, // n -bytes secret key seed
     const uint8_t* const __restrict pk_seed, // n -bytes public key seed
     const uint64_t idx_tree,                 // 8 -bytes address to XMSS tree
     const uint32_t idx_leaf,      // 4 -bytes leaf index in that XMSS tree
     uint8_t* const __restrict sig // (h + d * len) * n -bytes HT signature
     )
  requires(sphincs_utils::check_ht_height_and_layer(h, d))
{
  constexpr size_t len = sphincs_utils::compute_wots_len<n, w>();
  constexpr uint32_t h_ = h / d;
  constexpr uint32_t mask = (1u << h_) - 1u;
  constexpr size_t xmss_sig_len = (static_cast<size_t>(h_) + len) * n;

  sphincs_adrs::adrs_t adrs{};
  uint8_t rt[n]{};

  adrs.set_layer_address(0u);
  adrs.set_tree_address(idx_tree);

  sphincs_xmss::sign<h_, n, w, v>(msg, sk_seed, idx_leaf, pk_seed, adrs, sig);
  sphincs_xmss::pk_from_sig<h_, n, w, v>(idx_leaf, sig, msg, pk_seed, adrs, rt);

  uint64_t itree = idx_tree;
  uint32_t ileaf = idx_leaf;

  for (uint32_t j = 1; j < d; j++) {
    const size_t off = static_cast<size_t>(j) * xmss_sig_len;
    uint8_t* const sig_ = sig + off;

    ileaf = static_cast<uint32_t>(itree) & mask;
    itree = itree >> h_;

    adrs.set_layer_address(j);
    adrs.set_tree_address(itree);

    sphincs_xmss::sign<h_, n, w, v>(rt, sk_seed, ileaf, pk_seed, adrs, sig_);

    if (j < (d - 1u)) {
      uint8_t t[n]{};

      sphincs_xmss::pk_from_sig<h_, n, w, v>(ileaf, sig_, rt, pk_seed, adrs, t);
      std::memcpy(rt, t, n);
    }
  }
}

// Verifies a hypertree signature of (h + d * len) * n -bytes on a message of
// n -bytes using hypertree public key of n -bytes, when n -bytes public key
// seed, 8 -bytes XMSS tree address and 4 -bytes leaf index of that XMSS tree
// are provided, using algorithm 13, described in section 4.2.5 of SPHINCS+
// specification https://sphincs.org/data/sphincs+-r3.1-specification.pdf
//
// This routine returns truth value in case of successful hypertree signature
// verification, otherwise it returns false.
template<const uint32_t h,
         const uint32_t d,
         const size_t n,
         const size_t w,
         const sphincs_hashing::variant v>
inline static bool
verify(const uint8_t* const __restrict msg,
       const uint8_t* const __restrict sig,
       const uint8_t* const __restrict pk_seed,
       const uint64_t idx_tree,
       const uint32_t idx_leaf,
       const uint8_t* const __restrict pkey)
  requires(sphincs_utils::check_ht_height_and_layer(h, d))
{
  constexpr size_t len = sphincs_utils::compute_wots_len<n, w>();
  constexpr uint32_t h_ = h / d;
  constexpr uint32_t mask = (1u << h_) - 1u;
  constexpr size_t xmss_sig_len = (static_cast<size_t>(h_) + len) * n;

  sphincs_adrs::adrs_t adrs{};
  uint8_t nd[n]{};
  uint8_t tmp[n]{};

  adrs.set_layer_address(0u);
  adrs.set_tree_address(idx_tree);

  sphincs_xmss::pk_from_sig<h_, n, w, v>(idx_leaf, sig, msg, pk_seed, adrs, nd);

  uint64_t itree = idx_tree;
  uint32_t ileaf = idx_leaf;

  for (uint32_t j = 1; j < d; j++) {
    const size_t off = static_cast<size_t>(j) * xmss_sig_len;
    const uint8_t* const sig_ = sig + off;

    ileaf = static_cast<uint32_t>(itree) & mask;
    itree = itree >> h_;

    adrs.set_layer_address(j);
    adrs.set_tree_address(itree);

    sphincs_xmss::pk_from_sig<h_, n, w, v>(ileaf, sig_, nd, pk_seed, adrs, tmp);
    std::memcpy(nd, tmp, n);
  }

  bool flg = false;
  for (size_t i = 0; i < n; i++) {
    flg |= static_cast<bool>(nd[i] ^ pkey[i]);
  }

  return !flg;
}

}
