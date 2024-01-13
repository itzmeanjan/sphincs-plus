#include "sphincs+.hpp"
#include "utils.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <gtest/gtest.h>
#include <string>
#include <string_view>
#include <vector>

// Test functional correctness and conformance ( to the reference implementation based on specification document
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf ) of SPHINCS+ implementation, using Known Answer Tests,
// generated following instructions described in https://gist.github.com/itzmeanjan/d483872509b8a1a7c4d6614ec9d43e6c.
template<size_t n, uint32_t h, uint32_t d, uint32_t a, uint32_t k, size_t w, sphincs_plus_hashing::variant v>
static inline void
test_sphincs_plus_kat(const std::string kat_file)
{
  using namespace std::literals;

  std::fstream file(kat_file);

  while (true) {
    std::string sk_seed0;

    if (!std::getline(file, sk_seed0).eof()) {
      auto sk_seed1 = std::string_view(sk_seed0);
      auto sk_seed2 = sk_seed1.substr(sk_seed1.find("="sv) + 2, sk_seed1.size());
      auto sk_seed = sphincs_plus_utils::from_hex(sk_seed2);
      auto _sk_seed = std::span<uint8_t, n>(sk_seed);

      std::string sk_prf0;
      std::getline(file, sk_prf0);

      auto sk_prf1 = std::string_view(sk_prf0);
      auto sk_prf2 = sk_prf1.substr(sk_prf1.find("="sv) + 2, sk_prf1.size());
      auto sk_prf = sphincs_plus_utils::from_hex(sk_prf2);
      auto _sk_prf = std::span<uint8_t, n>(sk_prf);

      std::string pk_seed0;
      std::getline(file, pk_seed0);

      auto pk_seed1 = std::string_view(pk_seed0);
      auto pk_seed2 = pk_seed1.substr(pk_seed1.find("="sv) + 2, pk_seed1.size());
      auto pk_seed = sphincs_plus_utils::from_hex(pk_seed2);
      auto _pk_seed = std::span<uint8_t, n>(pk_seed);

      std::string pk_root0;
      std::getline(file, pk_root0);

      auto pk_root1 = std::string_view(pk_root0);
      auto pk_root2 = pk_root1.substr(pk_root1.find("="sv) + 2, pk_root1.size());
      auto pk_root = sphincs_plus_utils::from_hex(pk_root2);

      std::string mlen0;
      std::getline(file, mlen0);
      size_t mlen = 0;

      auto mlen1 = std::string_view(mlen0);
      auto mlen2 = mlen1.substr(mlen1.find("="sv) + 2, mlen1.size());
      std::from_chars(mlen2.data(), mlen2.data() + mlen2.size(), mlen);

      std::string msg0;
      std::getline(file, msg0);

      auto msg1 = std::string_view(msg0);
      auto msg2 = msg1.substr(msg1.find("="sv) + 2, msg1.size());
      auto msg = sphincs_plus_utils::from_hex(msg2);
      auto _msg = std::span<uint8_t>(msg);

      std::string opt0;
      std::getline(file, opt0);

      auto opt1 = std::string_view(opt0);
      auto opt2 = opt1.substr(opt1.find("="sv) + 2, opt1.size());
      auto opt = sphincs_plus_utils::from_hex(opt2);
      auto _opt = std::span<uint8_t, n>(opt);

      std::string sig0;
      std::getline(file, sig0);

      auto sig1 = std::string_view(sig0);
      auto sig2 = sig1.substr(sig1.find("="sv) + 2, sig1.size());
      auto sig = sphincs_plus_utils::from_hex(sig2);

      constexpr size_t expected_pklen = sphincs_plus_utils::get_sphincs_pkey_len<n>();
      constexpr size_t expected_sklen = sphincs_plus_utils::get_sphincs_skey_len<n>();
      constexpr size_t expected_siglen = sphincs_plus_utils::get_sphincs_sig_len<n, h, d, a, k, w>();

      const size_t computed_pklen = pk_seed.size() + pk_root.size();
      const size_t computed_sklen = sk_seed.size() + sk_prf.size() + computed_pklen;
      const size_t computed_siglen = sig.size();

      EXPECT_EQ(expected_pklen, computed_pklen);
      EXPECT_EQ(expected_sklen, computed_sklen);
      EXPECT_EQ(expected_siglen, computed_siglen);
      EXPECT_EQ(msg.size(), mlen);

      std::vector<uint8_t> pkey(computed_pklen, 0);
      std::vector<uint8_t> skey(computed_sklen, 0);
      std::vector<uint8_t> computed_sig(computed_siglen, 0);

      auto _pkey = std::span<uint8_t, expected_pklen>(pkey);
      auto _skey = std::span<uint8_t, expected_sklen>(skey);
      auto _computed_sig = std::span<uint8_t, expected_siglen>(computed_sig);

      // Keygen -> (randomized) Sign -> Verify
      sphincs_plus::keygen<n, h, d, w, v>(_sk_seed, _sk_prf, _pk_seed, _skey, _pkey);
      sphincs_plus::sign<n, h, d, a, k, w, v, true>(_msg, _skey, _opt, _computed_sig);
      const auto flag = sphincs_plus::verify<n, h, d, a, k, w, v>(_msg, _computed_sig, _pkey);

      // Check if computed public key, secret key and signature matches expected ones, from KAT file.
      EXPECT_EQ(std::memcmp(pk_seed.data(), pkey.data(), pk_seed.size()), 0);
      EXPECT_EQ(std::memcmp(pk_root.data(), pkey.data() + pk_seed.size(), pk_root.size()), 0);
      EXPECT_EQ(std::memcmp(sk_seed.data(), skey.data(), sk_seed.size()), 0);
      EXPECT_EQ(std::memcmp(sk_prf.data(), skey.data() + sk_seed.size(), sk_prf.size()), 0);
      EXPECT_EQ(std::memcmp(pkey.data(), skey.data() + sk_seed.size() + sk_prf.size(), computed_pklen), 0);
      EXPECT_EQ(sig, computed_sig);
      EXPECT_TRUE(flag);

      std::string empty_line;
      std::getline(file, empty_line);
    } else {
      break;
    }
  }

  file.close();
}

TEST(SphincsPlus, SphincsPlus128sRobustKnownAnswerTests)
{
  test_sphincs_plus_kat<16, 63, 7, 12, 14, 16, sphincs_plus_hashing::variant::robust>("./kats/sphincs-shake-128s-robust.kat");
}

TEST(SphincsPlus, SphincsPlus128sSimpleKnownAnswerTests)
{
  test_sphincs_plus_kat<16, 63, 7, 12, 14, 16, sphincs_plus_hashing::variant::simple>("./kats/sphincs-shake-128s-simple.kat");
}

TEST(SphincsPlus, SphincsPlus128fRobustKnownAnswerTests)
{
  test_sphincs_plus_kat<16, 66, 22, 6, 33, 16, sphincs_plus_hashing::variant::robust>("./kats/sphincs-shake-128f-robust.kat");
}

TEST(SphincsPlus, SphincsPlus128fSimpleKnownAnswerTests)
{
  test_sphincs_plus_kat<16, 66, 22, 6, 33, 16, sphincs_plus_hashing::variant::simple>("./kats/sphincs-shake-128f-simple.kat");
}

TEST(SphincsPlus, SphincsPlus192sRobustKnownAnswerTests)
{
  test_sphincs_plus_kat<24, 63, 7, 14, 17, 16, sphincs_plus_hashing::variant::robust>("./kats/sphincs-shake-192s-robust.kat");
}

TEST(SphincsPlus, SphincsPlus192sSimpleKnownAnswerTests)
{
  test_sphincs_plus_kat<24, 63, 7, 14, 17, 16, sphincs_plus_hashing::variant::simple>("./kats/sphincs-shake-192s-simple.kat");
}

TEST(SphincsPlus, SphincsPlus192fRobustKnownAnswerTests)
{
  test_sphincs_plus_kat<24, 66, 22, 8, 33, 16, sphincs_plus_hashing::variant::robust>("./kats/sphincs-shake-192f-robust.kat");
}

TEST(SphincsPlus, SphincsPlus192fSimpleKnownAnswerTests)
{
  test_sphincs_plus_kat<24, 66, 22, 8, 33, 16, sphincs_plus_hashing::variant::simple>("./kats/sphincs-shake-192f-simple.kat");
}

TEST(SphincsPlus, SphincsPlus256sRobustKnownAnswerTests)
{
  test_sphincs_plus_kat<32, 64, 8, 14, 22, 16, sphincs_plus_hashing::variant::robust>("./kats/sphincs-shake-256s-robust.kat");
}

TEST(SphincsPlus, SphincsPlus256sSimpleKnownAnswerTests)
{
  test_sphincs_plus_kat<32, 64, 8, 14, 22, 16, sphincs_plus_hashing::variant::simple>("./kats/sphincs-shake-256s-simple.kat");
}

TEST(SphincsPlus, SphincsPlus256fRobustKnownAnswerTests)
{
  test_sphincs_plus_kat<32, 68, 17, 9, 35, 16, sphincs_plus_hashing::variant::robust>("./kats/sphincs-shake-256f-robust.kat");
}

TEST(SphincsPlus, SphincsPlus256fSimpleKnownAnswerTests)
{
  test_sphincs_plus_kat<32, 68, 17, 9, 35, 16, sphincs_plus_hashing::variant::simple>("./kats/sphincs-shake-256f-simple.kat");
}
