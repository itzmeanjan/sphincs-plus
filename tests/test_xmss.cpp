#include "prng.hpp"
#include "xmss.hpp"
#include <cstdint>
#include <gtest/gtest.h>
#include <vector>

// Test correctness of fixed input-length XMSS implementation, in standalone
// mode, using
//
// - Public key generation
// - Signing using private key
// - Verifying signature by recovering public key from message and signature
//
// with random data.
template<uint32_t h, size_t n, size_t w, sphincs_plus_hashing::variant v>
static inline void
test_xmss()
{
  constexpr size_t len = sphincs_plus_utils::compute_wots_len<n, w>();
  constexpr size_t sig_len = len * n + h * n; // XMSS signature in bytes
  constexpr uint32_t idx = 0u;

  // Input
  std::vector<uint8_t> sk_seed(n, 0);
  std::vector<uint8_t> pk_seed(n, 0);
  sphincs_plus_adrs::adrs_t adrs{};
  std::vector<uint8_t> msg(n, 0);

  auto _sk_seed = std::span<uint8_t, n>(sk_seed);
  auto _pk_seed = std::span<uint8_t, n>(pk_seed);
  auto _msg = std::span<uint8_t, n>(msg);

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
  prng.read(adrs.data);
  prng.read(_msg);

  sphincs_plus_xmss::pkgen<h, n, w, v>(_sk_seed, _pk_seed, adrs, _pkey0);
  sphincs_plus_xmss::sign<h, n, w, v>(_msg, _sk_seed, idx, _pk_seed, adrs, _sig);
  sphincs_plus_xmss::pk_from_sig<h, n, w, v>(idx, _sig, _msg, _pk_seed, adrs, _pkey1);

  EXPECT_EQ(pkey0, pkey1);
}

TEST(SphincsPlus, XMSSNISTSecurityLevel1)
{
  test_xmss<63 / 7, 16, 16, sphincs_plus_hashing::variant::robust>();
  test_xmss<63 / 7, 16, 16, sphincs_plus_hashing::variant::simple>();
  test_xmss<66 / 22, 16, 16, sphincs_plus_hashing::variant::robust>();
  test_xmss<66 / 22, 16, 16, sphincs_plus_hashing::variant::simple>();
}

TEST(SphincsPlus, XMSSNISTSecurityLevel3)
{
  test_xmss<63 / 7, 24, 16, sphincs_plus_hashing::variant::robust>();
  test_xmss<63 / 7, 24, 16, sphincs_plus_hashing::variant::simple>();
  test_xmss<66 / 22, 24, 16, sphincs_plus_hashing::variant::robust>();
  test_xmss<66 / 22, 24, 16, sphincs_plus_hashing::variant::simple>();
}

TEST(SphincsPlus, XMSSNISTSecurityLevel5)
{
  test_xmss<64 / 8, 32, 16, sphincs_plus_hashing::variant::robust>();
  test_xmss<64 / 8, 32, 16, sphincs_plus_hashing::variant::simple>();
  test_xmss<68 / 17, 32, 16, sphincs_plus_hashing::variant::robust>();
  test_xmss<68 / 17, 32, 16, sphincs_plus_hashing::variant::simple>();
}
