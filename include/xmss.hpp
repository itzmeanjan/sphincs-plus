#pragma once
#include "wots.hpp"
#include <cassert>
#include <stack>

// Fixed Input-Length XMSS, used in SPHINCS+
namespace sphincs_xmss {

// Internal/ Leaf Node of Main Merkle Tree, where each node is n -bytes wide
//
// Read section 4.1.3 of SPHINCS+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf to understand why
// height of node needs to be kept track of.
template<const size_t n>
struct node_t
{
  uint8_t data[n]{};
  uint32_t height = 0u;
};

// Computes n -bytes root node of a subtree of height `n_height` with leftmost
// leaf node being WOTS+ compressed public key at index `s_idx`, using algorithm
// 7, described in section 4.1.3 of SPHINCS+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<const size_t n, const size_t w, const sphincs_hashing::variant v>
inline static void
treehash(
  const uint8_t* const __restrict sk_seed, // n -bytes secret key seed
  const uint32_t s_idx,                    // 4 -bytes start index
  const uint32_t n_height,                 // 4 -bytes target node height
  const uint8_t* const __restrict pk_seed, // n -bytes public key seed
  const sphincs_adrs::adrs_t adrs, // 32 -bytes address of containing tree
  uint8_t* const __restrict root   // n -bytes root of subtree of height
                                   // `n_height`
)
{
  // # -of leafs in the subtree
  const uint32_t leaf_cnt = 1u << n_height;
  assert((s_idx % leaf_cnt) == 0);

  // Stack which will hold at max `n_height` many intermediate nodes
  std::stack<node_t<n>> stack;

  for (uint32_t i = 0; i < leaf_cnt; i++) {
    sphincs_adrs::wots_hash_t hash_adrs{ adrs };

    hash_adrs.set_type(sphincs_adrs::type_t::WOTS_HASH);
    hash_adrs.set_keypair_address(s_idx + i);

    node_t<n> node{};
    sphincs_wots::pkgen<n, w, v>(sk_seed, pk_seed, hash_adrs, node.data);
    node.height = 1u;

    sphincs_adrs::tree_t tree_adrs{ adrs };

    tree_adrs.set_type(sphincs_adrs::type_t::TREE);
    tree_adrs.set_tree_height(1u);
    tree_adrs.set_tree_index(s_idx + i);

    // Two consecutive nodes, each of n -bytes width
    //
    // Used for computing parent node of binary Merkle Tree, from two children
    uint8_t c_nodes[n + n]{};

    while (!stack.empty()) {
      const auto top = stack.top();
      if (top.height != node.height) {
        break;
      }

      tree_adrs.set_tree_index((tree_adrs.get_tree_index() - 1u) >> 1);

      std::memcpy(c_nodes + 0, top.data, n);
      std::memcpy(c_nodes + n, node.data, n);

      sphincs_hashing::h<n, v>(pk_seed, tree_adrs.data, c_nodes, node.data);
      node.height = tree_adrs.get_tree_height() + 1u;

      tree_adrs.set_tree_height(tree_adrs.get_tree_height() + 1u);
      stack.pop();
    }

    stack.push(node);
  }

  const node_t<n> top = stack.top();
  std::memcpy(root, top.data, n);
  stack.pop(); // stack must be empty now !
}

// Computes XMSS public key, which is the n -bytes root of the binary hash tree,
// of height h, using algorithm 8, described in section 4.1.4 of SPHINCS+
// specification https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<const uint32_t h,
         const size_t n,
         const size_t w,
         const sphincs_hashing::variant v>
inline static void
pkgen(const uint8_t* const __restrict sk_seed, // n -bytes secret key seed
      const uint8_t* const __restrict pk_seed, // n -bytes public key seed
      const sphincs_adrs::adrs_t adrs, // 32 -bytes address of containing tree
      uint8_t* const __restrict pkey   // n -bytes public key
)
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
template<const uint32_t h,
         const size_t n,
         const size_t w,
         const sphincs_hashing::variant v>
inline static void
sign(const uint8_t* const __restrict msg, // n -bytes message ( to be signed )
     const uint8_t* const __restrict sk_seed, // n -bytes secret key seed
     const uint32_t idx,                      // 4 -bytes WOTS+ keypair index
     const uint8_t* const __restrict pk_seed, // n -bytes public key seed
     const sphincs_adrs::adrs_t adrs, // 32 -bytes address of XMSS instance
     uint8_t* const __restrict sig    // (len * n + h * n) -bytes signature
)
{
  constexpr size_t len = sphincs_utils::compute_wots_len<n, w>();
  constexpr size_t off0 = 0ul;
  constexpr size_t off1 = off0 + len * n;

  for (uint32_t j = 0; j < h; j++) {
    const size_t off2 = j * n;
    const size_t off = off1 + off2;

    const uint32_t k = (idx >> j) ^ 1u;
    treehash<n, w, v>(sk_seed, k << j, j, pk_seed, adrs, sig + off);
  }

  sphincs_adrs::wots_hash_t wots_adrs{ adrs };

  wots_adrs.set_type(sphincs_adrs::type_t::WOTS_HASH);
  wots_adrs.set_keypair_address(idx);

  sphincs_wots::sign<n, w, v>(msg, sk_seed, pk_seed, wots_adrs, sig);
}

// Computes n -bytes XMSS public key from (len * n + h * n) -bytes XMSS
// signature and n -bytes message, when n -bytes public key seed, 4 -bytes WOTS+
// keypair address and 32 -bytes address, encapsulating XMSS instance, is
// provided.
//
// It uses algorithm 10 for implicit XMSS signature verification, which is
// described in section 4.1.7 of the specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<const uint32_t h,
         const size_t n,
         const size_t w,
         const sphincs_hashing::variant v>
inline static void
pk_from_sig(
  const uint32_t idx,                      // 4 -bytes WOTS+ keypair index
  const uint8_t* const __restrict sig,     // (len * n + h * n) -bytes signature
  const uint8_t* const __restrict msg,     // n -bytes message
  const uint8_t* const __restrict pk_seed, // n -bytes public key seed
  const sphincs_adrs::adrs_t adrs,         // 32 -bytes address of XMSS instance
  uint8_t* const __restrict pkey           // n -bytes public key
)
{
  constexpr size_t len = sphincs_utils::compute_wots_len<n, w>();
  constexpr size_t soff = len * n;

  // Two consecutive nodes, each of n -bytes width
  //
  // Used for computing parent node of binary Merkle Tree, from two children
  uint8_t c_nodes[n + n]{};

  // Single node, used for temporarily storing computed n -bytes digest
  uint8_t tmp[n]{};

  sphincs_adrs::wots_hash_t hash_adrs{ adrs };

  hash_adrs.set_type(sphincs_adrs::type_t::WOTS_HASH);
  hash_adrs.set_keypair_address(idx);

  sphincs_wots::pk_from_sig<n, w, v>(sig, msg, pk_seed, hash_adrs, c_nodes);

  sphincs_adrs::tree_t tree_adrs{ adrs };

  tree_adrs.set_type(sphincs_adrs::type_t::TREE);
  tree_adrs.set_tree_index(idx);

  for (uint32_t k = 0; k < h; k++) {
    const size_t aoff = k * n;
    const size_t off = soff + aoff;

    tree_adrs.set_tree_height(k + 1u);

    const bool flg = static_cast<bool>((idx >> k) & 1u);
    if (!flg) {
      tree_adrs.set_tree_index(tree_adrs.get_tree_index() >> 1);

      std::memcpy(c_nodes + n, sig + off, n);
      sphincs_hashing::h<n, v>(pk_seed, tree_adrs.data, c_nodes, tmp);
      std::memcpy(c_nodes + n, tmp, n);
    } else {
      tree_adrs.set_tree_index((tree_adrs.get_tree_index() - 1u) >> 1);

      std::memcpy(c_nodes + n, c_nodes + 0, n);
      std::memcpy(c_nodes + 0, sig + off, n);
      sphincs_hashing::h<n, v>(pk_seed, tree_adrs.data, c_nodes, tmp);
      std::memcpy(c_nodes + n, tmp, n);
    }

    std::memcpy(c_nodes + 0, c_nodes + n, n);
  }

  std::memcpy(pkey, c_nodes + 0, n);
}

}
