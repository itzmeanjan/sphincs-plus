#pragma once
#include <cstddef>
#include <cstdint>

// Utility functions for SPHINCS+
namespace sphincs_utils {

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

}
