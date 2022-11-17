#pragma once
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <sstream>

// Utility functions for SPHINCS+
namespace sphincs_utils {

// Compile-time check to ensure that `w` parameter takes only allowed values.
//
// See Winternitz Parameter point in section 3.1 of
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
inline static consteval bool
check_w(const size_t w)
{
  return (w == 4) || (w == 16) || (w == 256);
}

// Given a 32 -bit word, this routine extracts out each byte from that word and
// places them in a big endian byte array.
inline static void
to_be_bytes(const uint32_t word, uint8_t* const bytes)
{
  bytes[0] = static_cast<uint8_t>(word >> 24);
  bytes[1] = static_cast<uint8_t>(word >> 16);
  bytes[2] = static_cast<uint8_t>(word >> 8);
  bytes[3] = static_cast<uint8_t>(word >> 0);
}

// Given a byte array of length 4, this routine converts it to a big endian 32
// -bit word.
inline static uint32_t
from_be_bytes(const uint8_t* const bytes)
{
  return (static_cast<uint32_t>(bytes[0]) << 24) |
         (static_cast<uint32_t>(bytes[1]) << 16) |
         (static_cast<uint32_t>(bytes[2]) << 8) |
         (static_cast<uint32_t>(bytes[3]) << 0);
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

}
