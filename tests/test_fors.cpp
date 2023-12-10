#include "fors.hpp"
#include <gtest/gtest.h>

// Test correctness of FORS implementation, in standalone mode, using
//
// - Public key generation
// - Signing using private key
// - Recovering public key from signature and message
//
// with random data.
template<size_t n, uint32_t a, uint32_t k, sphincs_hashing::variant v>
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

  bool flag = false;
  for (size_t i = 0; i < n; i++) {
    flag |= static_cast<bool>(pkey0[i] ^ pkey1[i]);
  }

  std::free(sk_seed);
  std::free(pk_seed);
  std::free(msg);
  std::free(pkey0);
  std::free(pkey1);
  std::free(sig);

  EXPECT_FALSE(flag);
}

TEST(SphincsPlus, FORSNISTSecurityLevel1)
{
  test_fors<16, 12, 14, sphincs_hashing::variant::robust>();
  test_fors<16, 12, 14, sphincs_hashing::variant::simple>();
  test_fors<16, 6, 33, sphincs_hashing::variant::robust>();
  test_fors<16, 6, 33, sphincs_hashing::variant::simple>();
}

TEST(SphincsPlus, FORSNISTSecurityLevel3)
{
  test_fors<24, 14, 17, sphincs_hashing::variant::robust>();
  test_fors<24, 14, 17, sphincs_hashing::variant::simple>();
  test_fors<24, 8, 33, sphincs_hashing::variant::robust>();
  test_fors<24, 8, 33, sphincs_hashing::variant::simple>();
}

TEST(SphincsPlus, FORSNISTSecurityLevel5)
{
  test_fors<32, 14, 22, sphincs_hashing::variant::robust>();
  test_fors<32, 14, 22, sphincs_hashing::variant::simple>();
  test_fors<32, 9, 35, sphincs_hashing::variant::robust>();
  test_fors<32, 9, 35, sphincs_hashing::variant::simple>();
}
