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

}
