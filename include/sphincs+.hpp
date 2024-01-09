#pragma once
#include "fors.hpp"
#include "hypertree.hpp"

// SPHINCS+ Signature Scheme, with generic API
namespace sphincs_plus {

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
template<size_t n, uint32_t h, uint32_t d, size_t w, sphincs_plus_hashing::variant v>
static inline void
keygen(std::span<const uint8_t, n> sk_seed,
       std::span<const uint8_t, n> sk_prf,
       std::span<const uint8_t, n> pk_seed,
       std::span<uint8_t, sphincs_plus_utils::get_sphincs_skey_len<n>()> skey,
       std::span<uint8_t, sphincs_plus_utils::get_sphincs_pkey_len<n>()> pkey)
  requires(sphincs_plus_params::check_keygen_params<n, h, d, w, v>())
{
  std::array<uint8_t, n> pk_root{};
  sphincs_plus_ht::pkgen<h, d, n, w, v>(sk_seed, pk_seed, pk_root);

  // prepare 2*n -bytes public key
  std::copy(pk_seed.begin(), pk_seed.end(), pkey.template subspan<0, n>().begin());
  std::copy(pk_root.begin(), pk_root.end(), pkey.template subspan<n, n>().begin());

  // prepare 4*n -bytes secret key, which also keeps a copy of public key ( used during signing )
  std::copy(sk_seed.begin(), sk_seed.end(), skey.template subspan<0, n>().begin());
  std::copy(sk_prf.begin(), sk_prf.end(), skey.template subspan<n, n>().begin());
  std::copy(pkey.begin(), pkey.end(), skey.template subspan<2 * n, pkey.size()>().begin());
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
template<size_t n, uint32_t h, uint32_t d, uint32_t a, uint32_t k, size_t w, sphincs_plus_hashing::variant v, bool randomize = false>
static inline void
sign(std::span<const uint8_t> msg,
     std::span<const uint8_t, sphincs_plus_utils::get_sphincs_skey_len<n>()> skey,
     std::span<const uint8_t, n * randomize> rand_bytes,
     std::span<uint8_t, sphincs_plus_utils::get_sphincs_sig_len<n, h, d, a, k, w>()> sig)
  requires(sphincs_plus_params::check_sign_verify_params<n, h, d, a, k, w, v>())
{
  constexpr size_t md_len = static_cast<size_t>((k * a + 7) >> 3);
  constexpr size_t itree_len = static_cast<size_t>((h - (h / d) + 7) >> 3);
  constexpr size_t ileaf_len = static_cast<size_t>(((h / d) + 7) >> 3);
  constexpr size_t m = md_len + itree_len + ileaf_len;

  constexpr size_t fors_sl = sphincs_plus_utils::compute_fors_sig_len<n, a, k>();

  auto _sig0 = sig.template subspan<0, n>();                                                              // Randomness portion
  auto _sig1 = sig.template subspan<n, fors_sl>();                                                        // FORS signature portion
  auto _sig2 = sig.template subspan<n + fors_sl, sphincs_plus_utils::compute_ht_sig_len<h, d, n, w>()>(); // HT signature portion

  auto sk_seed = skey.template subspan<0, n>();
  auto sk_prf = skey.template subspan<n, n>();
  auto pk_seed = skey.template subspan<2 * n, n>();
  auto pk_root = skey.template subspan<3 * n, n>();

  constexpr uint32_t h_ = h - (h / d);
  constexpr bool flg = h_ == 64u;

  constexpr uint64_t mask0 = (1ul << (h_ - 1u * flg)) + (1ul << 63) * flg - 1ul;
  constexpr uint32_t mask1 = (1u << (h / d)) - 1ul;

  std::array<uint8_t, n> opt{};
  std::array<uint8_t, m> dig{};
  auto _dig = std::span(dig);

  if constexpr (randomize) {
    std::copy(rand_bytes.begin(), rand_bytes.end(), opt.begin());
  } else {
    std::copy(pk_seed.begin(), pk_seed.end(), opt.begin());
  }

  sphincs_plus_hashing::prf_msg<n>(sk_prf, opt, msg, _sig0);
  sphincs_plus_hashing::h_msg<n, m>(_sig0, pk_seed, pk_root, msg, _dig);

  auto md = _dig.template subspan<0, md_len>();
  auto tmp_itree = _dig.template subspan<md_len, itree_len>();
  auto tmp_ileaf = _dig.template subspan<md_len + itree_len, ileaf_len>();

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

  sphincs_plus_adrs::fors_tree_t adrs{};

  adrs.set_layer_address(0u);
  adrs.set_tree_address(itree);
  adrs.set_type(sphincs_plus_adrs::type_t::FORS_TREE);
  adrs.set_keypair_address(ileaf);

  std::array<uint8_t, n> tmp{};

  sphincs_plus_fors::sign<n, a, k, v>(md, sk_seed, pk_seed, adrs, _sig1);
  sphincs_plus_fors::pk_from_sig<n, a, k, v>(_sig1, md, pk_seed, adrs, tmp);
  sphincs_plus_ht::sign<h, d, n, w, v>(tmp, sk_seed, pk_seed, itree, ileaf, _sig2);
}

// Verifies a SPHINCS+ signature on a message of mlen -bytes using SPHINCS+
// public key of length 2*n -bytes, returning truth value ( boolean result ) in
// case of successful signature verification, following algorithm 21, as
// described in section 6.5 of specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n, uint32_t h, uint32_t d, uint32_t a, uint32_t k, size_t w, sphincs_plus_hashing::variant v>
static inline bool
verify(std::span<const uint8_t> msg,
       std::span<const uint8_t, sphincs_plus_utils::get_sphincs_sig_len<n, h, d, a, k, w>()> sig,
       std::span<const uint8_t, sphincs_plus_utils::get_sphincs_pkey_len<n>()> pkey)
  requires(sphincs_plus_params::check_sign_verify_params<n, h, d, a, k, w, v>())
{
  constexpr size_t md_len = static_cast<size_t>((k * a + 7) >> 3);
  constexpr size_t itree_len = static_cast<size_t>((h - (h / d) + 7) >> 3);
  constexpr size_t ileaf_len = static_cast<size_t>(((h / d) + 7) >> 3);
  constexpr size_t m = md_len + itree_len + ileaf_len;

  constexpr size_t fors_sl = sphincs_plus_utils::compute_fors_sig_len<n, a, k>();

  auto _sig0 = sig.template subspan<0, n>();                                                              // Randomness portion
  auto _sig1 = sig.template subspan<n, fors_sl>();                                                        // FORS signature portion
  auto _sig2 = sig.template subspan<n + fors_sl, sphincs_plus_utils::compute_ht_sig_len<h, d, n, w>()>(); // HT signature portion

  auto pk_seed = pkey.template subspan<0, n>();
  auto pk_root = pkey.template subspan<n, n>();

  constexpr uint32_t h_ = h - (h / d);
  constexpr bool flg = h_ == 64u;

  constexpr uint64_t mask0 = (1ul << (h_ - 1u * flg)) + (1ul << 63) * flg - 1ul;
  constexpr uint32_t mask1 = (1u << (h / d)) - 1ul;

  std::array<uint8_t, m> dig{};
  auto _dig = std::span(dig);
  sphincs_plus_hashing::h_msg<n, m>(_sig0, pk_seed, pk_root, msg, _dig);

  auto md = _dig.template subspan<0, md_len>();
  auto tmp_itree = _dig.template subspan<md_len, itree_len>();
  auto tmp_ileaf = _dig.template subspan<md_len + itree_len, ileaf_len>();

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

  sphincs_plus_adrs::fors_tree_t adrs{};

  adrs.set_layer_address(0u);
  adrs.set_tree_address(itree);
  adrs.set_type(sphincs_plus_adrs::type_t::FORS_TREE);
  adrs.set_keypair_address(ileaf);

  std::array<uint8_t, n> tmp{};
  sphincs_plus_fors::pk_from_sig<n, a, k, v>(_sig1, md, pk_seed, adrs, tmp);

  return sphincs_plus_ht::verify<h, d, n, w, v>(tmp, _sig2, pk_seed, itree, ileaf, pk_root);
}

}
