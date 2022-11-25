#pragma once
#include "sphincs.hpp"
#include "test_fors.hpp"
#include "test_hypertree.hpp"
#include "test_wots.hpp"
#include "test_xmss.hpp"

// Test functional correctness of SPHINCS+
namespace test_sphincs {

// Test correctness of SPHINCS+ implementation, using
//
// - Keypair generation
// - Signing message using private key
// - Verifying signature using public key and message
//
// with random data.
template<const size_t n,
         const uint32_t h,
         const uint32_t d,
         const uint32_t a,
         const uint32_t k,
         const size_t w,
         const sphincs_hashing::variant v,
         const bool randomize = false>
inline static void
test_sphincs_plus(const size_t mlen)
{
  namespace utils = sphincs_utils;
  constexpr size_t pklen = utils::get_sphincs_pkey_len<n>();
  constexpr size_t sklen = utils::get_sphincs_skey_len<n>();
  constexpr size_t siglen = utils::get_sphincs_sig_len<n, h, d, a, k, w>();

  // acquire memory resources
  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));
  uint8_t* msg = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* sig = static_cast<uint8_t*>(std::malloc(siglen));

  sphincs_utils::random_data<uint8_t>(msg, mlen);

  sphincs::keygen<n, h, d, w, v>(skey, pkey);
  sphincs::sign<n, h, d, a, k, w, v, randomize>(msg, mlen, skey, sig);
  const bool flg = sphincs::verify<n, h, d, a, k, w, v>(msg, mlen, sig, pkey);

  // release memory resources
  std::free(pkey);
  std::free(skey);
  std::free(msg);
  std::free(sig);

  assert(flg);
}

}
