#pragma once
#include "address.hpp"
#include "hashing.hpp"

// One-Time Signature scheme WOTS+, used in SPHINCS+
namespace sphincs_wots {

// Given a n -bytes input message, 4 -bytes wide starting index, 4 -bytes wide
// step count, 32 -bytes WOTS+ hash address and n -bytes public seed, this
// routine computes a n -bytes output, which is the result of iteratively
// applying tweakable hash function `F` on input string `x` for `steps` time.
//
// See algorithm 2 in section 3.2 of SPHINCS+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<const size_t n, const size_t w, const sphincs_hashing::variant v>
inline static void
chain(const uint8_t* const __restrict x,       // n -bytes
      const uint32_t s_idx,                    // starting index
      const uint32_t steps,                    // # -of steps
      sphincs_adrs::wots_hash_t adrs,          // 32 -bytes WOTS+ hash address
      const uint8_t* const __restrict pk_seed, // n -bytes public key seed
      uint8_t* const __restrict chained        // n -bytes output
      )
  requires(sphincs_utils::check_w(w))
{
  const uint32_t till = s_idx + steps;
  const bool flg = static_cast<size_t>(till) > (w - 1ul);
  if (flg) {
    std::memset(chained, 0, n);
    return;
  }

  std::memcpy(chained, x, n);
  uint8_t tmp[n]{};

  for (uint32_t i = s_idx; i < till; i++) {
    adrs.set_hash_address(i);

    sphincs_hashing::f<n, v>(pk_seed, adrs.data, chained, tmp);
    std::memcpy(chained, tmp, n);
  }
}

// Generates n -bytes WOTS+ compressed public key, given n -bytes secret key
// seed, n -bytes public key seed and 32 -bytes WOTS+ hash address, using
// algorithm 4 defined in section 3.4 of SPHINCS+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<const size_t n, const size_t w, const sphincs_hashing::variant v>
inline static void
pkgen(const uint8_t* const __restrict sk_seed, // n -bytes secret key seed
      const uint8_t* const __restrict pk_seed, // n -bytes public key seed
      sphincs_adrs::wots_hash_t adrs,          // 32 -bytes WOTS+ hash address
      uint8_t* const __restrict pkey           // n -bytes public key
)
{
  constexpr size_t len = sphincs_utils::compute_wots_len<n, w>();

  sphincs_adrs::wots_pk_t pk_adrs{ adrs };
  sphincs_adrs::wots_prf_t sk_adrs{ adrs };

  sk_adrs.set_type(sphincs_adrs::type_t::WOTS_PRF);
  sk_adrs.set_keypair_address(adrs.get_keypair_address());

  uint8_t sk_limb[n]{};
  uint8_t chain_limbs[n * len]{};

  for (uint32_t i = 0; i < static_cast<uint32_t>(len); i++) {
    const size_t off = static_cast<size_t>(i) * n;

    sk_adrs.set_chain_address(i);
    sk_adrs.set_hash_address();

    sphincs_hashing::prf<n>(pk_seed, sk_seed, sk_adrs.data, sk_limb);

    adrs.set_chain_address(i);
    adrs.set_hash_address(0);

    chain<n, w, v>(sk_limb, 0, w - 1, adrs, pk_seed, chain_limbs + off);
  }

  pk_adrs.set_type(sphincs_adrs::type_t::WOTS_PK);
  pk_adrs.set_keypair_address(adrs.get_keypair_address());

  sphincs_hashing::t_l<n, len, v>(pk_seed, pk_adrs.data, chain_limbs, pkey);
}

}
