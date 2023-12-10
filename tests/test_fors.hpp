#pragma once
#include "fors.hpp"

// Test functional correctness of SPHINCS+
namespace test_sphincs {

// Test correctness of FORS implementation, in standalone mode, using
//
// - Public key generation
// - Signing using private key
// - Recovering public key from signature and message
//
// with random data.
template<const size_t n,
         const uint32_t a,
         const uint32_t k,
         const sphincs_hashing::variant v>
inline static void
test_fors()
{
  constexpr size_t msg_len = (k * a + 7) / 8; // = ⌈(k * a) / 8⌉ -bytes
  constexpr size_t sig_len = k * n * (a + 1); // FORS signature length

  // Input
  uint8_t* sk_seed = static_cast<uint8_t*>(std::malloc(sizeof(uint8_t) * n));
  uint8_t* pk_seed = static_cast<uint8_t*>(std::malloc(sizeof(uint8_t) * n));
  sphincs_adrs::fors_tree_t adrs{};
  uint8_t* msg = static_cast<uint8_t*>(std::malloc(sizeof(uint8_t) * msg_len));

  // Output
  uint8_t* pkey0 = static_cast<uint8_t*>(std::malloc(sizeof(uint8_t) * n));
  uint8_t* pkey1 = static_cast<uint8_t*>(std::malloc(sizeof(uint8_t) * n));
  uint8_t* sig = static_cast<uint8_t*>(std::malloc(sizeof(uint8_t) * sig_len));

  sphincs_utils::random_data<uint8_t>(sk_seed, n);
  sphincs_utils::random_data<uint8_t>(pk_seed, n);
  sphincs_utils::random_data<uint8_t>(msg, msg_len);

  sphincs_fors::pkgen<n, a, k, v>(sk_seed, pk_seed, adrs, pkey0);
  sphincs_fors::sign<n, a, k, v>(msg, sk_seed, pk_seed, adrs, sig);
  sphincs_fors::pk_from_sig<n, a, k, v>(sig, msg, pk_seed, adrs, pkey1);

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
