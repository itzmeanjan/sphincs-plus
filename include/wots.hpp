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

}
