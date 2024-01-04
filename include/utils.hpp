#pragma once
#include "hashing.hpp"
#include "params.hpp"
#include <array>
#include <bit>
#include <cassert>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>
#include <type_traits>

// Utility functions for SPHINCS+
namespace sphincs_plus_utils {

// Compile-time compute logarithm base 2 of Winternitz parameter `w` for WOTS+
//
// See section 3.1 of SPHINCS+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t w>
static inline constexpr size_t
log2()
  requires(sphincs_plus_params::check_w(w))
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

// Given a 32 -bit unsigned integer word, this routine swaps byte order and returns byte swapped 32 -bit word.
//
// Collects inspiration from https://github.com/itzmeanjan/ascon/blob/f31d9085c096021cc4da6191398d2bf2d5805be0/include/utils.hpp#L17-L47.
static inline constexpr uint32_t
bswap(const uint32_t a)
{
#if defined __GNUG__ || defined __MINGW64__
  return __builtin_bswap32(a);
#elif defined _MSC_VER
  return _byteswap_uint32(a);
#else
  return ((a & 0x000000ffu) << 24) | ((a & 0x0000ff00u) << 8) | ((a & 0x00ff0000u) >> 8) | ((a & 0xff000000u) >> 24);
#endif
}

// Given a 32 -bit word, this routine extracts out each byte from that word and places them in a big endian byte array.
static inline void
to_be_bytes(const uint32_t word, std::span<uint8_t, sizeof(word)> bytes)
{
  if constexpr (std::endian::native == std::endian::little) {
    const uint32_t swapped = bswap(word);
    std::memcpy(bytes.data(), &swapped, sizeof(swapped));
  } else {
    std::memcpy(bytes.data(), &word, sizeof(word));
  }
}

// Given a byte array of length 4, this routine converts it to a big endian 32 -bit word.
static inline uint32_t
from_be_bytes(std::span<const uint8_t, 4> bytes)
{
  uint32_t res = 0;
  std::memcpy(&res, bytes.data(), bytes.size());

  if constexpr (std::endian::native == std::endian::little) {
    res = bswap(res);
  }

  return res;
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
template<size_t y>
static inline std::array<uint8_t, y>
to_byte(const uint32_t x)
{
  std::array<uint8_t, y> res{};
  auto _res = std::span(res);

  if constexpr (y > sizeof(x)) {
    constexpr size_t start_at = y - sizeof(x);
    to_be_bytes(x, res.template subspan<start_at, sizeof(x)>());
  } else {
    if constexpr (std::endian::native == std::endian::little) {
      const uint32_t swapped = bswap(x);
      std::memcpy(_res.data(), &swapped, _res.size());
    } else {
      std::memcpy(_res.data(), &x, _res.size());
    }
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
base_w(std::span<const uint8_t, ilen> in, std::span<uint8_t, olen> out)
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
    std::copy(in.begin(), in.end(), out.begin());
  }
}

// Given a byte array, this routine extracts out a -many contiguous bits,
// starting from bit index `frm_idx` and ending at `to_idx` ( inclusive ) while
// enumerating bits inside each byte from least significant bits to most
// significant bits ( i.e. a = to_idx - frm_idx + 1 ). Extracted a -many
// contiguous bits are now interpreted as an 32 -bit unsigned integer
// ∈ [0, t) | a <= 32 and t = 2^a
static inline uint32_t
extract_contiguous_bits_as_u32(std::span<const uint8_t> msg, // byte array to extract bits from
                               const uint32_t frm_idx,       // starting bit index
                               const uint32_t to_idx         // ending bit index
)
{
  constexpr uint8_t mask = 0b1;
  uint32_t res = 0u;

  for (uint32_t i = frm_idx; i <= to_idx; i++) {
    const uint32_t byte_off = i >> 3;
    const uint32_t bit_off = i & 7u;

    const uint8_t bit = (msg[byte_off] >> bit_off) & mask;
    res |= static_cast<uint32_t>(bit) << (i - frm_idx);
  }

  return res;
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

}
