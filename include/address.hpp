#pragma once
#include "utils.hpp"

// Hash function address scheme for SPHINCS+
namespace sphincs_plus_adrs {

// Allowed SPHINCS+ address types
//
// See bottom of page 13 of specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
enum class type_t : uint32_t
{
  WOTS_HASH = 0u,
  WOTS_PK,
  TREE,
  FORS_TREE,
  FORS_ROOTS,
  WOTS_PRF,
  FORS_PRF
};

// Compile-time evaluable byte offsets for SPHINCS+ ADRS structure
static constexpr size_t WORD_BYTE_WIDTH = sizeof(uint32_t);
static constexpr size_t WORD0_BEGIN = 0;
static constexpr size_t WORD1_BEGIN = WORD0_BEGIN + WORD_BYTE_WIDTH;
static constexpr size_t WORD2_BEGIN = WORD1_BEGIN + WORD_BYTE_WIDTH;
static constexpr size_t WORD3_BEGIN = WORD2_BEGIN + WORD_BYTE_WIDTH;
static constexpr size_t WORD4_BEGIN = WORD3_BEGIN + WORD_BYTE_WIDTH;
static constexpr size_t WORD5_BEGIN = WORD4_BEGIN + WORD_BYTE_WIDTH;
static constexpr size_t WORD6_BEGIN = WORD5_BEGIN + WORD_BYTE_WIDTH;
static constexpr size_t WORD7_BEGIN = WORD6_BEGIN + WORD_BYTE_WIDTH;
static constexpr size_t ADRS_BYTE_WIDTH = WORD7_BEGIN + WORD_BYTE_WIDTH;

// Common structure of SPHINCS+ ADRS ( 32 -bytes wide ), with each word being 4 -bytes wide.
struct adrs_t
{
  std::array<uint8_t, ADRS_BYTE_WIDTH> data{};

  adrs_t() = default;
  adrs_t(const adrs_t& adrs) { std::copy(adrs.data.begin(), adrs.data.end(), data.begin()); }

  // Returns 1 -word wide layer address
  inline uint32_t get_layer_address() const
  {
    auto _data = std::span(data);
    return sphincs_plus_utils::from_be_bytes(_data.subspan<WORD0_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Only sets 1 -word wide layer address
  inline void set_layer_address(const uint32_t address)
  {
    auto _data = std::span(data);
    sphincs_plus_utils::to_be_bytes(address, _data.subspan<WORD0_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Returns 3 -word wide tree address
  inline std::array<uint32_t, 3> get_tree_address() const
  {
    auto _data = std::span(data);
    return { sphincs_plus_utils::from_be_bytes(_data.subspan<WORD1_BEGIN, WORD_BYTE_WIDTH>()),
             sphincs_plus_utils::from_be_bytes(_data.subspan<WORD2_BEGIN, WORD_BYTE_WIDTH>()),
             sphincs_plus_utils::from_be_bytes(_data.subspan<WORD3_BEGIN, WORD_BYTE_WIDTH>()) };
  }

  // Only sets 3 -word wide tree address
  inline void set_tree_address(const std::array<uint32_t, 3> address)
  {
    auto _data = std::span(data);

    sphincs_plus_utils::to_be_bytes(address[0], _data.subspan<WORD1_BEGIN, WORD_BYTE_WIDTH>());
    sphincs_plus_utils::to_be_bytes(address[1], _data.subspan<WORD2_BEGIN, WORD_BYTE_WIDTH>());
    sphincs_plus_utils::to_be_bytes(address[2], _data.subspan<WORD3_BEGIN, WORD_BYTE_WIDTH>());
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
    auto _data = std::span(data);
    return static_cast<type_t>(sphincs_plus_utils::from_be_bytes(_data.subspan<WORD4_BEGIN, WORD_BYTE_WIDTH>()));
  }

  // Sets 1 -word wide address type, along with that zeros subsequent 3 -words.
  inline void set_type(const type_t type)
  {
    auto _data = std::span(data);
    sphincs_plus_utils::to_be_bytes(static_cast<uint32_t>(type), _data.subspan<WORD4_BEGIN, WORD_BYTE_WIDTH>());

    auto __data = _data.subspan<WORD5_BEGIN, ADRS_BYTE_WIDTH - WORD5_BEGIN>();
    std::fill(__data.begin(), __data.end(), 0x00);
  }
};

// Structure of WOTS+ Hash Address
struct wots_hash_t : adrs_t
{
  wots_hash_t() = default;
  wots_hash_t(const adrs_t& adrs) { std::copy(adrs.data.begin(), adrs.data.end(), data.begin()); }

  // Returns 1 -word wide keypair address
  inline uint32_t get_keypair_address() const
  {
    auto _data = std::span(data);
    return sphincs_plus_utils::from_be_bytes(_data.subspan<WORD5_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Set 1 -word wide key pair address
  inline void set_keypair_address(const uint32_t address)
  {
    auto _data = std::span(data);
    sphincs_plus_utils::to_be_bytes(address, _data.subspan<WORD5_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Returns 1 -word wide chain address
  inline uint32_t get_chain_address() const
  {
    auto _data = std::span(data);
    return sphincs_plus_utils::from_be_bytes(_data.subspan<WORD6_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Set 1 -word wide chain address
  inline void set_chain_address(const uint32_t address)
  {
    auto _data = std::span(data);
    sphincs_plus_utils::to_be_bytes(address, _data.subspan<WORD6_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Returns 1 -word wide hash address
  inline uint32_t get_hash_address() const
  {
    auto _data = std::span(data);
    return sphincs_plus_utils::from_be_bytes(_data.subspan<WORD7_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Set 1 -word wide hash address
  inline void set_hash_address(const uint32_t address)
  {
    auto _data = std::span(data);
    sphincs_plus_utils::to_be_bytes(address, _data.subspan<WORD7_BEGIN, WORD_BYTE_WIDTH>());
  }
};

// Structure of WOTS+ Public Key Compression Address
struct wots_pk_t : adrs_t
{
  wots_pk_t() = default;
  wots_pk_t(const adrs_t& adrs) { std::copy(adrs.data.begin(), adrs.data.end(), data.begin()); }

  // Returns 1 -word wide keypair address
  inline uint32_t get_keypair_address() const
  {
    auto _data = std::span(data);
    return sphincs_plus_utils::from_be_bytes(_data.subspan<WORD5_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Set 1 -word wide key pair address
  inline void set_keypair_address(const uint32_t address)
  {
    auto _data = std::span(data);
    sphincs_plus_utils::to_be_bytes(address, _data.subspan<WORD5_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Zeros last two words of ADRS structure
  inline void set_padding()
  {
    auto _data = std::span(data);
    auto __data = _data.subspan<WORD6_BEGIN, ADRS_BYTE_WIDTH - WORD6_BEGIN>();
    std::fill(__data.begin(), __data.end(), 0x00);
  }
};

// Structure of Main Hash Tree Address
struct tree_t : adrs_t
{
  tree_t() = default;
  tree_t(const adrs_t& adrs) { std::copy(adrs.data.begin(), adrs.data.end(), data.begin()); }

  // Zeros 1 -word wide padding
  inline void set_padding()
  {
    auto _data = std::span(data);
    auto __data = _data.subspan<WORD5_BEGIN, WORD_BYTE_WIDTH>();
    std::fill(__data.begin(), __data.end(), 0x00);
  }

  // Returns 1 -word wide tree height
  inline uint32_t get_tree_height() const
  {
    auto _data = std::span(data);
    return sphincs_plus_utils::from_be_bytes(_data.subspan<WORD6_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Sets 1 -word wide tree height
  inline void set_tree_height(const uint32_t height)
  {
    auto _data = std::span(data);
    sphincs_plus_utils::to_be_bytes(height, _data.subspan<WORD6_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Returns 1 -word wide tree index
  inline uint32_t get_tree_index() const
  {
    auto _data = std::span(data);
    return sphincs_plus_utils::from_be_bytes(_data.subspan<WORD7_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Sets 1 -word wide tree index
  inline void set_tree_index(const uint32_t idx)
  {
    auto _data = std::span(data);
    sphincs_plus_utils::to_be_bytes(idx, _data.subspan<WORD7_BEGIN, WORD_BYTE_WIDTH>());
  }
};

// Structure of FORS Tree Address
struct fors_tree_t : adrs_t
{
  fors_tree_t() = default;
  fors_tree_t(const adrs_t& adrs) { std::copy(adrs.data.begin(), adrs.data.end(), data.begin()); }

  // Returns 1 -word wide keypair address
  inline uint32_t get_keypair_address() const
  {
    auto _data = std::span(data);
    return sphincs_plus_utils::from_be_bytes(_data.subspan<WORD5_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Sets 1 -word wide key pair address
  inline void set_keypair_address(const uint32_t address)
  {
    auto _data = std::span(data);
    sphincs_plus_utils::to_be_bytes(address, _data.subspan<WORD5_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Returns 1 -word wide tree height
  inline uint32_t get_tree_height() const
  {
    auto _data = std::span(data);
    return sphincs_plus_utils::from_be_bytes(_data.subspan<WORD6_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Sets 1 -word wide tree height
  inline void set_tree_height(const uint32_t height)
  {
    auto _data = std::span(data);
    sphincs_plus_utils::to_be_bytes(height, _data.subspan<WORD6_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Returns 1 -word wide tree index
  inline uint32_t get_tree_index() const
  {
    auto _data = std::span(data);
    return sphincs_plus_utils::from_be_bytes(_data.subspan<WORD7_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Sets 1 -word wide tree index
  inline void set_tree_index(const uint32_t idx)
  {
    auto _data = std::span(data);
    sphincs_plus_utils::to_be_bytes(idx, _data.subspan<WORD7_BEGIN, WORD_BYTE_WIDTH>());
  }
};

// Structure of FORS Tree Root Compression Address
struct fors_roots_t : adrs_t
{
  fors_roots_t() = default;
  fors_roots_t(const adrs_t& adrs) { std::copy(adrs.data.begin(), adrs.data.end(), data.begin()); }

  // Returns 1 -word wide keypair address
  inline uint32_t get_keypair_address() const
  {
    auto _data = std::span(data);
    return sphincs_plus_utils::from_be_bytes(_data.subspan<WORD5_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Sets 1 -word wide key pair address
  inline void set_keypair_address(const uint32_t address)
  {
    auto _data = std::span(data);
    sphincs_plus_utils::to_be_bytes(address, _data.subspan<WORD5_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Zeros last two words of ADRS structure
  inline void set_padding()
  {
    auto _data = std::span(data);
    auto __data = _data.subspan<WORD6_BEGIN, ADRS_BYTE_WIDTH - WORD6_BEGIN>();
    std::fill(__data.begin(), __data.end(), 0x00);
  }
};

// Structure of WOTS+ Key Generation Address
struct wots_prf_t : adrs_t
{
  wots_prf_t() = default;
  wots_prf_t(const adrs_t& adrs) { std::copy(adrs.data.begin(), adrs.data.end(), data.begin()); }

  // Returns 1 -word wide keypair address
  inline uint32_t get_keypair_address() const
  {
    auto _data = std::span(data);
    return sphincs_plus_utils::from_be_bytes(_data.subspan<WORD5_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Set 1 -word wide key pair address
  inline void set_keypair_address(const uint32_t address)
  {
    auto _data = std::span(data);
    sphincs_plus_utils::to_be_bytes(address, _data.subspan<WORD5_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Returns 1 -word wide chain address
  inline uint32_t get_chain_address() const
  {
    auto _data = std::span(data);
    return sphincs_plus_utils::from_be_bytes(_data.subspan<WORD6_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Set 1 -word wide chain address
  inline void set_chain_address(const uint32_t address)
  {
    auto _data = std::span(data);
    sphincs_plus_utils::to_be_bytes(address, _data.subspan<WORD6_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Returns 1 -word wide hash address, which is always set to 0
  inline constexpr uint32_t get_hash_address() const { return 0u; }

  // Zeros 1 -word wide hash address
  inline void set_hash_address()
  {
    auto _data = std::span(data);
    auto __data = _data.subspan<WORD7_BEGIN, ADRS_BYTE_WIDTH - WORD7_BEGIN>();
    std::fill(__data.begin(), __data.end(), 0x00);
  }
};

// Structure of FORS Key Generation Address
struct fors_prf_t : adrs_t
{
  fors_prf_t() = default;
  fors_prf_t(const adrs_t& adrs) { std::copy(adrs.data.begin(), adrs.data.end(), data.begin()); }

  // Returns 1 -word wide keypair address
  inline uint32_t get_keypair_address() const
  {
    auto _data = std::span(data);
    return sphincs_plus_utils::from_be_bytes(_data.subspan<WORD5_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Sets 1 -word wide key pair address
  inline void set_keypair_address(const uint32_t address)
  {
    auto _data = std::span(data);
    sphincs_plus_utils::to_be_bytes(address, _data.subspan<WORD5_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Returns 1 -word wide tree height
  inline constexpr uint32_t get_tree_height() const { return 0u; }

  // Zeros 1 -word wide tree height
  inline void set_tree_height()
  {
    auto _data = std::span(data);
    auto __data = _data.subspan<WORD6_BEGIN, WORD_BYTE_WIDTH>();
    std::fill(__data.begin(), __data.end(), 0x00);
  }

  // Returns 1 -word wide tree index
  inline uint32_t get_tree_index() const
  {
    auto _data = std::span(data);
    return sphincs_plus_utils::from_be_bytes(_data.subspan<WORD7_BEGIN, WORD_BYTE_WIDTH>());
  }

  // Sets 1 -word wide tree index
  inline void set_tree_index(const uint32_t idx)
  {
    auto _data = std::span(data);
    sphincs_plus_utils::to_be_bytes(idx, _data.subspan<WORD7_BEGIN, WORD_BYTE_WIDTH>());
  }
};

}
