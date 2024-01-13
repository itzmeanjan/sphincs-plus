#pragma once
#include "address.hpp"
#include "hashing.hpp"
#include <array>

// One-Time Signature scheme WOTS+, used in SPHINCS+
namespace sphincs_plus_wots {

// Given a n -bytes input message, 4 -bytes wide starting index, 4 -bytes wide
// step count, 32 -bytes WOTS+ hash address and n -bytes public seed, this
// routine computes a n -bytes output, which is the result of iteratively
// applying tweakable hash function `F` on input string `x` for `steps` time.
//
// See algorithm 2 in section 3.2 of SPHINCS+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n, size_t w, sphincs_plus_hashing::variant v>
static inline void
chain(std::span<const uint8_t, n> x,
      const uint32_t s_idx,
      const uint32_t steps,
      sphincs_plus_adrs::wots_hash_t adrs,
      std::span<const uint8_t, n> pk_seed,
      std::span<uint8_t, n> chained)
  requires(sphincs_plus_params::check_w(w))
{
  const uint32_t till = s_idx + steps;
  const bool flg = static_cast<size_t>(till) > (w - 1ul);
  if (flg) {
    std::fill(chained.begin(), chained.end(), 0x00);
    return;
  }

  std::copy(x.begin(), x.end(), chained.begin());
  std::array<uint8_t, n> tmp{};

  for (uint32_t i = s_idx; i < till; i++) {
    adrs.set_hash_address(i);

    sphincs_plus_hashing::f<n, v>(pk_seed, adrs.data, chained, tmp);
    std::copy(tmp.begin(), tmp.end(), chained.begin());
  }
}

// Generates n -bytes WOTS+ compressed public key, given n -bytes secret key
// seed, n -bytes public key seed and 32 -bytes WOTS+ hash address, using
// algorithm 4 defined in section 3.4 of SPHINCS+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n, size_t w, sphincs_plus_hashing::variant v>
static inline void
pkgen(std::span<const uint8_t, n> sk_seed, std::span<const uint8_t, n> pk_seed, sphincs_plus_adrs::wots_hash_t adrs, std::span<uint8_t, n> pkey)
{
  constexpr size_t len = sphincs_plus_utils::compute_wots_len<n, w>();

  sphincs_plus_adrs::wots_pk_t pk_adrs{ adrs };
  sphincs_plus_adrs::wots_prf_t sk_adrs{ adrs };

  sk_adrs.set_type(sphincs_plus_adrs::type_t::WOTS_PRF);
  sk_adrs.set_keypair_address(adrs.get_keypair_address());

  std::array<uint8_t, n> sk_limb{};
  std::array<uint8_t, n * len> chain_limbs{};
  auto _chain_limbs = std::span(chain_limbs);

  for (uint32_t i = 0; i < static_cast<uint32_t>(len); i++) {
    const size_t off = static_cast<size_t>(i) * n;

    sk_adrs.set_chain_address(i);
    sk_adrs.set_hash_address();

    sphincs_plus_hashing::prf<n>(pk_seed, sk_seed, sk_adrs.data, sk_limb);

    adrs.set_chain_address(i);
    adrs.set_hash_address(0);

    chain<n, w, v>(sk_limb, 0u, static_cast<uint32_t>(w - 1), adrs, pk_seed, std::span<uint8_t, n>(_chain_limbs.subspan(off, n)));
  }

  pk_adrs.set_type(sphincs_plus_adrs::type_t::WOTS_PK);
  pk_adrs.set_keypair_address(adrs.get_keypair_address());

  sphincs_plus_hashing::t_l<n, len, v>(pk_seed, pk_adrs.data, chain_limbs, pkey);
}

// Generates n * len -bytes WOTS+ signature, given n -bytes message, n -bytes
// secret key seed, n -bytes public key seed and 32 -bytes WOTS+ hash address,
// using algorithm 5 defined in section 3.5 of SPHINCS+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n, size_t w, sphincs_plus_hashing::variant v>
static inline void
sign(std::span<const uint8_t, n> msg,
     std::span<const uint8_t, n> sk_seed,
     std::span<const uint8_t, n> pk_seed,
     sphincs_plus_adrs::wots_hash_t adrs,
     std::span<uint8_t, n * sphincs_plus_utils::compute_wots_len<n, w>()> sig)
{
  constexpr size_t lgw = sphincs_plus_utils::log2<w>();
  constexpr size_t len1 = sphincs_plus_utils::compute_wots_len1<n, w>();
  constexpr size_t len2 = sphincs_plus_utils::compute_wots_len2<n, w, len1>();
  constexpr size_t len = len1 + len2;
  static_assert(sig.size() == len * n, "Ensure that WOTS+ signature size is correctly computed !");

  uint32_t csum = 0;
  std::array<uint8_t, sig.size()> tmp{};
  auto _tmp = std::span(tmp);

  sphincs_plus_utils::base_w<w, n, len1>(msg, _tmp.template subspan<0, len1>());

  for (size_t i = 0; i < len1; i++) {
    csum += static_cast<uint32_t>(w - 1ul) - static_cast<uint32_t>(tmp[i]);
  }

  if constexpr ((lgw & 7ul) != 0) {
    csum <<= (8ul - ((len2 * lgw) & 7ul));
  }

  constexpr size_t t0 = len2 * lgw;
  constexpr size_t t1 = t0 + 7ul;
  constexpr size_t len_2_bytes = t1 >> 3; // = ceil(t0 / 8)

  const auto bytes = sphincs_plus_utils::to_byte<len_2_bytes>(csum);
  sphincs_plus_utils::base_w<w, len_2_bytes, len2>(bytes, _tmp.template subspan<len1, len2>());

  sphincs_plus_adrs::wots_prf_t sk_adrs{ adrs };

  sk_adrs.set_type(sphincs_plus_adrs::type_t::WOTS_PRF);
  sk_adrs.set_keypair_address(adrs.get_keypair_address());

  std::array<uint8_t, n> sk{};

  for (uint32_t i = 0; i < static_cast<uint32_t>(len); i++) {
    const size_t off = static_cast<size_t>(i) * n;

    sk_adrs.set_chain_address(i);
    sk_adrs.set_hash_address();

    sphincs_plus_hashing::prf<n>(pk_seed, sk_seed, sk_adrs.data, sk);

    adrs.set_chain_address(i);
    adrs.set_hash_address(0);

    const uint32_t steps = static_cast<uint32_t>(_tmp[i]);
    chain<n, w, v>(sk, 0u, steps, adrs, pk_seed, std::span<uint8_t, n>(sig.subspan(off, n)));
  }
}

// Computes n -bytes WOTS+ compressed public key from n -bytes message and
// n * len -bytes signature, when n -bytes public key seed and 32 -bytes WOTS+
// hash address is also provided, using algorithm 6, defined in section 3.6 of
// SPHINCS+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n, size_t w, sphincs_plus_hashing::variant v>
static inline void
pk_from_sig(std::span<const uint8_t, n * sphincs_plus_utils::compute_wots_len<n, w>()> sig,
            std::span<const uint8_t, n> msg,
            std::span<const uint8_t, n> pk_seed,
            sphincs_plus_adrs::wots_hash_t adrs,
            std::span<uint8_t, n> pkey)
{
  constexpr size_t lgw = sphincs_plus_utils::log2<w>();
  constexpr size_t len1 = sphincs_plus_utils::compute_wots_len1<n, w>();
  constexpr size_t len2 = sphincs_plus_utils::compute_wots_len2<n, w, len1>();
  constexpr size_t len = len1 + len2;
  static_assert(sig.size() == len * n, "Ensure that WOTS+ signature size is correctly computed !");

  sphincs_plus_adrs::wots_pk_t pk_adrs{ adrs };

  uint32_t csum = 0;
  std::array<uint8_t, sig.size()> tmp0{};
  auto _tmp0 = std::span(tmp0);

  sphincs_plus_utils::base_w<w, n, len1>(msg, _tmp0.template subspan<0, len1>());

  for (size_t i = 0; i < len1; i++) {
    csum += static_cast<uint32_t>(w - 1ul) - static_cast<uint32_t>(_tmp0[i]);
  }

  if constexpr ((lgw & 7ul) != 0) {
    csum <<= (8ul - ((len2 * lgw) & 7ul));
  }

  constexpr size_t t0 = len2 * lgw;
  constexpr size_t t1 = t0 + 7ul;
  constexpr size_t len_2_bytes = t1 >> 3; // = ceil(t0 / 8)

  const auto bytes = sphincs_plus_utils::to_byte<len_2_bytes>(csum);
  sphincs_plus_utils::base_w<w, len_2_bytes, len2>(bytes, _tmp0.template subspan<len1, len2>());

  std::array<uint8_t, n * len> tmp1{};
  auto _tmp1 = std::span(tmp1);

  for (uint32_t i = 0; i < static_cast<uint32_t>(len); i++) {
    const size_t off = static_cast<size_t>(i) * n;

    adrs.set_chain_address(i);
    adrs.set_hash_address(0);

    const uint32_t sidx = static_cast<uint32_t>(_tmp0[i]);
    const uint32_t steps = static_cast<uint32_t>((w - 1) - _tmp0[i]);
    chain<n, w, v>(std::span<const uint8_t, n>(sig.subspan(off, n)), sidx, steps, adrs, pk_seed, std::span<uint8_t, n>(_tmp1.subspan(off, n)));
  }

  pk_adrs.set_type(sphincs_plus_adrs::type_t::WOTS_PK);
  pk_adrs.set_keypair_address(adrs.get_keypair_address());

  sphincs_plus_hashing::t_l<n, len, v>(pk_seed, pk_adrs.data, _tmp1, pkey);
}

}
