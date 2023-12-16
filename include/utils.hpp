#pragma once
#include "hashing.hpp"
#include <array>
#include <bit>
#include <cassert>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <random>
#include <sstream>
#include <type_traits>

// Utility functions for SPHINCS+
namespace sphincs_plus_utils {

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

// Compile-time compute logarithm base 2 of Winternitz parameter `w` for WOTS+
//
// See section 3.1 of SPHINCS+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t w>
static inline constexpr size_t
log2()
  requires(check_w(w))
{
  return std::bit_width(w) - 1;
}

// Compile-time compute `len1` parameter for WOTS+
//
// See section 3.1 of SPHINCS+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n, size_t w>
static inline constexpr size_t
compute_wots_len1()
{
  return (8 * n) / log2<w>();
}

// Compile-time compute `len2` parameter for WOTS+
//
// See section 3.1 of SPHINCS+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n, size_t w, size_t len1>
static inline constexpr size_t
compute_wots_len2()
{
  constexpr size_t t0 = len1 * (w - 1);
  constexpr size_t t1 = std::bit_width(t0);
  constexpr size_t t2 = t1 / log2<w>();
  constexpr size_t t3 = t2 + 1;
  return t3;
}

// Compile-time compute `len` parameter for WOTS+
//
// See section 3.1 of SPHINCS+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n, size_t w>
static inline constexpr size_t
compute_wots_len()
{
  constexpr size_t len1 = compute_wots_len1<n, w>();
  constexpr size_t len2 = compute_wots_len2<n, w, len1>();
  return len1 + len2;
}

// Compile-time compute message digest length ( = m ) in bytes, following
// section 6.1 of SPHINCS+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<uint32_t h, uint32_t d, uint32_t a, uint32_t k>
static inline constexpr size_t
compute_sphincs_md_len()
{
  constexpr uint32_t t0 = (k * a + 7) >> 3;
  constexpr uint32_t t1 = (h - (h / d) + 7) >> 3;
  constexpr uint32_t t2 = ((h / d) + 7) >> 3;

  return static_cast<size_t>(t0 + t1 + t2);
}

// Compile-time compute length of FORS sigature, following section 5.5 of
// the specification https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n, uint32_t a, uint32_t k>
static inline constexpr size_t
compute_fors_sig_len()
{
  return n * static_cast<size_t>(k * (a + 1u));
}

// Compile-time compute length of HyperTree signature, following section 4.2.3
// of the specification https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<uint32_t h, uint32_t d, size_t n, size_t w>
static inline constexpr size_t
compute_ht_sig_len()
{
  constexpr size_t len = compute_wots_len<n, w>();
  return static_cast<size_t>(h + d * len) * n;
}

// Compile-time compute length of SPHINCS+ public key; see figure 14 of the
// specification https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n>
static inline constexpr size_t
get_sphincs_pkey_len()
{
  return n + n;
}

// Compile-time compute length of SPHINCS+ secret key; see figure 14 of the
// specification https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n>
static inline constexpr size_t
get_sphincs_skey_len()
{
  return n + n + get_sphincs_pkey_len<n>();
}

// Compile-time compute length of SPHINCS+ signature, see figure 15 of the
// specification https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n, uint32_t h, uint32_t d, uint32_t a, uint32_t k, size_t w>
static inline constexpr size_t
get_sphincs_sig_len()
{
  return n + compute_fors_sig_len<n, a, k>() + compute_ht_sig_len<h, d, n, w>();
}

// Given a 32 -bit word, this routine extracts out each byte from that word and
// places them in a big endian byte array.
static inline void
to_be_bytes(const uint32_t word, uint8_t* const bytes)
{
  bytes[0] = static_cast<uint8_t>(word >> 24);
  bytes[1] = static_cast<uint8_t>(word >> 16);
  bytes[2] = static_cast<uint8_t>(word >> 8);
  bytes[3] = static_cast<uint8_t>(word >> 0);
}

// Given a byte array of length 4, this routine converts it to a big endian 32
// -bit word.
static inline uint32_t
from_be_bytes(const uint8_t* const bytes)
{
  return (static_cast<uint32_t>(bytes[0]) << 24) | (static_cast<uint32_t>(bytes[1]) << 16) | (static_cast<uint32_t>(bytes[2]) << 8) |
         (static_cast<uint32_t>(bytes[3]) << 0);
}

// Compile-time check to ensure that output length of base-w string is within
// allowed bounds.
//
// See section 2.5 of SPHINCS+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t w, size_t ilen, size_t olen>
static inline constexpr bool
check_olen()
{
  constexpr size_t lgw = log2<w>();
  constexpr size_t max = (8 * ilen) / lgw;
  return olen <= max;
}

// Given an unsigned integer ( whose type can be templated ), this routine
// returns a big endian byte array of length y.
//
// Two edge cases, to keep in mind,
//
// - If y > sizeof(x), then extra bytes will be zerod.
// - If y < sizeof(x) and x requires > y -bytes to be represented properly some
// bits may be lost.
//
// See section 2.4 of SPHINCS+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<typename T, size_t y>
static inline std::array<uint8_t, y>
to_byte(const T x)
  requires(std::is_unsigned_v<T>)
{
  constexpr size_t blen = sizeof(T);
  constexpr bool flg = y > blen;
  constexpr size_t br[]{ 0, y - blen };
  constexpr size_t start = br[flg];

  std::array<uint8_t, y> res{};

  for (size_t i = start; i < y; i++) {
    const size_t shr = ((y - 1) - i) << 3;
    res[i] = static_cast<uint8_t>(x >> shr);
  }

  return res;
}

// Given a byte array of length ilen -bytes, this routine computes output array
// of length olen s.t. each element of output array ∈ [0, w), w ∈ {4, 16, 256}
//
// See algorithm 1 in section 2.5 of SPHINCS+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t w, size_t ilen, size_t olen>
static inline void
base_w(const uint8_t* const __restrict in, uint8_t* const __restrict out)
  requires(check_olen<w, ilen, olen>())
{
  constexpr size_t lgw = log2<w>();
  constexpr uint8_t mask = w - 1;

  if constexpr (w == 4) {
    for (size_t i = 0; i < olen; i++) {
      const size_t off = i >> 2;
      const size_t boff = (3ul - (i & 3ul)) * lgw;

      out[i] = (in[off] >> boff) & mask;
    }
  } else if constexpr (w == 16) {
    for (size_t i = 0; i < olen; i++) {
      const size_t off = i >> 1;
      const size_t boff = (1ul - (i & 1ul)) * lgw;

      out[i] = (in[off] >> boff) & mask;
    }
  } else {
    std::memcpy(out, in, olen);
  }
}

// Given a byte array, this routine extracts out a -many contiguous bits,
// starting from bit index `frm_idx` and ending at `to_idx` ( inclusive ) while
// enumerating bits inside each byte from least significant bits to most
// significant bits ( i.e. a = to_idx - frm_idx + 1 ). Extracted a -many
// contiguous bits are now interpreted as an 32 -bit unsigned integer
// ∈ [0, t) | a <= 32 and t = 2^a
static inline uint32_t
extract_contiguous_bits_as_u32(const uint8_t* const __restrict msg, // byte array to extract bits from
                               const uint32_t frm_idx,              // starting bit index
                               const uint32_t to_idx                // ending bit index
)
{
  constexpr uint8_t mask = 0b1;

  assert(to_idx > frm_idx);
  const uint32_t bits = to_idx - frm_idx + 1u;
  assert(bits <= 32u);

  uint32_t res = 0u;

  for (uint32_t i = frm_idx; i <= to_idx; i++) {
    const uint32_t byte_off = i >> 3;
    const uint32_t bit_off = i & 7u;

    const uint8_t bit = (msg[byte_off] >> bit_off) & mask;
    res |= static_cast<uint32_t>(bit) << (i - frm_idx);
  }

  return res;
}

// Given a bytearray of length N, this function converts it to human readable
// hex string of length N << 1 | N >= 0
static inline const std::string
to_hex(const uint8_t* const bytes, const size_t len)
{
  std::stringstream ss;
  ss << std::hex;

  for (size_t i = 0; i < len; i++) {
    ss << std::setw(2) << std::setfill('0') << static_cast<uint32_t>(bytes[i]);
  }

  return ss.str();
}

// Given a hex encoded string of length 2*L, this routine can be used for
// parsing it as a byte array of length L.
static inline std::vector<uint8_t>
from_hex(std::string_view hex)
{
  const size_t hlen = hex.length();
  assert(hlen % 2 == 0);

  const size_t blen = hlen / 2;
  std::vector<uint8_t> res(blen, 0);

  for (size_t i = 0; i < blen; i++) {
    const size_t off = i * 2;

    uint8_t byte = 0;
    auto sstr = hex.substr(off, 2);
    std::from_chars(sstr.data(), sstr.data() + 2, byte, 16);

    res[i] = byte;
  }

  return res;
}

// Generates N -many random values of type T | N >= 0
template<typename T>
static inline void
random_data(T* const data, const size_t len)
  requires(std::is_unsigned_v<T>)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<T> dis;

  for (size_t i = 0; i < len; i++) {
    data[i] = dis(gen);
  }
}

}
