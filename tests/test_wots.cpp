#include "prng.hpp"
#include "wots.hpp"
#include <gtest/gtest.h>
#include <vector>

// Test correctness of WOTS+ implementation, in standalone mode, using
//
// - Public key generation
// - Signing using private key
// - Verifying signature by recovering public key from message and signature
//
// Note, WOTS+ is the One-Time Signature scheme that is used in SPHINCS+, though
// its API is not fit for standalone usage, here random data filled WOTS+ hash
// address is used for ensuring correctness - which should work correctly.
template<size_t n, size_t w, sphincs_plus_hashing::variant v>
static inline void
test_wots_plus()
{
  constexpr size_t len = sphincs_plus_utils::compute_wots_len<n, w>();

  // Input
  std::vector<uint8_t> sk_seed(n, 0);
  std::vector<uint8_t> pk_seed(n, 0);
  sphincs_plus_adrs::wots_hash_t adrs{};
  std::vector<uint8_t> msg(n, 0);

  auto _sk_seed = std::span<uint8_t, n>(sk_seed);
  auto _pk_seed = std::span<uint8_t, n>(pk_seed);
  auto _msg = std::span<uint8_t, n>(msg);

  // Output
  std::vector<uint8_t> pkey0(n, 0);
  std::vector<uint8_t> pkey1(n, 0);
  std::vector<uint8_t> sig(n * len, 0);

  auto _pkey0 = std::span<uint8_t, n>(pkey0);
  auto _pkey1 = std::span<uint8_t, n>(pkey1);
  auto _sig = std::span<uint8_t, n * len>(sig);

  prng::prng_t prng;
  prng.read(_sk_seed);
  prng.read(_pk_seed);
  prng.read(adrs.data);
  prng.read(_msg);

  sphincs_plus_wots::pkgen<n, w, v>(_sk_seed, _pk_seed, adrs, _pkey0);
  sphincs_plus_wots::sign<n, w, v>(_msg, _sk_seed, _pk_seed, adrs, _sig);
  sphincs_plus_wots::pk_from_sig<n, w, v>(_sig, _msg, _pk_seed, adrs, _pkey1);

  EXPECT_EQ(pkey0, pkey1);
}

TEST(SphincsPlus, WOTS_PlusNISTSecurityLevel1)
{
  test_wots_plus<16, 16, sphincs_plus_hashing::variant::robust>();
  test_wots_plus<16, 16, sphincs_plus_hashing::variant::simple>();
}

TEST(SphincsPlus, WOTS_PlusNISTSecurityLevel3)
{
  test_wots_plus<24, 16, sphincs_plus_hashing::variant::robust>();
  test_wots_plus<24, 16, sphincs_plus_hashing::variant::simple>();
}

TEST(SphincsPlus, WOTS_PlusNISTSecurityLevel5)
{
  test_wots_plus<32, 16, sphincs_plus_hashing::variant::robust>();
  test_wots_plus<32, 16, sphincs_plus_hashing::variant::simple>();
}
