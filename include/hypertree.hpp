#pragma once
#include "xmss.hpp"

// HT: The Hypertree, used in SPHINCS+
namespace sphincs_plus_ht {

// Computes n -bytes hypertree public key, which is the root of the single XMSS
// tree living on top layer ( i.e. layer index d - 1 ) of the hypertree, given n
// -bytes secret key seed & n -bytes public key seed, using algorithm 11,
// described in section 4.2.2 of SPHINCS+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<uint32_t h, uint32_t d, size_t n, size_t w, sphincs_plus_hashing::variant v>
static inline void
pkgen(std::span<const uint8_t, n> sk_seed, std::span<const uint8_t, n> pk_seed, std::span<uint8_t, n> pkey)
  requires(sphincs_plus_params::check_ht_height_and_layer(h, d))
{
  sphincs_plus_adrs::adrs_t adrs{};

  adrs.set_layer_address(d - 1u);
  adrs.set_tree_address(0ul);

  sphincs_plus_xmss::pkgen<h / d, n, w, v>(sk_seed, pk_seed, adrs, pkey);
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
template<uint32_t h, uint32_t d, size_t n, size_t w, sphincs_plus_hashing::variant v>
static inline void
sign(std::span<const uint8_t, n> msg,
     std::span<const uint8_t, n> sk_seed,
     std::span<const uint8_t, n> pk_seed,
     const uint64_t idx_tree,
     const uint32_t idx_leaf,
     std::span<uint8_t, (h + d * sphincs_plus_utils::compute_wots_len<n, w>()) * n> sig)
  requires(sphincs_plus_params::check_ht_height_and_layer(h, d))
{
  constexpr size_t len = sphincs_plus_utils::compute_wots_len<n, w>();
  constexpr uint32_t h_ = h / d;
  constexpr size_t xmss_sig_len = (static_cast<size_t>(h_) + len) * n;
  constexpr uint32_t mask = (1u << h_) - 1u;

  sphincs_plus_adrs::adrs_t adrs{};
  std::array<uint8_t, n> rt{};
  auto xmss_sig = sig.template subspan<0, xmss_sig_len>();

  adrs.set_layer_address(0u);
  adrs.set_tree_address(idx_tree);

  sphincs_plus_xmss::sign<h_, n, w, v>(msg, sk_seed, idx_leaf, pk_seed, adrs, xmss_sig);
  sphincs_plus_xmss::pk_from_sig<h_, n, w, v>(idx_leaf, xmss_sig, msg, pk_seed, adrs, rt);

  uint64_t itree = idx_tree;
  uint32_t ileaf = idx_leaf;

  for (uint32_t j = 1; j < d; j++) {
    const size_t off = static_cast<size_t>(j) * xmss_sig_len;
    auto _sig = std::span<uint8_t, xmss_sig_len>(sig.subspan(off, xmss_sig_len));

    ileaf = static_cast<uint32_t>(itree) & mask;
    itree = itree >> h_;

    adrs.set_layer_address(j);
    adrs.set_tree_address(itree);

    sphincs_plus_xmss::sign<h_, n, w, v>(rt, sk_seed, ileaf, pk_seed, adrs, _sig);

    if (j < (d - 1u)) {
      std::array<uint8_t, n> t{};

      sphincs_plus_xmss::pk_from_sig<h_, n, w, v>(ileaf, _sig, rt, pk_seed, adrs, t);
      std::copy(t.begin(), t.end(), rt.begin());
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
template<uint32_t h, uint32_t d, size_t n, size_t w, sphincs_plus_hashing::variant v>
static inline bool
verify(std::span<const uint8_t, n> msg,
       std::span<const uint8_t, (h + d * sphincs_plus_utils::compute_wots_len<n, w>()) * n> sig,
       std::span<const uint8_t, n> pk_seed,
       const uint64_t idx_tree,
       const uint32_t idx_leaf,
       std::span<const uint8_t, n> pkey)
  requires(sphincs_plus_params::check_ht_height_and_layer(h, d))
{
  constexpr size_t len = sphincs_plus_utils::compute_wots_len<n, w>();
  constexpr uint32_t h_ = h / d;
  constexpr size_t xmss_sig_len = (static_cast<size_t>(h_) + len) * n;
  constexpr uint32_t mask = (1u << h_) - 1u;

  sphincs_plus_adrs::adrs_t adrs{};
  std::array<uint8_t, n> nd{};
  std::array<uint8_t, n> tmp{};

  adrs.set_layer_address(0u);
  adrs.set_tree_address(idx_tree);

  auto xmss_sig = sig.template subspan<0, xmss_sig_len>();
  sphincs_plus_xmss::pk_from_sig<h_, n, w, v>(idx_leaf, xmss_sig, msg, pk_seed, adrs, nd);

  uint64_t itree = idx_tree;
  uint32_t ileaf = idx_leaf;

  for (uint32_t j = 1; j < d; j++) {
    const size_t off = static_cast<size_t>(j) * xmss_sig_len;
    auto _sig = std::span<uint8_t, xmss_sig_len>(sig.subspan(off, xmss_sig_len));

    ileaf = static_cast<uint32_t>(itree) & mask;
    itree = itree >> h_;

    adrs.set_layer_address(j);
    adrs.set_tree_address(itree);

    sphincs_plus_xmss::pk_from_sig<h_, n, w, v>(ileaf, _sig, nd, pk_seed, adrs, tmp);
    std::copy(tmp.begin(), tmp.end(), nd.begin());
  }

  bool flg = false;
  for (size_t i = 0; i < n; i++) {
    flg |= static_cast<bool>(nd[i] ^ pkey[i]);
  }

  return !flg;
}

}
