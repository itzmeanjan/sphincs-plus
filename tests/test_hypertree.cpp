#include "hypertree.hpp"
#include "prng.hpp"
#include <gtest/gtest.h>
#include <vector>

// Test correctness of HyperTree implementation, in standalone mode, using
//
// - Public key generation
// - Signing using private key
// - Verifying signature by using public key and message
//
// with random data.
template<uint32_t h, uint32_t d, size_t n, size_t w, sphincs_plus_hashing::variant v>
static inline void
test_hypertree()
{
  constexpr size_t len = sphincs_plus_utils::compute_wots_len<n, w>();
  constexpr size_t sig_len = (h + d * len) * n; // HT signature in bytes
  constexpr uint64_t itree = 0ul;               // XMSS tree index
  constexpr uint32_t ileaf = 0u;                // Leaf index in that XMSS tree

  // Input
  std::vector<uint8_t> sk_seed(n, 0);
  std::vector<uint8_t> pk_seed(n, 0);
  std::vector<uint8_t> msg(n, 0);

  auto _sk_seed = std::span<uint8_t, n>(sk_seed);
  auto _pk_seed = std::span<uint8_t, n>(pk_seed);
  auto _msg = std::span<uint8_t, n>(msg);

  // Output
  std::vector<uint8_t> pkey(n, 0);
  std::vector<uint8_t> sig(sig_len, 0);

  auto _pkey = std::span<uint8_t, n>(pkey);
  auto _sig = std::span<uint8_t, sig_len>(sig);

  prng::prng_t prng;
  prng.read(_sk_seed);
  prng.read(_pk_seed);
  prng.read(_msg);

  sphincs_plus_ht::pkgen<h, d, n, w, v>(_sk_seed, _pk_seed, _pkey);
  sphincs_plus_ht::sign<h, d, n, w, v>(_msg, _sk_seed, _pk_seed, itree, ileaf, _sig);
  const bool flag = sphincs_plus_ht::verify<h, d, n, w, v>(_msg, _sig, _pk_seed, itree, ileaf, _pkey);

  EXPECT_TRUE(flag);
}

TEST(SphincsPlus, HyperTreeNISTSecurityLevel1)
{
  test_hypertree<63, 7, 16, 16, sphincs_plus_hashing::variant::robust>();
  test_hypertree<63, 7, 16, 16, sphincs_plus_hashing::variant::simple>();
  test_hypertree<66, 22, 16, 16, sphincs_plus_hashing::variant::robust>();
  test_hypertree<66, 22, 16, 16, sphincs_plus_hashing::variant::simple>();
}

TEST(SphincsPlus, HyperTreeNISTSecurityLevel3)
{
  test_hypertree<63, 7, 24, 16, sphincs_plus_hashing::variant::robust>();
  test_hypertree<63, 7, 24, 16, sphincs_plus_hashing::variant::simple>();
  test_hypertree<66, 22, 24, 16, sphincs_plus_hashing::variant::robust>();
  test_hypertree<66, 22, 24, 16, sphincs_plus_hashing::variant::simple>();
}

TEST(SphincsPlus, HyperTreeNISTSecurityLevel5)
{
  test_hypertree<64, 8, 32, 16, sphincs_plus_hashing::variant::robust>();
  test_hypertree<64, 8, 32, 16, sphincs_plus_hashing::variant::simple>();
  test_hypertree<68, 17, 32, 16, sphincs_plus_hashing::variant::robust>();
  test_hypertree<68, 17, 32, 16, sphincs_plus_hashing::variant::simple>();
}
