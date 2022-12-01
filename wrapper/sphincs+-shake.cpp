#include "sphincs_inner.hpp"

// Thin C wrapper on top of underlying C++ implementation of
// SPHINCS+-SHAKE-{128,192,256}{s,f}-{robust,simple} digital signature algorithm
// (DSA), which can be used for producing shared library object with conformant
// C-ABI & interfaced from other languages such as Rust, Python etc.
//
// This interface is being exposed for testing this implementation's functional
// correctness and conformance with SPHINCS+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
// using known answer tests produced using
// https://gist.github.com/itzmeanjan/d483872509b8a1a7c4d6614ec9d43e6c

// Function prototypes
extern "C"
{
  // Key Generation API

  // Generates keypair for SPHINCS+-SHAKE-128s-robust parameter set, as provided
  // in table 3 of the specification
  // https://sphincs.org/data/sphincs+-r3.1-specification.pdf
  void sphincs_shake_128s_robust_keygen(
    const uint8_t* const __restrict, // 16 -bytes secret key seed
    const uint8_t* const __restrict, // 16 -bytes secret key PRF
    const uint8_t* const __restrict, // 16 -bytes public key seed
    uint8_t* const __restrict,       // 64 -bytes SPHINCS+ secret key
    uint8_t* const __restrict        // 32 -bytes SPHINCS+ public key
  );

  // Generates keypair for SPHINCS+-SHAKE-128s-simple parameter set, as provided
  // in table 3 of the specification
  // https://sphincs.org/data/sphincs+-r3.1-specification.pdf
  void sphincs_shake_128s_simple_keygen(
    const uint8_t* const __restrict, // 16 -bytes secret key seed
    const uint8_t* const __restrict, // 16 -bytes secret key PRF
    const uint8_t* const __restrict, // 16 -bytes public key seed
    uint8_t* const __restrict,       // 64 -bytes SPHINCS+ secret key
    uint8_t* const __restrict        // 32 -bytes SPHINCS+ public key
  );

  // Generates keypair for SPHINCS+-SHAKE-128f-robust parameter set, as provided
  // in table 3 of the specification
  // https://sphincs.org/data/sphincs+-r3.1-specification.pdf
  void sphincs_shake_128f_robust_keygen(
    const uint8_t* const __restrict, // 16 -bytes secret key seed
    const uint8_t* const __restrict, // 16 -bytes secret key PRF
    const uint8_t* const __restrict, // 16 -bytes public key seed
    uint8_t* const __restrict,       // 64 -bytes SPHINCS+ secret key
    uint8_t* const __restrict        // 32 -bytes SPHINCS+ public key
  );

  // Generates keypair for SPHINCS+-SHAKE-128f-simple parameter set, as provided
  // in table 3 of the specification
  // https://sphincs.org/data/sphincs+-r3.1-specification.pdf
  void sphincs_shake_128f_simple_keygen(
    const uint8_t* const __restrict, // 16 -bytes secret key seed
    const uint8_t* const __restrict, // 16 -bytes secret key PRF
    const uint8_t* const __restrict, // 16 -bytes public key seed
    uint8_t* const __restrict,       // 64 -bytes SPHINCS+ secret key
    uint8_t* const __restrict        // 32 -bytes SPHINCS+ public key
  );

  // Generates keypair for SPHINCS+-SHAKE-192s-robust parameter set, as provided
  // in table 3 of the specification
  // https://sphincs.org/data/sphincs+-r3.1-specification.pdf
  void sphincs_shake_192s_robust_keygen(
    const uint8_t* const __restrict, // 24 -bytes secret key seed
    const uint8_t* const __restrict, // 24 -bytes secret key PRF
    const uint8_t* const __restrict, // 24 -bytes public key seed
    uint8_t* const __restrict,       // 96 -bytes SPHINCS+ secret key
    uint8_t* const __restrict        // 48 -bytes SPHINCS+ public key
  );

  // Generates keypair for SPHINCS+-SHAKE-192s-simple parameter set, as provided
  // in table 3 of the specification
  // https://sphincs.org/data/sphincs+-r3.1-specification.pdf
  void sphincs_shake_192s_simple_keygen(
    const uint8_t* const __restrict, // 24 -bytes secret key seed
    const uint8_t* const __restrict, // 24 -bytes secret key PRF
    const uint8_t* const __restrict, // 24 -bytes public key seed
    uint8_t* const __restrict,       // 96 -bytes SPHINCS+ secret key
    uint8_t* const __restrict        // 48 -bytes SPHINCS+ public key
  );

  // Generates keypair for SPHINCS+-SHAKE-192f-robust parameter set, as provided
  // in table 3 of the specification
  // https://sphincs.org/data/sphincs+-r3.1-specification.pdf
  void sphincs_shake_192f_robust_keygen(
    const uint8_t* const __restrict, // 24 -bytes secret key seed
    const uint8_t* const __restrict, // 24 -bytes secret key PRF
    const uint8_t* const __restrict, // 24 -bytes public key seed
    uint8_t* const __restrict,       // 96 -bytes SPHINCS+ secret key
    uint8_t* const __restrict        // 48 -bytes SPHINCS+ public key
  );

  // Generates keypair for SPHINCS+-SHAKE-192f-simple parameter set, as provided
  // in table 3 of the specification
  // https://sphincs.org/data/sphincs+-r3.1-specification.pdf
  void sphincs_shake_192f_simple_keygen(
    const uint8_t* const __restrict, // 24 -bytes secret key seed
    const uint8_t* const __restrict, // 24 -bytes secret key PRF
    const uint8_t* const __restrict, // 24 -bytes public key seed
    uint8_t* const __restrict,       // 96 -bytes SPHINCS+ secret key
    uint8_t* const __restrict        // 48 -bytes SPHINCS+ public key
  );

  // Generates keypair for SPHINCS+-SHAKE-256s-robust parameter set, as provided
  // in table 3 of the specification
  // https://sphincs.org/data/sphincs+-r3.1-specification.pdf
  void sphincs_shake_256s_robust_keygen(
    const uint8_t* const __restrict, // 32 -bytes secret key seed
    const uint8_t* const __restrict, // 32 -bytes secret key PRF
    const uint8_t* const __restrict, // 32 -bytes public key seed
    uint8_t* const __restrict,       // 128 -bytes SPHINCS+ secret key
    uint8_t* const __restrict        // 64 -bytes SPHINCS+ public key
  );

  // Generates keypair for SPHINCS+-SHAKE-256s-simple parameter set, as provided
  // in table 3 of the specification
  // https://sphincs.org/data/sphincs+-r3.1-specification.pdf
  void sphincs_shake_256s_simple_keygen(
    const uint8_t* const __restrict, // 32 -bytes secret key seed
    const uint8_t* const __restrict, // 32 -bytes secret key PRF
    const uint8_t* const __restrict, // 32 -bytes public key seed
    uint8_t* const __restrict,       // 128 -bytes SPHINCS+ secret key
    uint8_t* const __restrict        // 64 -bytes SPHINCS+ public key
  );

  // Generates keypair for SPHINCS+-SHAKE-256f-robust parameter set, as provided
  // in table 3 of the specification
  // https://sphincs.org/data/sphincs+-r3.1-specification.pdf
  void sphincs_shake_256f_robust_keygen(
    const uint8_t* const __restrict, // 32 -bytes secret key seed
    const uint8_t* const __restrict, // 32 -bytes secret key PRF
    const uint8_t* const __restrict, // 32 -bytes public key seed
    uint8_t* const __restrict,       // 128 -bytes SPHINCS+ secret key
    uint8_t* const __restrict        // 64 -bytes SPHINCS+ public key
  );

  // Generates keypair for SPHINCS+-SHAKE-256f-simple parameter set, as provided
  // in table 3 of the specification
  // https://sphincs.org/data/sphincs+-r3.1-specification.pdf
  void sphincs_shake_256f_simple_keygen(
    const uint8_t* const __restrict, // 32 -bytes secret key seed
    const uint8_t* const __restrict, // 32 -bytes secret key PRF
    const uint8_t* const __restrict, // 32 -bytes public key seed
    uint8_t* const __restrict,       // 128 -bytes SPHINCS+ secret key
    uint8_t* const __restrict        // 64 -bytes SPHINCS+ public key
  );

  // Signing API

  // Sign message using SPHINCS+-SHAKE-128s-robust secret key
  void sphincs_shake_128s_robust_sign(
    const uint8_t* const __restrict, // message to be signed
    const size_t,                    // byte length of message
    const uint8_t* const __restrict, // SPHINCS+ secret key of 64 -bytes
    const uint8_t* const __restrict, // 16 -bytes randomness
    uint8_t* const __restrict        // SPHINCS+ signature
  );

  // Sign message using SPHINCS+-SHAKE-128s-simple secret key
  void sphincs_shake_128s_simple_sign(
    const uint8_t* const __restrict, // message to be signed
    const size_t,                    // byte length of message
    const uint8_t* const __restrict, // SPHINCS+ secret key of 64 -bytes
    const uint8_t* const __restrict, // 16 -bytes randomness
    uint8_t* const __restrict        // SPHINCS+ signature
  );

  // Sign message using SPHINCS+-SHAKE-128f-robust secret key
  void sphincs_shake_128f_robust_sign(
    const uint8_t* const __restrict, // message to be signed
    const size_t,                    // byte length of message
    const uint8_t* const __restrict, // SPHINCS+ secret key of 64 -bytes
    const uint8_t* const __restrict, // 16 -bytes randomness
    uint8_t* const __restrict        // SPHINCS+ signature
  );

  // Sign message using SPHINCS+-SHAKE-128f-simple secret key
  void sphincs_shake_128f_simple_sign(
    const uint8_t* const __restrict, // message to be signed
    const size_t,                    // byte length of message
    const uint8_t* const __restrict, // SPHINCS+ secret key of 64 -bytes
    const uint8_t* const __restrict, // 16 -bytes randomness
    uint8_t* const __restrict        // SPHINCS+ signature
  );

  // Sign message using SPHINCS+-SHAKE-192s-robust secret key
  void sphincs_shake_192s_robust_sign(
    const uint8_t* const __restrict, // message to be signed
    const size_t,                    // byte length of message
    const uint8_t* const __restrict, // SPHINCS+ secret key of 96 -bytes
    const uint8_t* const __restrict, // 24 -bytes randomness
    uint8_t* const __restrict        // SPHINCS+ signature
  );

  // Sign message using SPHINCS+-SHAKE-192s-simple secret key
  void sphincs_shake_192s_simple_sign(
    const uint8_t* const __restrict, // message to be signed
    const size_t,                    // byte length of message
    const uint8_t* const __restrict, // SPHINCS+ secret key of 96 -bytes
    const uint8_t* const __restrict, // 24 -bytes randomness
    uint8_t* const __restrict        // SPHINCS+ signature
  );

  // Sign message using SPHINCS+-SHAKE-192f-robust secret key
  void sphincs_shake_192f_robust_sign(
    const uint8_t* const __restrict, // message to be signed
    const size_t,                    // byte length of message
    const uint8_t* const __restrict, // SPHINCS+ secret key of 96 -bytes
    const uint8_t* const __restrict, // 24 -bytes randomness
    uint8_t* const __restrict        // SPHINCS+ signature
  );

  // Sign message using SPHINCS+-SHAKE-192f-simple secret key
  void sphincs_shake_192f_simple_sign(
    const uint8_t* const __restrict, // message to be signed
    const size_t,                    // byte length of message
    const uint8_t* const __restrict, // SPHINCS+ secret key of 96 -bytes
    const uint8_t* const __restrict, // 24 -bytes randomness
    uint8_t* const __restrict        // SPHINCS+ signature
  );

  // Sign message using SPHINCS+-SHAKE-256s-robust secret key
  void sphincs_shake_256s_robust_sign(
    const uint8_t* const __restrict, // message to be signed
    const size_t,                    // byte length of message
    const uint8_t* const __restrict, // SPHINCS+ secret key of 128 -bytes
    const uint8_t* const __restrict, // 32 -bytes randomness
    uint8_t* const __restrict        // SPHINCS+ signature
  );

  // Sign message using SPHINCS+-SHAKE-256s-simple secret key
  void sphincs_shake_256s_simple_sign(
    const uint8_t* const __restrict, // message to be signed
    const size_t,                    // byte length of message
    const uint8_t* const __restrict, // SPHINCS+ secret key of 128 -bytes
    const uint8_t* const __restrict, // 32 -bytes randomness
    uint8_t* const __restrict        // SPHINCS+ signature
  );

  // Sign message using SPHINCS+-SHAKE-256f-robust secret key
  void sphincs_shake_256f_robust_sign(
    const uint8_t* const __restrict, // message to be signed
    const size_t,                    // byte length of message
    const uint8_t* const __restrict, // SPHINCS+ secret key of 128 -bytes
    const uint8_t* const __restrict, // 32 -bytes randomness
    uint8_t* const __restrict        // SPHINCS+ signature
  );

  // Sign message using SPHINCS+-SHAKE-256f-simple secret key
  void sphincs_shake_256f_simple_sign(
    const uint8_t* const __restrict, // message to be signed
    const size_t,                    // byte length of message
    const uint8_t* const __restrict, // SPHINCS+ secret key of 128 -bytes
    const uint8_t* const __restrict, // 32 -bytes randomness
    uint8_t* const __restrict        // SPHINCS+ signature
  );

  // Verification API

  // Verify signature using SPHINCS+-SHAKE-128s-robust public key
  bool sphincs_shake_128s_robust_verify(
    const uint8_t* const __restrict, // message which was signed
    const size_t,                    // byte length of message
    const uint8_t* const __restrict, // SPHINCS+ signature
    const uint8_t* const __restrict  // SPHINCS+ public key of 32 -bytes
  );

  // Verify signature using SPHINCS+-SHAKE-128s-simple public key
  bool sphincs_shake_128s_simple_verify(
    const uint8_t* const __restrict, // message which was signed
    const size_t,                    // byte length of message
    const uint8_t* const __restrict, // SPHINCS+ signature
    const uint8_t* const __restrict  // SPHINCS+ public key of 32 -bytes
  );

  // Verify signature using SPHINCS+-SHAKE-128f-robust public key
  bool sphincs_shake_128f_robust_verify(
    const uint8_t* const __restrict, // message which was signed
    const size_t,                    // byte length of message
    const uint8_t* const __restrict, // SPHINCS+ signature
    const uint8_t* const __restrict  // SPHINCS+ public key of 32 -bytes
  );

  // Verify signature using SPHINCS+-SHAKE-128f-simple public key
  bool sphincs_shake_128f_simple_verify(
    const uint8_t* const __restrict, // message which was signed
    const size_t,                    // byte length of message
    const uint8_t* const __restrict, // SPHINCS+ signature
    const uint8_t* const __restrict  // SPHINCS+ public key of 32 -bytes
  );

  // Verify signature using SPHINCS+-SHAKE-192s-robust public key
  bool sphincs_shake_192s_robust_verify(
    const uint8_t* const __restrict, // message which was signed
    const size_t,                    // byte length of message
    const uint8_t* const __restrict, // SPHINCS+ signature
    const uint8_t* const __restrict  // SPHINCS+ public key of 48 -bytes
  );

  // Verify signature using SPHINCS+-SHAKE-192s-simple public key
  bool sphincs_shake_192s_simple_verify(
    const uint8_t* const __restrict, // message which was signed
    const size_t,                    // byte length of message
    const uint8_t* const __restrict, // SPHINCS+ signature
    const uint8_t* const __restrict  // SPHINCS+ public key of 48 -bytes
  );

  // Verify signature using SPHINCS+-SHAKE-192f-robust public key
  bool sphincs_shake_192f_robust_verify(
    const uint8_t* const __restrict, // message which was signed
    const size_t,                    // byte length of message
    const uint8_t* const __restrict, // SPHINCS+ signature
    const uint8_t* const __restrict  // SPHINCS+ public key of 48 -bytes
  );

  // Verify signature using SPHINCS+-SHAKE-192f-simple public key
  bool sphincs_shake_192f_simple_verify(
    const uint8_t* const __restrict, // message which was signed
    const size_t,                    // byte length of message
    const uint8_t* const __restrict, // SPHINCS+ signature
    const uint8_t* const __restrict  // SPHINCS+ public key of 48 -bytes
  );

  // Verify signature using SPHINCS+-SHAKE-256s-robust public key
  bool sphincs_shake_256s_robust_verify(
    const uint8_t* const __restrict, // message which was signed
    const size_t,                    // byte length of message
    const uint8_t* const __restrict, // SPHINCS+ signature
    const uint8_t* const __restrict  // SPHINCS+ public key of 64 -bytes
  );

  // Verify signature using SPHINCS+-SHAKE-256s-simple public key
  bool sphincs_shake_256s_simple_verify(
    const uint8_t* const __restrict, // message which was signed
    const size_t,                    // byte length of message
    const uint8_t* const __restrict, // SPHINCS+ signature
    const uint8_t* const __restrict  // SPHINCS+ public key of 64 -bytes
  );

  // Verify signature using SPHINCS+-SHAKE-256f-robust public key
  bool sphincs_shake_256f_robust_verify(
    const uint8_t* const __restrict, // message which was signed
    const size_t,                    // byte length of message
    const uint8_t* const __restrict, // SPHINCS+ signature
    const uint8_t* const __restrict  // SPHINCS+ public key of 64 -bytes
  );

  // Verify signature using SPHINCS+-SHAKE-256f-simple public key
  bool sphincs_shake_256f_simple_verify(
    const uint8_t* const __restrict, // message which was signed
    const size_t,                    // byte length of message
    const uint8_t* const __restrict, // SPHINCS+ signature
    const uint8_t* const __restrict  // SPHINCS+ public key of 64 -bytes
  );
}

// Function implementations
extern "C"
{

  // Key Generation API

  void sphincs_shake_128s_robust_keygen(const uint8_t* const __restrict sk_seed,
                                        const uint8_t* const __restrict sk_prf,
                                        const uint8_t* const __restrict pk_seed,
                                        uint8_t* const __restrict skey,
                                        uint8_t* const __restrict pkey)
  {
    constexpr size_t n = 16;
    constexpr uint32_t h = 63;
    constexpr uint32_t d = 7;
    constexpr size_t w = 16;
    constexpr auto r = sphincs_hashing::variant::robust;

    sphincs_inner::keygen<n, h, d, w, r>(sk_seed, sk_prf, pk_seed, skey, pkey);
  }

  void sphincs_shake_128s_simple_keygen(const uint8_t* const __restrict sk_seed,
                                        const uint8_t* const __restrict sk_prf,
                                        const uint8_t* const __restrict pk_seed,
                                        uint8_t* const __restrict skey,
                                        uint8_t* const __restrict pkey)
  {
    constexpr size_t n = 16;
    constexpr uint32_t h = 63;
    constexpr uint32_t d = 7;
    constexpr size_t w = 16;
    constexpr auto r = sphincs_hashing::variant::simple;

    sphincs_inner::keygen<n, h, d, w, r>(sk_seed, sk_prf, pk_seed, skey, pkey);
  }

  void sphincs_shake_128f_robust_keygen(const uint8_t* const __restrict sk_seed,
                                        const uint8_t* const __restrict sk_prf,
                                        const uint8_t* const __restrict pk_seed,
                                        uint8_t* const __restrict skey,
                                        uint8_t* const __restrict pkey)
  {
    constexpr size_t n = 16;
    constexpr uint32_t h = 66;
    constexpr uint32_t d = 22;
    constexpr size_t w = 16;
    constexpr auto r = sphincs_hashing::variant::robust;

    sphincs_inner::keygen<n, h, d, w, r>(sk_seed, sk_prf, pk_seed, skey, pkey);
  }

  void sphincs_shake_128f_simple_keygen(const uint8_t* const __restrict sk_seed,
                                        const uint8_t* const __restrict sk_prf,
                                        const uint8_t* const __restrict pk_seed,
                                        uint8_t* const __restrict skey,
                                        uint8_t* const __restrict pkey)
  {
    constexpr size_t n = 16;
    constexpr uint32_t h = 66;
    constexpr uint32_t d = 22;
    constexpr size_t w = 16;
    constexpr auto r = sphincs_hashing::variant::simple;

    sphincs_inner::keygen<n, h, d, w, r>(sk_seed, sk_prf, pk_seed, skey, pkey);
  }

  void sphincs_shake_192s_robust_keygen(const uint8_t* const __restrict sk_seed,
                                        const uint8_t* const __restrict sk_prf,
                                        const uint8_t* const __restrict pk_seed,
                                        uint8_t* const __restrict skey,
                                        uint8_t* const __restrict pkey)
  {
    constexpr size_t n = 24;
    constexpr uint32_t h = 63;
    constexpr uint32_t d = 7;
    constexpr size_t w = 16;
    constexpr auto r = sphincs_hashing::variant::robust;

    sphincs_inner::keygen<n, h, d, w, r>(sk_seed, sk_prf, pk_seed, skey, pkey);
  }

  void sphincs_shake_192s_simple_keygen(const uint8_t* const __restrict sk_seed,
                                        const uint8_t* const __restrict sk_prf,
                                        const uint8_t* const __restrict pk_seed,
                                        uint8_t* const __restrict skey,
                                        uint8_t* const __restrict pkey)
  {
    constexpr size_t n = 24;
    constexpr uint32_t h = 63;
    constexpr uint32_t d = 7;
    constexpr size_t w = 16;
    constexpr auto r = sphincs_hashing::variant::simple;

    sphincs_inner::keygen<n, h, d, w, r>(sk_seed, sk_prf, pk_seed, skey, pkey);
  }

  void sphincs_shake_192f_robust_keygen(const uint8_t* const __restrict sk_seed,
                                        const uint8_t* const __restrict sk_prf,
                                        const uint8_t* const __restrict pk_seed,
                                        uint8_t* const __restrict skey,
                                        uint8_t* const __restrict pkey)
  {
    constexpr size_t n = 24;
    constexpr uint32_t h = 66;
    constexpr uint32_t d = 22;
    constexpr size_t w = 16;
    constexpr auto r = sphincs_hashing::variant::robust;

    sphincs_inner::keygen<n, h, d, w, r>(sk_seed, sk_prf, pk_seed, skey, pkey);
  }

  void sphincs_shake_192f_simple_keygen(const uint8_t* const __restrict sk_seed,
                                        const uint8_t* const __restrict sk_prf,
                                        const uint8_t* const __restrict pk_seed,
                                        uint8_t* const __restrict skey,
                                        uint8_t* const __restrict pkey)
  {
    constexpr size_t n = 24;
    constexpr uint32_t h = 66;
    constexpr uint32_t d = 22;
    constexpr size_t w = 16;
    constexpr auto r = sphincs_hashing::variant::simple;

    sphincs_inner::keygen<n, h, d, w, r>(sk_seed, sk_prf, pk_seed, skey, pkey);
  }

  void sphincs_shake_256s_robust_keygen(const uint8_t* const __restrict sk_seed,
                                        const uint8_t* const __restrict sk_prf,
                                        const uint8_t* const __restrict pk_seed,
                                        uint8_t* const __restrict skey,
                                        uint8_t* const __restrict pkey)
  {
    constexpr size_t n = 32;
    constexpr uint32_t h = 64;
    constexpr uint32_t d = 8;
    constexpr size_t w = 16;
    constexpr auto r = sphincs_hashing::variant::robust;

    sphincs_inner::keygen<n, h, d, w, r>(sk_seed, sk_prf, pk_seed, skey, pkey);
  }

  void sphincs_shake_256s_simple_keygen(const uint8_t* const __restrict sk_seed,
                                        const uint8_t* const __restrict sk_prf,
                                        const uint8_t* const __restrict pk_seed,
                                        uint8_t* const __restrict skey,
                                        uint8_t* const __restrict pkey)
  {
    constexpr size_t n = 32;
    constexpr uint32_t h = 64;
    constexpr uint32_t d = 8;
    constexpr size_t w = 16;
    constexpr auto r = sphincs_hashing::variant::simple;

    sphincs_inner::keygen<n, h, d, w, r>(sk_seed, sk_prf, pk_seed, skey, pkey);
  }

  void sphincs_shake_256f_robust_keygen(const uint8_t* const __restrict sk_seed,
                                        const uint8_t* const __restrict sk_prf,
                                        const uint8_t* const __restrict pk_seed,
                                        uint8_t* const __restrict skey,
                                        uint8_t* const __restrict pkey)
  {
    constexpr size_t n = 32;
    constexpr uint32_t h = 68;
    constexpr uint32_t d = 17;
    constexpr size_t w = 16;
    constexpr auto r = sphincs_hashing::variant::robust;

    sphincs_inner::keygen<n, h, d, w, r>(sk_seed, sk_prf, pk_seed, skey, pkey);
  }

  void sphincs_shake_256f_simple_keygen(const uint8_t* const __restrict sk_seed,
                                        const uint8_t* const __restrict sk_prf,
                                        const uint8_t* const __restrict pk_seed,
                                        uint8_t* const __restrict skey,
                                        uint8_t* const __restrict pkey)
  {
    constexpr size_t n = 32;
    constexpr uint32_t h = 68;
    constexpr uint32_t d = 17;
    constexpr size_t w = 16;
    constexpr auto r = sphincs_hashing::variant::simple;

    sphincs_inner::keygen<n, h, d, w, r>(sk_seed, sk_prf, pk_seed, skey, pkey);
  }

  void sphincs_shake_128s_robust_sign(const uint8_t* const __restrict msg,
                                      const size_t mlen,
                                      const uint8_t* const __restrict skey,
                                      const uint8_t* const __restrict rbytes,
                                      uint8_t* const __restrict sig)
  {
    constexpr size_t n = 16;
    constexpr uint32_t h = 63;
    constexpr uint32_t d = 7;
    constexpr uint32_t a = 12;
    constexpr uint32_t k = 14;
    constexpr size_t w = 16;
    constexpr auto v = sphincs_hashing::variant::robust;
    constexpr bool r = true;

    sphincs_inner::sign<n, h, d, a, k, w, v, r>(msg, mlen, skey, rbytes, sig);
  }

  void sphincs_shake_128s_simple_sign(const uint8_t* const __restrict msg,
                                      const size_t mlen,
                                      const uint8_t* const __restrict skey,
                                      const uint8_t* const __restrict rbytes,
                                      uint8_t* const __restrict sig)
  {
    constexpr size_t n = 16;
    constexpr uint32_t h = 63;
    constexpr uint32_t d = 7;
    constexpr uint32_t a = 12;
    constexpr uint32_t k = 14;
    constexpr size_t w = 16;
    constexpr auto v = sphincs_hashing::variant::simple;
    constexpr bool r = true;

    sphincs_inner::sign<n, h, d, a, k, w, v, r>(msg, mlen, skey, rbytes, sig);
  }

  void sphincs_shake_128f_robust_sign(const uint8_t* const __restrict msg,
                                      const size_t mlen,
                                      const uint8_t* const __restrict skey,
                                      const uint8_t* const __restrict rbytes,
                                      uint8_t* const __restrict sig)
  {
    constexpr size_t n = 16;
    constexpr uint32_t h = 66;
    constexpr uint32_t d = 22;
    constexpr uint32_t a = 6;
    constexpr uint32_t k = 33;
    constexpr size_t w = 16;
    constexpr auto v = sphincs_hashing::variant::robust;
    constexpr bool r = true;

    sphincs_inner::sign<n, h, d, a, k, w, v, r>(msg, mlen, skey, rbytes, sig);
  }

  void sphincs_shake_128f_simple_sign(const uint8_t* const __restrict msg,
                                      const size_t mlen,
                                      const uint8_t* const __restrict skey,
                                      const uint8_t* const __restrict rbytes,
                                      uint8_t* const __restrict sig)
  {
    constexpr size_t n = 16;
    constexpr uint32_t h = 66;
    constexpr uint32_t d = 22;
    constexpr uint32_t a = 6;
    constexpr uint32_t k = 33;
    constexpr size_t w = 16;
    constexpr auto v = sphincs_hashing::variant::simple;
    constexpr bool r = true;

    sphincs_inner::sign<n, h, d, a, k, w, v, r>(msg, mlen, skey, rbytes, sig);
  }

  void sphincs_shake_192s_robust_sign(const uint8_t* const __restrict msg,
                                      const size_t mlen,
                                      const uint8_t* const __restrict skey,
                                      const uint8_t* const __restrict rbytes,
                                      uint8_t* const __restrict sig)
  {
    constexpr size_t n = 24;
    constexpr uint32_t h = 63;
    constexpr uint32_t d = 7;
    constexpr uint32_t a = 14;
    constexpr uint32_t k = 17;
    constexpr size_t w = 16;
    constexpr auto v = sphincs_hashing::variant::robust;
    constexpr bool r = true;

    sphincs_inner::sign<n, h, d, a, k, w, v, r>(msg, mlen, skey, rbytes, sig);
  }

  void sphincs_shake_192s_simple_sign(const uint8_t* const __restrict msg,
                                      const size_t mlen,
                                      const uint8_t* const __restrict skey,
                                      const uint8_t* const __restrict rbytes,
                                      uint8_t* const __restrict sig)
  {
    constexpr size_t n = 24;
    constexpr uint32_t h = 63;
    constexpr uint32_t d = 7;
    constexpr uint32_t a = 14;
    constexpr uint32_t k = 17;
    constexpr size_t w = 16;
    constexpr auto v = sphincs_hashing::variant::simple;
    constexpr bool r = true;

    sphincs_inner::sign<n, h, d, a, k, w, v, r>(msg, mlen, skey, rbytes, sig);
  }

  void sphincs_shake_192f_robust_sign(const uint8_t* const __restrict msg,
                                      const size_t mlen,
                                      const uint8_t* const __restrict skey,
                                      const uint8_t* const __restrict rbytes,
                                      uint8_t* const __restrict sig)
  {
    constexpr size_t n = 24;
    constexpr uint32_t h = 66;
    constexpr uint32_t d = 22;
    constexpr uint32_t a = 8;
    constexpr uint32_t k = 33;
    constexpr size_t w = 16;
    constexpr auto v = sphincs_hashing::variant::robust;
    constexpr bool r = true;

    sphincs_inner::sign<n, h, d, a, k, w, v, r>(msg, mlen, skey, rbytes, sig);
  }

  void sphincs_shake_192f_simple_sign(const uint8_t* const __restrict msg,
                                      const size_t mlen,
                                      const uint8_t* const __restrict skey,
                                      const uint8_t* const __restrict rbytes,
                                      uint8_t* const __restrict sig)
  {
    constexpr size_t n = 24;
    constexpr uint32_t h = 66;
    constexpr uint32_t d = 22;
    constexpr uint32_t a = 8;
    constexpr uint32_t k = 33;
    constexpr size_t w = 16;
    constexpr auto v = sphincs_hashing::variant::simple;
    constexpr bool r = true;

    sphincs_inner::sign<n, h, d, a, k, w, v, r>(msg, mlen, skey, rbytes, sig);
  }

  void sphincs_shake_256s_robust_sign(const uint8_t* const __restrict msg,
                                      const size_t mlen,
                                      const uint8_t* const __restrict skey,
                                      const uint8_t* const __restrict rbytes,
                                      uint8_t* const __restrict sig)
  {
    constexpr size_t n = 32;
    constexpr uint32_t h = 64;
    constexpr uint32_t d = 8;
    constexpr uint32_t a = 14;
    constexpr uint32_t k = 22;
    constexpr size_t w = 16;
    constexpr auto v = sphincs_hashing::variant::robust;
    constexpr bool r = true;

    sphincs_inner::sign<n, h, d, a, k, w, v, r>(msg, mlen, skey, rbytes, sig);
  }

  void sphincs_shake_256s_simple_sign(const uint8_t* const __restrict msg,
                                      const size_t mlen,
                                      const uint8_t* const __restrict skey,
                                      const uint8_t* const __restrict rbytes,
                                      uint8_t* const __restrict sig)
  {
    constexpr size_t n = 32;
    constexpr uint32_t h = 64;
    constexpr uint32_t d = 8;
    constexpr uint32_t a = 14;
    constexpr uint32_t k = 22;
    constexpr size_t w = 16;
    constexpr auto v = sphincs_hashing::variant::simple;
    constexpr bool r = true;

    sphincs_inner::sign<n, h, d, a, k, w, v, r>(msg, mlen, skey, rbytes, sig);
  }

  void sphincs_shake_256f_robust_sign(const uint8_t* const __restrict msg,
                                      const size_t mlen,
                                      const uint8_t* const __restrict skey,
                                      const uint8_t* const __restrict rbytes,
                                      uint8_t* const __restrict sig)
  {
    constexpr size_t n = 32;
    constexpr uint32_t h = 68;
    constexpr uint32_t d = 17;
    constexpr uint32_t a = 9;
    constexpr uint32_t k = 35;
    constexpr size_t w = 16;
    constexpr auto v = sphincs_hashing::variant::robust;
    constexpr bool r = true;

    sphincs_inner::sign<n, h, d, a, k, w, v, r>(msg, mlen, skey, rbytes, sig);
  }

  void sphincs_shake_256f_simple_sign(const uint8_t* const __restrict msg,
                                      const size_t mlen,
                                      const uint8_t* const __restrict skey,
                                      const uint8_t* const __restrict rbytes,
                                      uint8_t* const __restrict sig)
  {
    constexpr size_t n = 32;
    constexpr uint32_t h = 68;
    constexpr uint32_t d = 17;
    constexpr uint32_t a = 9;
    constexpr uint32_t k = 35;
    constexpr size_t w = 16;
    constexpr auto v = sphincs_hashing::variant::simple;
    constexpr bool r = true;

    sphincs_inner::sign<n, h, d, a, k, w, v, r>(msg, mlen, skey, rbytes, sig);
  }

  bool sphincs_shake_128s_robust_verify(const uint8_t* const __restrict msg,
                                        const size_t mlen,
                                        const uint8_t* const __restrict sig,
                                        const uint8_t* const __restrict pkey)
  {
    constexpr size_t n = 16;
    constexpr uint32_t h = 63;
    constexpr uint32_t d = 7;
    constexpr uint32_t a = 12;
    constexpr uint32_t k = 14;
    constexpr size_t w = 16;
    constexpr auto v = sphincs_hashing::variant::robust;

    return sphincs_inner::verify<n, h, d, a, k, w, v>(msg, mlen, sig, pkey);
  }

  bool sphincs_shake_128s_simple_verify(const uint8_t* const __restrict msg,
                                        const size_t mlen,
                                        const uint8_t* const __restrict sig,
                                        const uint8_t* const __restrict pkey)
  {
    constexpr size_t n = 16;
    constexpr uint32_t h = 63;
    constexpr uint32_t d = 7;
    constexpr uint32_t a = 12;
    constexpr uint32_t k = 14;
    constexpr size_t w = 16;
    constexpr auto v = sphincs_hashing::variant::simple;

    return sphincs_inner::verify<n, h, d, a, k, w, v>(msg, mlen, sig, pkey);
  }

  bool sphincs_shake_128f_robust_verify(const uint8_t* const __restrict msg,
                                        const size_t mlen,
                                        const uint8_t* const __restrict sig,
                                        const uint8_t* const __restrict pkey)
  {
    constexpr size_t n = 16;
    constexpr uint32_t h = 66;
    constexpr uint32_t d = 22;
    constexpr uint32_t a = 6;
    constexpr uint32_t k = 33;
    constexpr size_t w = 16;
    constexpr auto v = sphincs_hashing::variant::robust;

    return sphincs_inner::verify<n, h, d, a, k, w, v>(msg, mlen, sig, pkey);
  }

  bool sphincs_shake_128f_simple_verify(const uint8_t* const __restrict msg,
                                        const size_t mlen,
                                        const uint8_t* const __restrict sig,
                                        const uint8_t* const __restrict pkey)
  {
    constexpr size_t n = 16;
    constexpr uint32_t h = 66;
    constexpr uint32_t d = 22;
    constexpr uint32_t a = 6;
    constexpr uint32_t k = 33;
    constexpr size_t w = 16;
    constexpr auto v = sphincs_hashing::variant::simple;

    return sphincs_inner::verify<n, h, d, a, k, w, v>(msg, mlen, sig, pkey);
  }

  bool sphincs_shake_192s_robust_verify(const uint8_t* const __restrict msg,
                                        const size_t mlen,
                                        const uint8_t* const __restrict sig,
                                        const uint8_t* const __restrict pkey)
  {
    constexpr size_t n = 24;
    constexpr uint32_t h = 63;
    constexpr uint32_t d = 7;
    constexpr uint32_t a = 14;
    constexpr uint32_t k = 17;
    constexpr size_t w = 16;
    constexpr auto v = sphincs_hashing::variant::robust;

    return sphincs_inner::verify<n, h, d, a, k, w, v>(msg, mlen, sig, pkey);
  }

  bool sphincs_shake_192s_simple_verify(const uint8_t* const __restrict msg,
                                        const size_t mlen,
                                        const uint8_t* const __restrict sig,
                                        const uint8_t* const __restrict pkey)
  {
    constexpr size_t n = 24;
    constexpr uint32_t h = 63;
    constexpr uint32_t d = 7;
    constexpr uint32_t a = 14;
    constexpr uint32_t k = 17;
    constexpr size_t w = 16;
    constexpr auto v = sphincs_hashing::variant::simple;

    return sphincs_inner::verify<n, h, d, a, k, w, v>(msg, mlen, sig, pkey);
  }

  bool sphincs_shake_192f_robust_verify(const uint8_t* const __restrict msg,
                                        const size_t mlen,
                                        const uint8_t* const __restrict sig,
                                        const uint8_t* const __restrict pkey)
  {
    constexpr size_t n = 24;
    constexpr uint32_t h = 66;
    constexpr uint32_t d = 22;
    constexpr uint32_t a = 8;
    constexpr uint32_t k = 33;
    constexpr size_t w = 16;
    constexpr auto v = sphincs_hashing::variant::robust;

    return sphincs_inner::verify<n, h, d, a, k, w, v>(msg, mlen, sig, pkey);
  }

  bool sphincs_shake_192f_simple_verify(const uint8_t* const __restrict msg,
                                        const size_t mlen,
                                        const uint8_t* const __restrict sig,
                                        const uint8_t* const __restrict pkey)
  {
    constexpr size_t n = 24;
    constexpr uint32_t h = 66;
    constexpr uint32_t d = 22;
    constexpr uint32_t a = 8;
    constexpr uint32_t k = 33;
    constexpr size_t w = 16;
    constexpr auto v = sphincs_hashing::variant::simple;

    return sphincs_inner::verify<n, h, d, a, k, w, v>(msg, mlen, sig, pkey);
  }

  bool sphincs_shake_256s_robust_verify(const uint8_t* const __restrict msg,
                                        const size_t mlen,
                                        const uint8_t* const __restrict sig,
                                        const uint8_t* const __restrict pkey)
  {
    constexpr size_t n = 32;
    constexpr uint32_t h = 64;
    constexpr uint32_t d = 8;
    constexpr uint32_t a = 14;
    constexpr uint32_t k = 22;
    constexpr size_t w = 16;
    constexpr auto v = sphincs_hashing::variant::robust;

    return sphincs_inner::verify<n, h, d, a, k, w, v>(msg, mlen, sig, pkey);
  }

  bool sphincs_shake_256s_simple_verify(const uint8_t* const __restrict msg,
                                        const size_t mlen,
                                        const uint8_t* const __restrict sig,
                                        const uint8_t* const __restrict pkey)
  {
    constexpr size_t n = 32;
    constexpr uint32_t h = 64;
    constexpr uint32_t d = 8;
    constexpr uint32_t a = 14;
    constexpr uint32_t k = 22;
    constexpr size_t w = 16;
    constexpr auto v = sphincs_hashing::variant::simple;

    return sphincs_inner::verify<n, h, d, a, k, w, v>(msg, mlen, sig, pkey);
  }

  bool sphincs_shake_256f_robust_verify(const uint8_t* const __restrict msg,
                                        const size_t mlen,
                                        const uint8_t* const __restrict sig,
                                        const uint8_t* const __restrict pkey)
  {
    constexpr size_t n = 32;
    constexpr uint32_t h = 68;
    constexpr uint32_t d = 17;
    constexpr uint32_t a = 9;
    constexpr uint32_t k = 35;
    constexpr size_t w = 16;
    constexpr auto v = sphincs_hashing::variant::robust;

    return sphincs_inner::verify<n, h, d, a, k, w, v>(msg, mlen, sig, pkey);
  }

  bool sphincs_shake_256f_simple_verify(const uint8_t* const __restrict msg,
                                        const size_t mlen,
                                        const uint8_t* const __restrict sig,
                                        const uint8_t* const __restrict pkey)
  {
    constexpr size_t n = 32;
    constexpr uint32_t h = 68;
    constexpr uint32_t d = 17;
    constexpr uint32_t a = 9;
    constexpr uint32_t k = 35;
    constexpr size_t w = 16;
    constexpr auto v = sphincs_hashing::variant::simple;

    return sphincs_inner::verify<n, h, d, a, k, w, v>(msg, mlen, sig, pkey);
  }
}
