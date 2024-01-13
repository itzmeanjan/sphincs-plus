#pragma once
#include "address.hpp"
#include "xmss.hpp"

// FORS: Forest of Random Subsets
namespace sphincs_plus_fors {

// Computes a n -bytes FORS private key value, living at specified index `idx`,
// given n -bytes public key seed, n -bytes private key seed and 32 -bytes FORS
// tree address, encoding the position of the FORS keypair within SPHINCS+,
// following algorithm 14, as described in section 5.2 of specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n>
static inline void
skgen(std::span<const uint8_t, n> pk_seed,
      std::span<const uint8_t, n> sk_seed,
      const sphincs_plus_adrs::fors_tree_t adrs,
      const uint32_t idx,
      std::span<uint8_t, n> skey)
{
  sphincs_plus_adrs::fors_prf_t prf_adrs{ adrs };

  prf_adrs.set_type(sphincs_plus_adrs::type_t::FORS_PRF);
  prf_adrs.set_keypair_address(adrs.get_keypair_address());
  prf_adrs.set_tree_index(idx);

  sphincs_plus_hashing::prf<n>(pk_seed, sk_seed, prf_adrs.data, skey);
}

// Computes n -bytes root node of a subtree ( in the FORS tree ) of height
// `n_height` with leftmost leaf node being hash of the private key value at
// index `s_idx`, using algorithm 15, described in section 5.3 of SPHINCS+
// specification https://sphincs.org/data/sphincs+-r3.1-specification.pdf
//
// Note, this algorithm is just a slight modification of algorithm 7, described
// in section 4.1.3 of the specification, which describes treehash algorithm,
// for computing n -bytes root node of subtree of height z, in main (binary)
// Merkle Tree.
template<size_t n, sphincs_plus_hashing::variant v>
static inline void
treehash(std::span<const uint8_t, n> sk_seed,
         const uint32_t s_idx,
         const uint32_t n_height,
         std::span<const uint8_t, n> pk_seed,
         sphincs_plus_adrs::fors_tree_t adrs,
         std::span<uint8_t, n> root)
{
  // # -of leafs in the subtree
  const uint32_t leaf_cnt = 1u << n_height;

  // Stack holding at max `n_height` many intermediate nodes of FORS tree
  std::stack<sphincs_plus_xmss::node_t<n>> stack;
  std::array<uint8_t, n> sk_val{};

  for (uint32_t i = 0; i < leaf_cnt; i++) {
    skgen<n>(pk_seed, sk_seed, adrs, s_idx + i, sk_val);

    adrs.set_tree_height(0u);
    adrs.set_tree_index(s_idx + i);

    sphincs_plus_xmss::node_t<n> node{};
    sphincs_plus_hashing::f<n, v>(pk_seed, adrs.data, sk_val, node.data);
    node.height = 1u;

    adrs.set_tree_height(1u);
    adrs.set_tree_index(s_idx + i);

    // Two consecutive nodes, each of n -bytes width
    //
    // Used for computing parent node of binary FORS Tree, from two children
    std::array<uint8_t, n + n> c_nodes{};
    auto _c_nodes = std::span(c_nodes);

    while (!stack.empty()) {
      const auto top = stack.top();
      if (top.height != node.height) {
        break;
      }

      adrs.set_tree_index((adrs.get_tree_index() - 1u) >> 1);

      std::copy(top.data.begin(), top.data.end(), _c_nodes.template subspan<0, n>().begin());
      std::copy(node.data.begin(), node.data.end(), _c_nodes.template subspan<n, n>().begin());

      sphincs_plus_hashing::h<n, v>(pk_seed, adrs.data, _c_nodes, node.data);
      node.height = adrs.get_tree_height() + 1u;

      adrs.set_tree_height(adrs.get_tree_height() + 1u);
      stack.pop();
    }

    stack.push(node);
  }

  const sphincs_plus_xmss::node_t<n> top = stack.top();
  std::copy(top.data.begin(), top.data.end(), root.begin());
  stack.pop(); // Drop root of FORS subtree, stack is empty now.
}

// Computes a n -bytes FORS public key, given n -bytes secret key seed, n -bytes
// public key seed and 32 -bytes FORS address, encoding the position of FORS
// instance within SPHINCS+, using algorithm 16, as described in section 5.4 of
// the specification https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n, uint32_t a, uint32_t k, sphincs_plus_hashing::variant v>
static inline void
pkgen(std::span<const uint8_t, n> sk_seed, std::span<const uint8_t, n> pk_seed, const sphincs_plus_adrs::fors_tree_t adrs, std::span<uint8_t, n> pkey)
{
  constexpr uint32_t t = 1u << a; // # -of leaves in FORS subtree
  std::array<uint8_t, k * n> roots{};
  auto _roots = std::span(roots);

  for (uint32_t i = 0; i < k; i++) {
    const size_t off = static_cast<size_t>(i) * n;
    treehash<n, v>(sk_seed, i * t, a, pk_seed, adrs, std::span<uint8_t, n>(_roots.subspan(off, n)));
  }

  sphincs_plus_adrs::fors_roots_t roots_adrs{ adrs };

  roots_adrs.set_type(sphincs_plus_adrs::type_t::FORS_ROOTS);
  roots_adrs.set_keypair_address(adrs.get_keypair_address());

  sphincs_plus_hashing::t_l<n, k, v>(pk_seed, roots_adrs.data, roots, pkey);
}

// Computes k * n * (a + 1) -bytes FORS signature over message of ⌈(k * a) / 8⌉
// -bytes ( i.e. more specifically k * a -bit string ), given n -bytes secret
// key seed, n -bytes public key seed and 32 -bytes address, encapsulating
// position of FORS instance within SPHINCS+ structure, using algorithm 17, as
// described in section 5.5 of SPHINCS+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n, uint32_t a, uint32_t k, sphincs_plus_hashing::variant v>
static inline void
sign(std::span<const uint8_t, ((k * a) + 7) / 8> msg,
     std::span<const uint8_t, n> sk_seed,
     std::span<const uint8_t, n> pk_seed,
     const sphincs_plus_adrs::fors_tree_t adrs,
     std::span<uint8_t, (k * n * (a + 1))> sig)
{
  constexpr uint32_t t = 1u << a; // # -of leaves in FORS subtree

  constexpr size_t skey_val_len = n;
  constexpr size_t auth_path_len = static_cast<size_t>(a) * n;
  constexpr size_t sig_elm_len = skey_val_len + auth_path_len;

  for (uint32_t i = 0; i < k; i++) {
    const size_t frm = i * a;
    const size_t to = (i + 1) * a - 1;

    const uint32_t idx = sphincs_plus_utils::extract_contiguous_bits_as_u32(msg, frm, to);

    const size_t off0 = i * sig_elm_len;
    const size_t off1 = off0 + skey_val_len;

    skgen<n>(pk_seed, sk_seed, adrs, i * t + idx, std::span<uint8_t, n>(sig.subspan(off0, skey_val_len)));

    for (uint32_t j = 0; j < a; j++) {
      const size_t off2 = off1 + j * n;

      const uint32_t s = (idx >> j) ^ 1u;
      treehash<n, v>(sk_seed, i * t + (s << j), j, pk_seed, adrs, std::span<uint8_t, n>(sig.subspan(off2, n)));
    }
  }
}

// Computes n -bytes FORS public key, from k * n * (a + 1) -bytes FORS
// signature and ⌈(k * a) / 8⌉ -bytes message ( i.e. actually k * a -bit array )
// while using n -bytes public key seed and 32 -bytes address, encapsulating
// address of FORS instance within SPHINCS+ virtual structure, following
// algorithm 18, as described in section
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n, uint32_t a, uint32_t k, sphincs_plus_hashing::variant v>
static inline void
pk_from_sig(std::span<const uint8_t, (k * n * (a + 1))> sig,
            std::span<const uint8_t, ((k * a) + 7) / 8> msg,
            std::span<const uint8_t, n> pk_seed,
            sphincs_plus_adrs::fors_tree_t adrs,
            std::span<uint8_t, n> pkey)
{
  constexpr uint32_t t = 1u << a; // # -of leaves in FORS subtree

  constexpr size_t skey_val_len = n;
  constexpr size_t auth_path_len = static_cast<size_t>(a) * n;
  constexpr size_t sig_elm_len = skey_val_len + auth_path_len;

  // A pair of sibling nodes, each of n -bytes
  std::array<uint8_t, n + n> c_nodes{};
  std::array<uint8_t, n> tmp{};
  std::array<uint8_t, n * k> roots{};

  auto _c_nodes = std::span(c_nodes);
  auto _roots = std::span(roots);

  for (uint32_t i = 0; i < k; i++) {
    const size_t frm = i * a;
    const size_t to = (i + 1) * a - 1;

    const uint32_t idx = sphincs_plus_utils::extract_contiguous_bits_as_u32(msg, frm, to);

    const size_t off0 = i * sig_elm_len;     // next n -bytes secret key value
    const size_t off1 = off0 + skey_val_len; // next a * n -bytes auth path

    adrs.set_tree_height(0u);
    adrs.set_tree_index(i * t + idx);

    sphincs_plus_hashing::f<n, v>(pk_seed, adrs.data, std::span<const uint8_t, n>(sig.subspan(off0, skey_val_len)), _c_nodes.template subspan<0, n>());

    for (uint32_t j = 0; j < a; j++) {
      const size_t off2 = off1 + j * n;
      adrs.set_tree_height(j + 1u);

      const bool flg = static_cast<bool>((idx >> j) & 1u);
      if (!flg) {
        adrs.set_tree_index(adrs.get_tree_index() >> 1);

        auto _sig = std::span<const uint8_t, n>(sig.subspan(off2, n));
        std::copy(_sig.begin(), _sig.end(), _c_nodes.template subspan<n, n>().begin());

        sphincs_plus_hashing::h<n, v>(pk_seed, adrs.data, _c_nodes, tmp);

        std::copy(tmp.begin(), tmp.end(), _c_nodes.template subspan<n, n>().begin());
      } else {
        adrs.set_tree_index((adrs.get_tree_index() - 1u) >> 1);

        auto __c_nodes = _c_nodes.template subspan<0, n>();
        std::copy(__c_nodes.begin(), __c_nodes.end(), _c_nodes.template subspan<n, n>().begin());

        auto _sig = std::span<const uint8_t, n>(sig.subspan(off2, n));
        std::copy(_sig.begin(), _sig.end(), _c_nodes.template subspan<0, n>().begin());

        sphincs_plus_hashing::h<n, v>(pk_seed, adrs.data, _c_nodes, tmp);

        std::copy(tmp.begin(), tmp.end(), _c_nodes.template subspan<n, n>().begin());
      }

      auto __c_nodes = _c_nodes.template subspan<n, n>();
      std::copy(__c_nodes.begin(), __c_nodes.end(), _c_nodes.template subspan<0, n>().begin());
    }

    const size_t off2 = i * n; // next n -bytes i -th FORS tree root
    auto __c_nodes = _c_nodes.template subspan<0, n>();
    std::copy(__c_nodes.begin(), __c_nodes.end(), std::span<uint8_t, n>(_roots.subspan(off2, n)).begin());
  }

  sphincs_plus_adrs::fors_roots_t roots_adrs{ adrs };

  roots_adrs.set_type(sphincs_plus_adrs::type_t::FORS_ROOTS);
  roots_adrs.set_keypair_address(adrs.get_keypair_address());

  sphincs_plus_hashing::t_l<n, k, v>(pk_seed, roots_adrs.data, roots, pkey);
}

}
