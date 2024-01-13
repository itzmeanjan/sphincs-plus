#include "fors.hpp"
#include "prng.hpp"
#include <gtest/gtest.h>
#include <vector>

// Test correctness of FORS implementation, in standalone mode, using
//
// - Public key generation
// - Signing using private key
// - Recovering public key from signature and message
//
// with random data.
template<size_t n, uint32_t a, uint32_t k, sphincs_plus_hashing::variant v>
static inline void
test_fors()
{
  constexpr size_t msg_len = (k * a + 7) / 8; // = ⌈(k * a) / 8⌉ -bytes
  constexpr size_t sig_len = k * n * (a + 1); // FORS signature length

  // Input
  std::vector<uint8_t> sk_seed(n, 0);
  std::vector<uint8_t> pk_seed(n, 0);
  sphincs_plus_adrs::fors_tree_t adrs{};
  std::vector<uint8_t> msg(msg_len, 0);

  auto _sk_seed = std::span<uint8_t, n>(sk_seed);
  auto _pk_seed = std::span<uint8_t, n>(pk_seed);
  auto _msg = std::span<uint8_t, msg_len>(msg);

  // Output
  std::vector<uint8_t> pkey0(n, 0);
  std::vector<uint8_t> pkey1(n, 0);
  std::vector<uint8_t> sig(sig_len, 0);

  auto _pkey0 = std::span<uint8_t, n>(pkey0);
  auto _pkey1 = std::span<uint8_t, n>(pkey1);
  auto _sig = std::span<uint8_t, sig_len>(sig);

  prng::prng_t prng;
  prng.read(_sk_seed);
  prng.read(_pk_seed);
  prng.read(_msg);

  sphincs_plus_fors::pkgen<n, a, k, v>(_sk_seed, _pk_seed, adrs, _pkey0);
  sphincs_plus_fors::sign<n, a, k, v>(_msg, _sk_seed, _pk_seed, adrs, _sig);
  sphincs_plus_fors::pk_from_sig<n, a, k, v>(_sig, _msg, _pk_seed, adrs, _pkey1);

  EXPECT_EQ(pkey0, pkey1);
}

TEST(SphincsPlus, FORSNISTSecurityLevel1)
{
  test_fors<16, 12, 14, sphincs_plus_hashing::variant::robust>();
  test_fors<16, 12, 14, sphincs_plus_hashing::variant::simple>();
  test_fors<16, 6, 33, sphincs_plus_hashing::variant::robust>();
  test_fors<16, 6, 33, sphincs_plus_hashing::variant::simple>();
}

TEST(SphincsPlus, FORSNISTSecurityLevel3)
{
  test_fors<24, 14, 17, sphincs_plus_hashing::variant::robust>();
  test_fors<24, 14, 17, sphincs_plus_hashing::variant::simple>();
  test_fors<24, 8, 33, sphincs_plus_hashing::variant::robust>();
  test_fors<24, 8, 33, sphincs_plus_hashing::variant::simple>();
}

TEST(SphincsPlus, FORSNISTSecurityLevel5)
{
  test_fors<32, 14, 22, sphincs_plus_hashing::variant::robust>();
  test_fors<32, 14, 22, sphincs_plus_hashing::variant::simple>();
  test_fors<32, 9, 35, sphincs_plus_hashing::variant::robust>();
  test_fors<32, 9, 35, sphincs_plus_hashing::variant::simple>();
}
