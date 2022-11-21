#pragma once
#include "address.hpp"
#include "hashing.hpp"

// FORS: Forest of Random Subsets
namespace sphincs_fors {

// Computes a n -bytes FORS private key value, living at specified index `idx`,
// given n -bytes public key seed, n -bytes private key seed and 32 -bytes FORS
// tree address, encoding the position of the FORS keypair within SPHINCS+,
// following algorithm 14, as described in section 5.2 of specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<const size_t n>
inline static void
skgen(const uint8_t* const __restrict pk_seed,
      const uint8_t* const __restrict sk_seed,
      sphincs_adrs::fors_tree_t adrs,
      const uint32_t idx,
      uint8_t* const __restrict skey)
{
  sphincs_adrs::fors_prf_t prf_adrs{ adrs };

  prf_adrs.set_type(sphincs_adrs::type_t::FORS_PRF);
  prf_adrs.set_keypair_address(adrs.get_keypair_address());
  prf_adrs.set_tree_index(idx);

  sphincs_hashing::prf<n>(pk_seed, sk_seed, prf_adrs.data, skey);
}

}
