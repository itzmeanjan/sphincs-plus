#include "hypertree.hpp"
#include <gtest/gtest.h>

// Test correctness of HyperTree implementation, in standalone mode, using
//
// - Public key generation
// - Signing using private key
// - Verifying signature by using public key and message
//
// with random data.
template<const uint32_t h, const uint32_t d, const size_t n, const size_t w, const sphincs_hashing::variant v>
inline static void
test_hypertree()
{
  constexpr size_t len = sphincs_utils::compute_wots_len<n, w>();
  constexpr size_t sig_len = (h + d * len) * n; // HT signature in bytes
  constexpr uint64_t itree = 0ul;               // XMSS tree index
  constexpr uint32_t ileaf = 0u;                // Leaf index in that XMSS tree

  // Input
  uint8_t* sk_seed = static_cast<uint8_t*>(std::malloc(sizeof(uint8_t) * n));
  uint8_t* pk_seed = static_cast<uint8_t*>(std::malloc(sizeof(uint8_t) * n));
  uint8_t* msg = static_cast<uint8_t*>(std::malloc(sizeof(uint8_t) * n));

  // Output
  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(sizeof(uint8_t) * n));
  uint8_t* sig = static_cast<uint8_t*>(std::malloc(sizeof(uint8_t) * sig_len));

  sphincs_utils::random_data<uint8_t>(sk_seed, n);
  sphincs_utils::random_data<uint8_t>(pk_seed, n);
  sphincs_utils::random_data<uint8_t>(msg, n);

  sphincs_ht::pkgen<h, d, n, w, v>(sk_seed, pk_seed, pkey);
  sphincs_ht::sign<h, d, n, w, v>(msg, sk_seed, pk_seed, itree, ileaf, sig);
  const bool flag = sphincs_ht::verify<h, d, n, w, v>(msg, sig, pk_seed, itree, ileaf, pkey);

  std::free(sk_seed);
  std::free(pk_seed);
  std::free(msg);
  std::free(pkey);
  std::free(sig);

  EXPECT_TRUE(flag);
}

TEST(SphincsPlus, HyperTreeNISTSecurityLevel1)
{
  test_hypertree<63, 7, 16, 16, sphincs_hashing::variant::robust>();
  test_hypertree<63, 7, 16, 16, sphincs_hashing::variant::simple>();
  test_hypertree<66, 22, 16, 16, sphincs_hashing::variant::robust>();
  test_hypertree<66, 22, 16, 16, sphincs_hashing::variant::simple>();
}

TEST(SphincsPlus, HyperTreeNISTSecurityLevel3)
{
  test_hypertree<63, 7, 24, 16, sphincs_hashing::variant::robust>();
  test_hypertree<63, 7, 24, 16, sphincs_hashing::variant::simple>();
  test_hypertree<66, 22, 24, 16, sphincs_hashing::variant::robust>();
  test_hypertree<66, 22, 24, 16, sphincs_hashing::variant::simple>();
}

TEST(SphincsPlus, HyperTreeNISTSecurityLevel5)
{
  test_hypertree<64, 8, 32, 16, sphincs_hashing::variant::robust>();
  test_hypertree<64, 8, 32, 16, sphincs_hashing::variant::simple>();
  test_hypertree<68, 17, 32, 16, sphincs_hashing::variant::robust>();
  test_hypertree<68, 17, 32, 16, sphincs_hashing::variant::simple>();
}
