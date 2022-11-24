#pragma once
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
{
  uint8_t pk_root[n]{};
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

}
