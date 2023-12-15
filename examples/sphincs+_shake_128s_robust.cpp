#include "sphincs+_128s_robust.hpp"
#include <iostream>

// Compile it with
//
// g++ -std=c++20 -Wall -O3 -march=native -I include -I sha3/include sphincs+_shake_128s_robust.cpp
int
main()
{
  constexpr size_t mlen = 32;

  // Allocate memory for seeds, public key, secret key, message ( fixed to 32 -bytes in this example ) and signature
  uint8_t* sk_seed = static_cast<uint8_t*>(std::malloc(sphincs_plus_128s_robust::n));
  uint8_t* sk_prf = static_cast<uint8_t*>(std::malloc(sphincs_plus_128s_robust::n));
  uint8_t* pk_seed = static_cast<uint8_t*>(std::malloc(sphincs_plus_128s_robust::n));
  uint8_t* pkey = static_cast<uint8_t*>(std::malloc(sphincs_plus_128s_robust::PubKeyLen));
  uint8_t* skey = static_cast<uint8_t*>(std::malloc(sphincs_plus_128s_robust::SecKeyLen));
  uint8_t* msg = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* sig = static_cast<uint8_t*>(std::malloc(sphincs_plus_128s_robust::SigLen));

  sphincs_plus_utils::random_data<uint8_t>(sk_seed, sphincs_plus_128s_robust::n);
  sphincs_plus_utils::random_data<uint8_t>(sk_prf, sphincs_plus_128s_robust::n);
  sphincs_plus_utils::random_data<uint8_t>(pk_seed, sphincs_plus_128s_robust::n);
  sphincs_plus_utils::random_data<uint8_t>(msg, mlen);

  // step 1: generate random keypair
  sphincs_plus_128s_robust::keygen(sk_seed, sk_prf, pk_seed, skey, pkey);
  // step 2: sign N(>0) -bytes message, using secret key
  //
  // In case of randomized message signing, `rand_bytes` must be pointing to n -bytes random seed, otherwise it can safely be passed as nullptr.
  sphincs_plus_128s_robust::sign(msg, mlen, skey, nullptr, sig);
  // step 3: verify signature, using public key
  const bool flag = sphincs_plus_128s_robust::verify(msg, mlen, sig, pkey);

  std::cout << "SPHINCS+-SHAKE-128s-robust @ NIST Security Level 1\n";
  std::cout << "Secret Key   : " << sphincs_plus_utils::to_hex(skey, sphincs_plus_128s_robust::SecKeyLen) << "\n";
  std::cout << "Public Key   : " << sphincs_plus_utils::to_hex(pkey, sphincs_plus_128s_robust::PubKeyLen) << "\n";
  std::cout << "Message      : " << sphincs_plus_utils::to_hex(msg, mlen) << "\n";
  std::cout << "Signature    : " << sphincs_plus_utils::to_hex(sig, sphincs_plus_128s_robust::SigLen) << "\n";
  std::cout << "Verified      : " << std::boolalpha << flag << "\n";

  // release memory resources
  std::free(sk_seed);
  std::free(sk_prf);
  std::free(pk_seed);
  std::free(pkey);
  std::free(skey);
  std::free(msg);
  std::free(sig);

  // ensure that SPHINCS+ keygen -> sign -> verify works as expected !
  assert(flag);

  return EXIT_SUCCESS;
}
