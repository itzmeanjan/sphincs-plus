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

  adrs_t() {}
  adrs_t(const adrs_t& adrs) { std::memcpy(data, adrs.data, 32); }

  // Returns 1 -word wide layer address
  inline uint32_t get_layer_address() const
  {
    return sphincs_utils::from_be_bytes(data + 0);
  }

  // Only sets 1 -word wide layer address
  inline void set_layer_address(const uint32_t address)
  {
    sphincs_utils::to_be_bytes(address, data + 0);
  }

  // Returns 3 -word wide tree address
  inline std::array<uint32_t, 3> get_tree_address() const
  {
    return { sphincs_utils::from_be_bytes(data + 4),
             sphincs_utils::from_be_bytes(data + 8),
             sphincs_utils::from_be_bytes(data + 12) };
  }

  // Only sets 3 -word wide tree address
  inline void set_tree_address(const std::array<uint32_t, 3> address)
  {
    sphincs_utils::to_be_bytes(address[0], data + 4);
    sphincs_utils::to_be_bytes(address[1], data + 8);
    sphincs_utils::to_be_bytes(address[2], data + 12);
  }

  // Only sets 3 -word wide tree address with a 64 -bit address i.e. higher
  // 32 -bits are zeroed
  inline void set_tree_address(const uint64_t address)
  {
    const uint32_t high = static_cast<uint32_t>(address >> 32);
    const uint32_t low = static_cast<uint32_t>(address >> 0);

    set_tree_address({ 0u, high, low });
  }

  // Returns 1 -word wide address type
  inline type_t get_type() const
  {
    return static_cast<type_t>(sphincs_utils::from_be_bytes(data + 16));
  }

  // Sets 1 -word wide address type, along with that zeros subsequent 3 -words.
  inline void set_type(const type_t type)
  {
    sphincs_utils::to_be_bytes(static_cast<uint32_t>(type), data + 16);
    std::memset(data + 20, 0, sizeof(uint32_t) * 3);
  }
};

// Structure of WOTS+ Hash Address
struct wots_hash_t : adrs_t
{
  wots_hash_t() {}
  wots_hash_t(const adrs_t& adrs) { std::memcpy(data, adrs.data, 32); }

  // Returns 1 -word wide keypair address
  inline uint32_t get_keypair_address() const
  {
    return sphincs_utils::from_be_bytes(data + 20);
  }

  // Set 1 -word wide key pair address
  inline void set_keypair_address(const uint32_t address)
  {
    sphincs_utils::to_be_bytes(address, data + 20);
  }

  // Returns 1 -word wide chain address
  inline uint32_t get_chain_address() const
  {
    return sphincs_utils::from_be_bytes(data + 24);
  }

  // Set 1 -word wide chain address
  inline void set_chain_address(const uint32_t address)
  {
    sphincs_utils::to_be_bytes(address, data + 24);
  }

  // Returns 1 -word wide hash address
  inline uint32_t get_hash_address() const
  {
    return sphincs_utils::from_be_bytes(data + 28);
  }

  // Set 1 -word wide hash address
  inline void set_hash_address(const uint32_t address)
  {
    sphincs_utils::to_be_bytes(address, data + 28);
  }
};

// Structure of WOTS+ Public Key Compression Address
struct wots_pk_t : adrs_t
{
  wots_pk_t() {}
  wots_pk_t(const adrs_t& adrs) { std::memcpy(data, adrs.data, 32); }

  // Returns 1 -word wide keypair address
  inline uint32_t get_keypair_address() const
  {
    return sphincs_utils::from_be_bytes(data + 20);
  }

  // Set 1 -word wide key pair address
  inline void set_keypair_address(const uint32_t address)
  {
    sphincs_utils::to_be_bytes(address, data + 20);
  }

  // Zeros last two words of ADRS structure
  inline void set_padding() { std::memset(data + 24, 0, sizeof(uint32_t) * 2); }
};

// Structure of Main Hash Tree Address
struct tree_t : adrs_t
{
  tree_t() {}
  tree_t(const adrs_t& adrs) { std::memcpy(data, adrs.data, 32); }

  // Zeros 1 -word wide padding
  inline void set_padding() { std::memset(data + 20, 0, sizeof(uint32_t)); }

  // Returns 1 -word wide tree height
  inline uint32_t get_tree_height() const
  {
    return sphincs_utils::from_be_bytes(data + 24);
  }

  // Sets 1 -word wide tree height
  inline void set_tree_height(const uint32_t height)
  {
    sphincs_utils::to_be_bytes(height, data + 24);
  }

  // Returns 1 -word wide tree index
  inline uint32_t get_tree_index() const
  {
    return sphincs_utils::from_be_bytes(data + 28);
  }

  // Sets 1 -word wide tree index
  inline void set_tree_index(const uint32_t idx)
  {
    sphincs_utils::to_be_bytes(idx, data + 28);
  }
};

// Structure of FORS Tree Address
struct fors_tree_t : adrs_t
{
  fors_tree_t() {}
  fors_tree_t(const adrs_t& adrs) { std::memcpy(data, adrs.data, 32); }

  // Returns 1 -word wide keypair address
  inline uint32_t get_keypair_address() const
  {
    return sphincs_utils::from_be_bytes(data + 20);
  }

  // Sets 1 -word wide key pair address
  inline void set_keypair_address(const uint32_t address)
  {
    sphincs_utils::to_be_bytes(address, data + 20);
  }

  // Returns 1 -word wide tree height
  inline uint32_t get_tree_height() const
  {
    return sphincs_utils::from_be_bytes(data + 24);
  }

  // Sets 1 -word wide tree height
  inline void set_tree_height(const uint32_t height)
  {
    sphincs_utils::to_be_bytes(height, data + 24);
  }

  // Returns 1 -word wide tree index
  inline uint32_t get_tree_index() const
  {
    return sphincs_utils::from_be_bytes(data + 28);
  }

  // Sets 1 -word wide tree index
  inline void set_tree_index(const uint32_t idx)
  {
    sphincs_utils::to_be_bytes(idx, data + 28);
  }
};

// Structure of FORS Tree Root Compression Address
struct fors_roots_t : adrs_t
{
  fors_roots_t() {}
  fors_roots_t(const adrs_t& adrs) { std::memcpy(data, adrs.data, 32); }

  // Returns 1 -word wide keypair address
  inline uint32_t get_keypair_address() const
  {
    return sphincs_utils::from_be_bytes(data + 20);
  }

  // Sets 1 -word wide key pair address
  inline void set_keypair_address(const uint32_t address)
  {
    sphincs_utils::to_be_bytes(address, data + 20);
  }

  // Zeros last two words of ADRS structure
  inline void set_padding() { std::memset(data + 24, 0, sizeof(uint32_t) * 2); }
};

// Structure of WOTS+ Key Generation Address
struct wots_prf_t : adrs_t
{
  wots_prf_t() {}
  wots_prf_t(const adrs_t& adrs) { std::memcpy(data, adrs.data, 32); }

  // Returns 1 -word wide keypair address
  inline uint32_t get_keypair_address() const
  {
    return sphincs_utils::from_be_bytes(data + 20);
  }

  // Set 1 -word wide key pair address
  inline void set_keypair_address(const uint32_t address)
  {
    sphincs_utils::to_be_bytes(address, data + 20);
  }

  // Returns 1 -word wide chain address
  inline uint32_t get_chain_address() const
  {
    return sphincs_utils::from_be_bytes(data + 24);
  }

  // Set 1 -word wide chain address
  inline void set_chain_address(const uint32_t address)
  {
    sphincs_utils::to_be_bytes(address, data + 24);
  }

  // Returns 1 -word wide hash address, which is always set to 0
  inline uint32_t get_hash_address() const { return 0u; }

  // Zeros 1 -word wide hash address
  inline void set_hash_address()
  {
    std::memset(data + 28, 0, sizeof(uint32_t));
  }
};

// Structure of FORS Key Generation Address
struct fors_prf_t : adrs_t
{
  fors_prf_t() {}
  fors_prf_t(const adrs_t& adrs) { std::memcpy(data, adrs.data, 32); }

  // Returns 1 -word wide keypair address
  inline uint32_t get_keypair_address() const
  {
    return sphincs_utils::from_be_bytes(data + 20);
  }

  // Sets 1 -word wide key pair address
  inline void set_keypair_address(const uint32_t address)
  {
    sphincs_utils::to_be_bytes(address, data + 20);
  }

  // Returns 1 -word wide tree height
  inline uint32_t get_tree_height() const { return 0u; }

  // Zeros 1 -word wide tree height
  inline void set_tree_height() { std::memset(data + 24, 0, sizeof(uint32_t)); }

  // Returns 1 -word wide tree index
  inline uint32_t get_tree_index() const
  {
    return sphincs_utils::from_be_bytes(data + 28);
  }

  // Sets 1 -word wide tree index
  inline void set_tree_index(const uint32_t idx)
  {
    sphincs_utils::to_be_bytes(idx, data + 28);
  }
};

}
