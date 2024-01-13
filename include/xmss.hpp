#pragma once
#include "wots.hpp"
#include <stack>

// Fixed Input-Length XMSS, used in SPHINCS+
namespace sphincs_plus_xmss {

// Internal/ Leaf Node of Main Merkle Tree, where each node is n -bytes wide
//
// Read section 4.1.3 of SPHINCS+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf to understand why
// height of node needs to be kept track of.
template<size_t n>
struct node_t
{
  std::array<uint8_t, n> data{};
  uint32_t height = 0u;

  constexpr node_t<n>() = default;
};

// Computes n -bytes root node of a subtree of height `n_height` with leftmost
// leaf node being WOTS+ compressed public key at index `s_idx`, using algorithm
// 7, described in section 4.1.3 of SPHINCS+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<size_t n, size_t w, sphincs_plus_hashing::variant v>
static inline void
treehash(std::span<const uint8_t, n> sk_seed,
         const uint32_t s_idx,
         const uint32_t n_height,
         std::span<const uint8_t, n> pk_seed,
         const sphincs_plus_adrs::adrs_t adrs,
         std::span<uint8_t, n> root)
{
  // # -of leafs in the subtree
  const uint32_t leaf_cnt = 1u << n_height;
  // Stack which will hold at max `n_height` many intermediate nodes
  std::stack<node_t<n>> stack;

  for (uint32_t i = 0; i < leaf_cnt; i++) {
    sphincs_plus_adrs::wots_hash_t hash_adrs{ adrs };

    hash_adrs.set_type(sphincs_plus_adrs::type_t::WOTS_HASH);
    hash_adrs.set_keypair_address(s_idx + i);

    node_t<n> node{};
    sphincs_plus_wots::pkgen<n, w, v>(sk_seed, pk_seed, hash_adrs, node.data);
    node.height = 1u;

    sphincs_plus_adrs::tree_t tree_adrs{ adrs };

    tree_adrs.set_type(sphincs_plus_adrs::type_t::TREE);
    tree_adrs.set_tree_height(1u);
    tree_adrs.set_tree_index(s_idx + i);

    // Two consecutive nodes, each of n -bytes width
    //
    // Used for computing parent node of binary Merkle Tree, from two children
    std::array<uint8_t, n + n> c_nodes{};
    auto _c_nodes = std::span(c_nodes);

    while (!stack.empty()) {
      const auto top = stack.top();
      if (top.height != node.height) {
        break;
      }

      tree_adrs.set_tree_index((tree_adrs.get_tree_index() - 1u) >> 1);

      std::copy(top.data.begin(), top.data.end(), _c_nodes.template subspan<0, n>().begin());
      std::copy(node.data.begin(), node.data.end(), _c_nodes.template subspan<n, n>().begin());

      sphincs_plus_hashing::h<n, v>(pk_seed, tree_adrs.data, _c_nodes, node.data);
      node.height = tree_adrs.get_tree_height() + 1u;

      tree_adrs.set_tree_height(tree_adrs.get_tree_height() + 1u);
      stack.pop();
    }

    stack.push(node);
  }

  const node_t<n> top = stack.top();
  std::copy(top.data.begin(), top.data.end(), root.begin());
  stack.pop(); // Drop root of XMSS Tree, stack is empty now.
}

// Computes XMSS public key, which is the n -bytes root of the binary hash tree,
// of height h, using algorithm 8, described in section 4.1.4 of SPHINCS+
// specification https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<uint32_t h, size_t n, size_t w, sphincs_plus_hashing::variant v>
static inline void
pkgen(std::span<const uint8_t, n> sk_seed, std::span<const uint8_t, n> pk_seed, const sphincs_plus_adrs::adrs_t adrs, std::span<uint8_t, n> pkey)
{
  treehash<n, w, v>(sk_seed, 0u, h, pk_seed, adrs, pkey);
}

// Computes (len * n + h * n) -bytes XMSS signature, for n -bytes message, using
// n-bytes secret key seed, 4 -bytes WOTS+ keypair index, n -bytes public key
// seed & 32 -bytes address, encapsulating which XMSS instance we're using,
// following algorithm 9, described in section 4.1.6 of SPHINCS+ specification.
//
// For understanding XMSS signature structure, I suggest you look at figure 10
// and read section 4.1.5 of SPHINCS+ specification.
//
// Find the specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<uint32_t h, size_t n, size_t w, sphincs_plus_hashing::variant v>
static inline void
sign(std::span<const uint8_t, n> msg,
     std::span<const uint8_t, n> sk_seed,
     const uint32_t idx,
     std::span<const uint8_t, n> pk_seed,
     const sphincs_plus_adrs::adrs_t adrs,
     std::span<uint8_t, sphincs_plus_utils::compute_wots_len<n, w>() * n + h * n> sig)
{
  constexpr size_t len = sphincs_plus_utils::compute_wots_len<n, w>();
  constexpr size_t off0 = 0ul;
  constexpr size_t off1 = off0 + len * n;

  for (uint32_t j = 0; j < h; j++) {
    const size_t off2 = j * n;
    const size_t off = off1 + off2;

    const uint32_t k = (idx >> j) ^ 1u;
    treehash<n, w, v>(sk_seed, k << j, j, pk_seed, adrs, std::span<uint8_t, n>(sig.subspan(off, n)));
  }

  sphincs_plus_adrs::wots_hash_t wots_adrs{ adrs };

  wots_adrs.set_type(sphincs_plus_adrs::type_t::WOTS_HASH);
  wots_adrs.set_keypair_address(idx);

  sphincs_plus_wots::sign<n, w, v>(msg, sk_seed, pk_seed, wots_adrs, sig.template subspan<off0, off1 - off0>());
}

// Computes n -bytes XMSS public key from (len * n + h * n) -bytes XMSS
// signature and n -bytes message, when n -bytes public key seed, 4 -bytes WOTS+
// keypair address and 32 -bytes address, encapsulating XMSS instance, is
// provided.
//
// It uses algorithm 10 for implicit XMSS signature verification, which is
// described in section 4.1.7 of the specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<uint32_t h, size_t n, size_t w, sphincs_plus_hashing::variant v>
static inline void
pk_from_sig(const uint32_t idx,
            std::span<const uint8_t, sphincs_plus_utils::compute_wots_len<n, w>() * n + h * n> sig,
            std::span<const uint8_t, n> msg,
            std::span<const uint8_t, n> pk_seed,
            const sphincs_plus_adrs::adrs_t adrs,
            std::span<uint8_t, n> pkey)
{
  constexpr size_t len = sphincs_plus_utils::compute_wots_len<n, w>();
  constexpr size_t soff = len * n;

  // Two consecutive nodes, each of n -bytes width
  //
  // Used for computing parent node of binary Merkle Tree, from two children
  std::array<uint8_t, n + n> c_nodes{};
  auto _c_nodes = std::span(c_nodes);

  // Single node, used for temporarily storing computed n -bytes digest
  std::array<uint8_t, n> tmp{};

  sphincs_plus_adrs::wots_hash_t hash_adrs{ adrs };

  hash_adrs.set_type(sphincs_plus_adrs::type_t::WOTS_HASH);
  hash_adrs.set_keypair_address(idx);

  sphincs_plus_wots::pk_from_sig<n, w, v>(sig.template subspan<0, soff>(), msg, pk_seed, hash_adrs, _c_nodes.template subspan<0, n>());

  sphincs_plus_adrs::tree_t tree_adrs{ adrs };

  tree_adrs.set_type(sphincs_plus_adrs::type_t::TREE);
  tree_adrs.set_tree_index(idx);

  for (uint32_t k = 0; k < h; k++) {
    const size_t aoff = k * n;
    const size_t off = soff + aoff;

    tree_adrs.set_tree_height(k + 1u);

    const bool flg = static_cast<bool>((idx >> k) & 1u);
    if (!flg) {
      tree_adrs.set_tree_index(tree_adrs.get_tree_index() >> 1);

      auto _sig = std::span<const uint8_t, n>(sig.subspan(off, n));
      std::copy(_sig.begin(), _sig.end(), _c_nodes.template subspan<n, n>().begin());

      sphincs_plus_hashing::h<n, v>(pk_seed, tree_adrs.data, _c_nodes, tmp);

      std::copy(tmp.begin(), tmp.end(), _c_nodes.template subspan<n, n>().begin());
    } else {
      tree_adrs.set_tree_index((tree_adrs.get_tree_index() - 1u) >> 1);

      auto __c_nodes = _c_nodes.template subspan<0, n>();
      std::copy(__c_nodes.begin(), __c_nodes.end(), _c_nodes.template subspan<n, n>().begin());

      auto _sig = std::span<const uint8_t, n>(sig.subspan(off, n));
      std::copy(_sig.begin(), _sig.end(), _c_nodes.template subspan<0, n>().begin());

      sphincs_plus_hashing::h<n, v>(pk_seed, tree_adrs.data, _c_nodes, tmp);

      std::copy(tmp.begin(), tmp.end(), _c_nodes.template subspan<n, n>().begin());
    }

    auto __c_nodes = _c_nodes.template subspan<n, n>();
    std::copy(__c_nodes.begin(), __c_nodes.end(), _c_nodes.template subspan<0, n>().begin());
  }

  auto __c_nodes = _c_nodes.template subspan<0, n>();
  std::copy(__c_nodes.begin(), __c_nodes.end(), pkey.begin());
}

}
