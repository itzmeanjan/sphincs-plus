#include "sphincs.hpp"
#include <gtest/gtest.h>

// Test correctness of SPHINCS+ implementation, using
//
// - Keypair generation
// - Signing message using private key
// - Verifying signature using public key and message
//
// with random data.
template<size_t n, uint32_t h, uint32_t d, uint32_t a, uint32_t k, size_t w, sphincs_hashing::variant v, bool randomize = false>
inline static void
test_sphincs_plus(const size_t mlen)
{
  namespace utils = sphincs_utils;
  constexpr size_t pklen = utils::get_sphincs_pkey_len<n>();
  constexpr size_t sklen = utils::get_sphincs_skey_len<n>();
  constexpr size_t siglen = utils::get_sphincs_sig_len<n, h, d, a, k, w>();

  // acquire memory resources
  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));
  uint8_t* msg = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* sig = static_cast<uint8_t*>(std::malloc(siglen));

  sphincs_utils::random_data<uint8_t>(msg, mlen);

  sphincs::keygen<n, h, d, w, v>(skey, pkey);
  sphincs::sign<n, h, d, a, k, w, v, randomize>(msg, mlen, skey, sig);
  const bool flag = sphincs::verify<n, h, d, a, k, w, v>(msg, mlen, sig, pkey);

  // release memory resources
  std::free(pkey);
  std::free(skey);
  std::free(msg);
  std::free(sig);

  EXPECT_TRUE(flag);
}

TEST(SphincsPlus, SphincsPlusNISTSecurityLevel1KeygenSignVerify)
{
  test_sphincs_plus<16, 63, 7, 12, 14, 16, sphincs_hashing::variant::robust>(32);
  test_sphincs_plus<16, 63, 7, 12, 14, 16, sphincs_hashing::variant::robust, true>(32);
  test_sphincs_plus<16, 63, 7, 12, 14, 16, sphincs_hashing::variant::simple>(32);
  test_sphincs_plus<16, 63, 7, 12, 14, 16, sphincs_hashing::variant::simple, true>(32);

  test_sphincs_plus<16, 66, 22, 6, 33, 16, sphincs_hashing::variant::robust>(32);
  test_sphincs_plus<16, 66, 22, 6, 33, 16, sphincs_hashing::variant::robust, true>(32);
  test_sphincs_plus<16, 66, 22, 6, 33, 16, sphincs_hashing::variant::simple>(32);
  test_sphincs_plus<16, 66, 22, 6, 33, 16, sphincs_hashing::variant::simple, true>(32);
}

TEST(SphincsPlus, SphincsPlusNISTSecurityLevel3KeygenSignVerify)
{
  test_sphincs_plus<24, 63, 7, 14, 17, 16, sphincs_hashing::variant::robust>(32);
  test_sphincs_plus<24, 63, 7, 14, 17, 16, sphincs_hashing::variant::robust, true>(32);
  test_sphincs_plus<24, 63, 7, 14, 17, 16, sphincs_hashing::variant::simple>(32);
  test_sphincs_plus<24, 63, 7, 14, 17, 16, sphincs_hashing::variant::simple, true>(32);

  test_sphincs_plus<24, 66, 22, 8, 33, 16, sphincs_hashing::variant::robust>(32);
  test_sphincs_plus<24, 66, 22, 8, 33, 16, sphincs_hashing::variant::robust, true>(32);
  test_sphincs_plus<24, 66, 22, 8, 33, 16, sphincs_hashing::variant::simple>(32);
  test_sphincs_plus<24, 66, 22, 8, 33, 16, sphincs_hashing::variant::simple, true>(32);
}

TEST(SphincsPlus, SphincsPlusNISTSecurityLevel5KeygenSignVerify)
{
  test_sphincs_plus<32, 64, 8, 14, 22, 16, sphincs_hashing::variant::robust>(32);
  test_sphincs_plus<32, 64, 8, 14, 22, 16, sphincs_hashing::variant::robust, true>(32);
  test_sphincs_plus<32, 64, 8, 14, 22, 16, sphincs_hashing::variant::simple>(32);
  test_sphincs_plus<32, 64, 8, 14, 22, 16, sphincs_hashing::variant::simple, true>(32);

  test_sphincs_plus<32, 68, 17, 9, 35, 16, sphincs_hashing::variant::robust>(32);
  test_sphincs_plus<32, 68, 17, 9, 35, 16, sphincs_hashing::variant::robust, true>(32);
  test_sphincs_plus<32, 68, 17, 9, 35, 16, sphincs_hashing::variant::simple>(32);
  test_sphincs_plus<32, 68, 17, 9, 35, 16, sphincs_hashing::variant::simple, true>(32);
}
