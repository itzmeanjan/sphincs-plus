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
{
  sphincs_adrs::adrs_t adrs{};

  adrs.set_layer_address(d - 1u);
  adrs.set_tree_address(0ul);

  sphincs_xmss::pkgen<h / d, n, w, v>(sk_seed, pk_seed, adrs, pkey);
}

}
