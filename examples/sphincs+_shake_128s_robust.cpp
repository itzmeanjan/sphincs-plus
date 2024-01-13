#include "prng.hpp"
#include "sphincs+_128s_robust.hpp"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

// Given a bytearray of length N, this function converts it to human readable
// hex string of length N << 1 | N >= 0
static inline const std::string
to_hex(std::span<const uint8_t> bytes)
{
  std::stringstream ss;
  ss << std::hex;

  for (size_t i = 0; i < bytes.size(); i++) {
    ss << std::setw(2) << std::setfill('0') << static_cast<uint32_t>(bytes[i]);
  }

  return ss.str();
}

// Compile it with
//
// g++ -std=c++20 -Wall -Wextra -pedantic -O3 -march=native -I include -I sha3/include sphincs+_shake_128s_robust.cpp
int
main()
{
  // Byte length of message to be signed is fixed to 32 -bytes, in this example.
  constexpr size_t mlen = 32;

  std::vector<uint8_t> sk_seed(sphincs_plus_128s_robust::n, 0);
  std::vector<uint8_t> sk_prf(sphincs_plus_128s_robust::n, 0);
  std::vector<uint8_t> pk_seed(sphincs_plus_128s_robust::n, 0);
  std::vector<uint8_t> pkey(sphincs_plus_128s_robust::PubKeyLen, 0);
  std::vector<uint8_t> skey(sphincs_plus_128s_robust::SecKeyLen, 0);
  std::vector<uint8_t> msg(mlen, 0);
  std::vector<uint8_t> sig(sphincs_plus_128s_robust::SigLen, 0);

  auto _sk_seed = std::span<uint8_t, sphincs_plus_128s_robust::n>(sk_seed);
  auto _sk_prf = std::span<uint8_t, sphincs_plus_128s_robust::n>(sk_prf);
  auto _pk_seed = std::span<uint8_t, sphincs_plus_128s_robust::n>(pk_seed);
  auto _pkey = std::span<uint8_t, sphincs_plus_128s_robust::PubKeyLen>(pkey);
  auto _skey = std::span<uint8_t, sphincs_plus_128s_robust::SecKeyLen>(skey);
  auto _msg = std::span(msg);
  auto _sig = std::span<uint8_t, sphincs_plus_128s_robust::SigLen>(sig);

  prng::prng_t prng;

  prng.read(_sk_seed);
  prng.read(_sk_prf);
  prng.read(_pk_seed);
  prng.read(_msg);

  // step 1: generate random keypair
  sphincs_plus_128s_robust::keygen(_sk_seed, _sk_prf, _pk_seed, _skey, _pkey);
  // step 2: sign N(>0) -bytes message, using secret key
  //
  // In case of randomized message signing, `rand_bytes` must be pointing to n -bytes random seed, otherwise it can safely be passed as empty span i.e. {}.
  sphincs_plus_128s_robust::sign(_msg, _skey, {}, _sig);
  // step 3: verify signature, using public key
  const bool flag = sphincs_plus_128s_robust::verify(_msg, _sig, _pkey);

  std::cout << "SPHINCS+-SHAKE-128s-robust @ NIST Security Level 1\n";
  std::cout << "Secret Key   : " << to_hex(_skey) << "\n";
  std::cout << "Public Key   : " << to_hex(_pkey) << "\n";
  std::cout << "Message      : " << to_hex(_msg) << "\n";
  std::cout << "Signature    : " << to_hex(_sig) << "\n";
  std::cout << "Verified      : " << std::boolalpha << flag << "\n";

  // ensure that SPHINCS+ keygen -> sign -> verify works as expected !
  assert(flag);

  return EXIT_SUCCESS;
}
