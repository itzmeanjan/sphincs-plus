#include "wots.hpp"
#include <gtest/gtest.h>

// Test correctness of WOTS+ implementation, in standalone mode, using
//
// - Public key generation
// - Signing using private key
// - Verifying signature by recovering public key from message and signature
//
// Note, WOTS+ is the One-Time Signature scheme that is used in SPHINCS+, though
// its API is not fit for standalone usage, here random data filled WOTS+ hash
// address is used for ensuring correctness - which should work correctly.
template<size_t n, size_t w, sphincs_hashing::variant v>
inline static void
test_wots_plus()
{
  constexpr size_t len = sphincs_utils::compute_wots_len<n, w>();

  // Input
  uint8_t* sk_seed = static_cast<uint8_t*>(std::malloc(sizeof(uint8_t) * n));
  uint8_t* pk_seed = static_cast<uint8_t*>(std::malloc(sizeof(uint8_t) * n));
  sphincs_adrs::wots_hash_t adrs{};
  uint8_t* msg = static_cast<uint8_t*>(std::malloc(sizeof(uint8_t) * n));

  // Output
  uint8_t* pkey0 = static_cast<uint8_t*>(std::malloc(sizeof(uint8_t) * n));
  uint8_t* pkey1 = static_cast<uint8_t*>(std::malloc(sizeof(uint8_t) * n));
  uint8_t* sig = static_cast<uint8_t*>(std::malloc(sizeof(uint8_t) * n * len));

  sphincs_utils::random_data<uint8_t>(sk_seed, n);
  sphincs_utils::random_data<uint8_t>(pk_seed, n);
  sphincs_utils::random_data<uint8_t>(adrs.data, 32);
  sphincs_utils::random_data<uint8_t>(msg, n);

  sphincs_wots::pkgen<n, w, v>(sk_seed, pk_seed, adrs, pkey0);
  sphincs_wots::sign<n, w, v>(msg, sk_seed, pk_seed, adrs, sig);
  sphincs_wots::pk_from_sig<n, w, v>(sig, msg, pk_seed, adrs, pkey1);

  bool flag = false;
  for (size_t i = 0; i < n; i++) {
    flag |= static_cast<bool>(pkey0[i] ^ pkey1[i]);
  }

  std::free(sk_seed);
  std::free(pk_seed);
  std::free(pkey0);
  std::free(pkey1);
  std::free(msg);
  std::free(sig);

  EXPECT_FALSE(flag);
}

TEST(SphincsPlus, WOTS_PlusNISTSecurityLevel1)
{
  test_wots_plus<16, 16, sphincs_hashing::variant::robust>();
  test_wots_plus<16, 16, sphincs_hashing::variant::simple>();
}

TEST(SphincsPlus, WOTS_PlusNISTSecurityLevel3)
{
  test_wots_plus<24, 16, sphincs_hashing::variant::robust>();
  test_wots_plus<24, 16, sphincs_hashing::variant::simple>();
}

TEST(SphincsPlus, WOTS_PlusNISTSecurityLevel5)
{
  test_wots_plus<32, 16, sphincs_hashing::variant::robust>();
  test_wots_plus<32, 16, sphincs_hashing::variant::simple>();
}
