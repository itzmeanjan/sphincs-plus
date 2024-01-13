#pragma once
#include "hashing.hpp"
#include <cstddef>
#include <cstdint>

// Compile-time checks ensuring SPHINCS+ is instantiated with correct parameters
namespace sphincs_plus_params {

// Compile-time check to ensure that SPHINCS+ key generation function is only
// invoked with parameter sets suggested in table 3 of the specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n, uint32_t h, uint32_t d, size_t w, sphincs_plus_hashing::variant v>
static inline constexpr bool
check_keygen_params()
{
  constexpr bool flg0 = w == 16;
  constexpr bool flg1 = (v == sphincs_plus_hashing::variant::robust) | (v == sphincs_plus_hashing::variant::simple);

  constexpr bool flg2 = (n == 16) & (h == 63) & (d == 7);
  constexpr bool flg3 = (n == 16) & (h == 66) & (d == 22);
  constexpr bool flg4 = (n == 24) & (h == 63) & (d == 7);
  constexpr bool flg5 = (n == 24) & (h == 66) & (d == 22);
  constexpr bool flg6 = (n == 32) & (h == 64) & (d == 8);
  constexpr bool flg7 = (n == 32) & (h == 68) & (d == 17);

  return (flg2 | flg3 | flg4 | flg5 | flg6 | flg7) & flg0 & flg1;
}

// Compile-time check to ensure that SPHINCS+ sign/ verify function is only
// invoked with parameter sets suggested in table 3 of the specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n, uint32_t h, uint32_t d, uint32_t a, uint32_t k, size_t w, sphincs_plus_hashing::variant v>
static inline constexpr bool
check_sign_verify_params()
{
  constexpr bool flg0 = w == 16;
  constexpr bool flg1 = (v == sphincs_plus_hashing::variant::robust) || (v == sphincs_plus_hashing::variant::simple);

  const bool flg2 = (n == 16) & (h == 63) & (d == 7) & (a == 12) & (k == 14);
  const bool flg3 = (n == 16) & (h == 66) & (d == 22) & (a == 6) & (k == 33);
  const bool flg4 = (n == 24) & (h == 63) & (d == 7) & (a == 14) & (k == 17);
  const bool flg5 = (n == 24) & (h == 66) & (d == 22) & (a == 8) & (k == 33);
  const bool flg6 = (n == 32) & (h == 64) & (d == 8) & (a == 14) & (k == 22);
  const bool flg7 = (n == 32) & (h == 68) & (d == 17) & (a == 9) & (k == 35);

  return (flg2 | flg3 | flg4 | flg5 | flg6 | flg7) & flg0 & flg1;
}

// Compile-time check to ensure that HyperTree's total height ( say h ) and
// number of layers ( say d ) are conformant so that we can use 64 -bit unsigned
// integer for indexing tree.
//
// Read more about this constraint in section 4.2.4 of the specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
static inline constexpr bool
check_ht_height_and_layer(const uint32_t h, const uint32_t d)
{
  return (h - (h / d)) <= 64u;
}

// Compile-time check to ensure that `w` parameter takes only allowed values.
//
// See Winternitz Parameter point in section 3.1 of
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
static inline constexpr bool
check_w(const size_t w)
{
  return (w == 4) || (w == 16) || (w == 256);
}

}
