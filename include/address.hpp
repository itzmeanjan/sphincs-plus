#pragma once
#include "utils.hpp"
#include <array>
#include <cstring>

// Hash function address scheme for SPHINCS+
namespace sphincs_adrs {

// Allowed SPHINCS+ address types
//
// See bottom of page 13 of specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
enum class type_t : uint32_t
{
  WOTS_HASH = 0,
  WOTS_PK = 1,
  TREE = 2,
  FORS_TREE = 3,
  FORS_ROOTS = 4,
  WOTS_PRF = 5,
  FORS_PRF = 6
};

// Common structure of SPHINCS+ ADRS ( 32 -bytes wide ), with each word being
// 4 -bytes.
struct adrs_t
{
  uint8_t data[32]{};

  // Only sets 1 -word wide layer address
  void set_layer_address(const uint32_t address)
  {
    sphincs_utils::to_be_bytes(address, data + 0);
  }

  // Only sets 3 -word wide tree address
  void set_tree_address(const std::array<uint32_t, 3> address)
  {
    sphincs_utils::to_be_bytes(address[0], data + 4);
    sphincs_utils::to_be_bytes(address[1], data + 8);
    sphincs_utils::to_be_bytes(address[2], data + 12);
  }

  // Sets 1 -word wide address type, along with that zeros subsequent 3 -words.
  void set_type(const type_t type)
  {
    sphincs_utils::to_be_bytes(static_cast<uint32_t>(type), data + 16);
    std::memset(data + 20, 0, sizeof(uint32_t) * 3);
  }
};

}
