#include "prng.hpp"
#include "sphincs+.hpp"
#include <gtest/gtest.h>
#include <vector>

// Test correctness of SPHINCS+ implementation, using
//
// - Keypair generation
// - Signing message using private key
// - Verifying signature using public key and message
//
// with random data.
template<size_t n, uint32_t h, uint32_t d, uint32_t a, uint32_t k, size_t w, sphincs_plus_hashing::variant v, bool randomize = false>
static inline void
test_sphincs_plus(const size_t mlen)
{
  namespace utils = sphincs_plus_utils;
  constexpr size_t pklen = utils::get_sphincs_pkey_len<n>();
  constexpr size_t sklen = utils::get_sphincs_skey_len<n>();
  constexpr size_t siglen = utils::get_sphincs_sig_len<n, h, d, a, k, w>();

  std::vector<uint8_t> sk_seed(n, 0);
  std::vector<uint8_t> sk_prf(n, 0);
  std::vector<uint8_t> pk_seed(n, 0);
  std::vector<uint8_t> pkey(pklen, 0);
  std::vector<uint8_t> skey(sklen, 0);
  std::vector<uint8_t> msg(mlen, 0);
  std::vector<uint8_t> rand_bytes(n, 0);
  std::vector<uint8_t> sig(siglen, 0);

  auto _sk_seed = std::span<uint8_t, n>(sk_seed);
  auto _sk_prf = std::span<uint8_t, n>(sk_prf);
  auto _pk_seed = std::span<uint8_t, n>(pk_seed);
  auto _pkey = std::span<uint8_t, pklen>(pkey);
  auto _skey = std::span<uint8_t, sklen>(skey);
  auto _msg = std::span<uint8_t>(msg);
  auto _rand_bytes = std::span<uint8_t, n>(rand_bytes);
  auto _sig = std::span<uint8_t, siglen>(sig);

  prng::prng_t prng;
  prng.read(_sk_seed);
  prng.read(_sk_prf);
  prng.read(_pk_seed);
  prng.read(_msg);
  prng.read(_rand_bytes);

  sphincs_plus::keygen<n, h, d, w, v>(_sk_seed, _sk_prf, _pk_seed, _skey, _pkey);
  if constexpr (randomize) {
    sphincs_plus::sign<n, h, d, a, k, w, v, randomize>(_msg, _skey, _rand_bytes, _sig);
  } else {
    sphincs_plus::sign<n, h, d, a, k, w, v, randomize>(_msg, _skey, {}, _sig);
  }
  const bool flag = sphincs_plus::verify<n, h, d, a, k, w, v>(_msg, _sig, _pkey);

  EXPECT_TRUE(flag);
}

TEST(SphincsPlus, SphincsPlusNISTSecurityLevel1KeygenSignVerify)
{
  test_sphincs_plus<16, 63, 7, 12, 14, 16, sphincs_plus_hashing::variant::robust>(32);
  test_sphincs_plus<16, 63, 7, 12, 14, 16, sphincs_plus_hashing::variant::robust, true>(32);
  test_sphincs_plus<16, 63, 7, 12, 14, 16, sphincs_plus_hashing::variant::simple>(32);
  test_sphincs_plus<16, 63, 7, 12, 14, 16, sphincs_plus_hashing::variant::simple, true>(32);

  test_sphincs_plus<16, 66, 22, 6, 33, 16, sphincs_plus_hashing::variant::robust>(32);
  test_sphincs_plus<16, 66, 22, 6, 33, 16, sphincs_plus_hashing::variant::robust, true>(32);
  test_sphincs_plus<16, 66, 22, 6, 33, 16, sphincs_plus_hashing::variant::simple>(32);
  test_sphincs_plus<16, 66, 22, 6, 33, 16, sphincs_plus_hashing::variant::simple, true>(32);
}

TEST(SphincsPlus, SphincsPlusNISTSecurityLevel3KeygenSignVerify)
{
  test_sphincs_plus<24, 63, 7, 14, 17, 16, sphincs_plus_hashing::variant::robust>(32);
  test_sphincs_plus<24, 63, 7, 14, 17, 16, sphincs_plus_hashing::variant::robust, true>(32);
  test_sphincs_plus<24, 63, 7, 14, 17, 16, sphincs_plus_hashing::variant::simple>(32);
  test_sphincs_plus<24, 63, 7, 14, 17, 16, sphincs_plus_hashing::variant::simple, true>(32);

  test_sphincs_plus<24, 66, 22, 8, 33, 16, sphincs_plus_hashing::variant::robust>(32);
  test_sphincs_plus<24, 66, 22, 8, 33, 16, sphincs_plus_hashing::variant::robust, true>(32);
  test_sphincs_plus<24, 66, 22, 8, 33, 16, sphincs_plus_hashing::variant::simple>(32);
  test_sphincs_plus<24, 66, 22, 8, 33, 16, sphincs_plus_hashing::variant::simple, true>(32);
}

TEST(SphincsPlus, SphincsPlusNISTSecurityLevel5KeygenSignVerify)
{
  test_sphincs_plus<32, 64, 8, 14, 22, 16, sphincs_plus_hashing::variant::robust>(32);
  test_sphincs_plus<32, 64, 8, 14, 22, 16, sphincs_plus_hashing::variant::robust, true>(32);
  test_sphincs_plus<32, 64, 8, 14, 22, 16, sphincs_plus_hashing::variant::simple>(32);
  test_sphincs_plus<32, 64, 8, 14, 22, 16, sphincs_plus_hashing::variant::simple, true>(32);

  test_sphincs_plus<32, 68, 17, 9, 35, 16, sphincs_plus_hashing::variant::robust>(32);
  test_sphincs_plus<32, 68, 17, 9, 35, 16, sphincs_plus_hashing::variant::robust, true>(32);
  test_sphincs_plus<32, 68, 17, 9, 35, 16, sphincs_plus_hashing::variant::simple>(32);
  test_sphincs_plus<32, 68, 17, 9, 35, 16, sphincs_plus_hashing::variant::simple, true>(32);
}
