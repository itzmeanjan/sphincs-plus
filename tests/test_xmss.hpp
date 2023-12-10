#pragma once
#include "xmss.hpp"

// Test functional correctness of SPHINCS+
namespace test_sphincs {

// Test correctness of fixed input-length XMSS implementation, in standalone
// mode, using
//
// - Public key generation
// - Signing using private key
// - Verifying signature by recovering public key from message and signature
//
// with random data.
template<const uint32_t h,
         const size_t n,
         const size_t w,
         const sphincs_hashing::variant v>
inline static void
test_xmss()
{
  constexpr size_t len = sphincs_utils::compute_wots_len<n, w>();
  constexpr size_t sig_len = len * n + h * n; // XMSS signature in bytes
  constexpr uint32_t idx = 0u;

  // Input
  uint8_t* sk_seed = static_cast<uint8_t*>(std::malloc(sizeof(uint8_t) * n));
  uint8_t* pk_seed = static_cast<uint8_t*>(std::malloc(sizeof(uint8_t) * n));
  sphincs_adrs::adrs_t adrs{};
  uint8_t* msg = static_cast<uint8_t*>(std::malloc(sizeof(uint8_t) * n));

  // Output
  uint8_t* pkey0 = static_cast<uint8_t*>(std::malloc(sizeof(uint8_t) * n));
  uint8_t* pkey1 = static_cast<uint8_t*>(std::malloc(sizeof(uint8_t) * n));
  uint8_t* sig = static_cast<uint8_t*>(std::malloc(sizeof(uint8_t) * sig_len));

  sphincs_utils::random_data<uint8_t>(sk_seed, n);
  sphincs_utils::random_data<uint8_t>(pk_seed, n);
  sphincs_utils::random_data<uint8_t>(adrs.data, 32);
  sphincs_utils::random_data<uint8_t>(msg, n);

  sphincs_xmss::pkgen<h, n, w, v>(sk_seed, pk_seed, adrs, pkey0);
  sphincs_xmss::sign<h, n, w, v>(msg, sk_seed, idx, pk_seed, adrs, sig);
  sphincs_xmss::pk_from_sig<h, n, w, v>(idx, sig, msg, pk_seed, adrs, pkey1);

  bool flg = false;
  for (size_t i = 0; i < n; i++) {
    flg |= static_cast<bool>(pkey0[i] ^ pkey1[i]);
  }

  std::free(sk_seed);
  std::free(pk_seed);
  std::free(msg);
  std::free(pkey0);
  std::free(pkey1);
  std::free(sig);

  assert(!flg);
}

}
