#include "prng.hpp"
#include "sphincs+_128f_simple.hpp"
#include <cstdio>

#define DUDECT_IMPLEMENTATION
#define DUDECT_VISIBLITY_STATIC
#include "dudect.h"

constexpr size_t MSG_BYTE_LEN = 64;

uint8_t
do_one_computation(uint8_t* const data)
{
  constexpr size_t doff0 = 0;
  constexpr size_t doff1 = doff0 + MSG_BYTE_LEN;
  constexpr size_t doff2 = doff1 + sphincs_plus_128f_simple::SecKeyLen;
  constexpr size_t doff3 = doff2 + sphincs_plus_128f_simple::n;

  auto msg = std::span(data + doff0, doff1 - doff0);
  auto skey = std::span<const uint8_t, doff2 - doff1>(data + doff1, doff2 - doff1);
  auto rand_bytes = std::span<const uint8_t, doff3 - doff2>(data + doff2, doff3 - doff2);

  std::array<uint8_t, sphincs_plus_128f_simple::SigLen> sig{};

  uint8_t ret_val = 0;

  sphincs_plus_128f_simple::sign<true>(msg, skey, rand_bytes, sig);
  ret_val ^= sig[0] ^ sig[sig.size() - 1];

  return ret_val;
}

void
prepare_inputs(dudect_config_t* const c, uint8_t* const input_data, uint8_t* const classes)
{
  randombytes(input_data, c->number_measurements * c->chunk_size);

  for (size_t i = 0; i < c->number_measurements; i++) {
    classes[i] = randombit();
    if (classes[i] == 0) {
      const size_t chunk_off = i * c->chunk_size;
      const size_t skey_begin = chunk_off + MSG_BYTE_LEN;

      std::array<uint8_t, sphincs_plus_128f_simple::n> sk_seed{};
      std::array<uint8_t, sphincs_plus_128f_simple::n> sk_prf{};
      std::array<uint8_t, sphincs_plus_128f_simple::n> pk_seed{};
      auto skey = std::span<uint8_t, sphincs_plus_128f_simple::SecKeyLen>(input_data + skey_begin, sphincs_plus_128f_simple::SecKeyLen);
      std::array<uint8_t, sphincs_plus_128f_simple::PubKeyLen> pkey{};

      prng::prng_t prng;

      prng.read(sk_seed);
      prng.read(sk_prf);
      prng.read(pk_seed);

      sphincs_plus_128f_simple::keygen(sk_seed, sk_prf, pk_seed, skey, pkey);
    }
  }
}

dudect_state_t
test_sphincs_plus_128f_simple_sign()
{
  constexpr size_t chunk_size = MSG_BYTE_LEN +                        // Message to be signed
                                sphincs_plus_128f_simple::SecKeyLen + // Secret key used for signing the message
                                sphincs_plus_128f_simple::n;          // Random bytes used for randomized message signing
  constexpr size_t number_measurements = 1e4;

  dudect_config_t config = {
    chunk_size,
    number_measurements,
  };
  dudect_ctx_t ctx;
  dudect_init(&ctx, &config);

  dudect_state_t state = DUDECT_NO_LEAKAGE_EVIDENCE_YET;
  while (state == DUDECT_NO_LEAKAGE_EVIDENCE_YET) {
    state = dudect_main(&ctx);
  }

  dudect_free(&ctx);

  printf("Detected timing leakage in \"%s\", defined in file \"%s\"\n", __func__, __FILE_NAME__);
  return state;
}

int
main()
{
  if (test_sphincs_plus_128f_simple_sign() != DUDECT_NO_LEAKAGE_EVIDENCE_YET) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
