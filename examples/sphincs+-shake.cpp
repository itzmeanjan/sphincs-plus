#include "sphincs.hpp"
#include <iostream>

// Compile it with
//
// g++ -std=c++20 -Wall -O3 -I include -I sha3/include sphincs+-shake.cpp
int
main()
{
  // SPHINCS+-SHAKE-128s-robust Parameter Set
  //
  // Find all recommended parameters in table 3 of SPHINCS+ specification
  // https://sphincs.org/data/sphincs+-r3.1-specification.pdf
  constexpr size_t n = 16;
  constexpr uint32_t h = 63;
  constexpr uint32_t d = 7;
  constexpr uint32_t a = 12;
  constexpr uint32_t k = 14;
  constexpr size_t w = 16;
  constexpr auto v = sphincs_hashing::variant::robust;

  // Compile-time compute length of public key, secret key and signature, based
  // on which how much memory to allocate for each of them, can be decided
  namespace utils = sphincs_utils;

  constexpr size_t pklen = utils::get_sphincs_pkey_len<n>();
  constexpr size_t sklen = utils::get_sphincs_skey_len<n>();
  constexpr size_t siglen = utils::get_sphincs_sig_len<n, h, d, a, k, w>();
  constexpr size_t mlen = 32;

  // Allocate memory for public key, secret key, message ( fixed to 32 -bytes in
  // this example ) and signature
  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(pklen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sklen));
  uint8_t* msg = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* sig = static_cast<uint8_t*>(std::malloc(siglen));

  // Generate random message bytes
  sphincs_utils::random_data<uint8_t>(msg, mlen);

  // step 1: generate random keypair
  sphincs::keygen<n, h, d, w, v>(skey, pkey);
  // step 2: sign 32 -bytes message, using secret key
  sphincs::sign<n, h, d, a, k, w, v>(msg, mlen, skey, sig);
  // step 3: verify signature, using public key
  const bool flg = sphincs::verify<n, h, d, a, k, w, v>(msg, mlen, sig, pkey);

  std::cout << "SPHINCS+-SHAKE-128s-robust @ NIST Security Level 1\n";
  std::cout << "Secret Key   : " << sphincs_utils::to_hex(skey, sklen) << "\n";
  std::cout << "Public Key   : " << sphincs_utils::to_hex(pkey, pklen) << "\n";
  std::cout << "Message      : " << sphincs_utils::to_hex(msg, mlen) << "\n";
  std::cout << "Signature    : " << sphincs_utils::to_hex(sig, siglen) << "\n";
  std::cout << "Verified      : " << std::boolalpha << flg << "\n";

  // release memory resources
  std::free(pkey);
  std::free(skey);
  std::free(msg);
  std::free(sig);

  // ensure that SPHINCS+ keygen -> sign -> verify works as expected !
  assert(flg);

  return EXIT_SUCCESS;
}
