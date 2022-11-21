#pragma once
#include "xmss.hpp"

// FORS: Forest of Random Subsets
namespace sphincs_fors {

// Computes a n -bytes FORS private key value, living at specified index `idx`,
// given n -bytes public key seed, n -bytes private key seed and 32 -bytes FORS
// tree address, encoding the position of the FORS keypair within SPHINCS+,
// following algorithm 14, as described in section 5.2 of specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<const size_t n>
inline static void
skgen(const uint8_t* const __restrict pk_seed,
      const uint8_t* const __restrict sk_seed,
      sphincs_adrs::fors_tree_t adrs,
      const uint32_t idx,
      uint8_t* const __restrict skey)
{
  sphincs_adrs::fors_prf_t prf_adrs{ adrs };

  prf_adrs.set_type(sphincs_adrs::type_t::FORS_PRF);
  prf_adrs.set_keypair_address(adrs.get_keypair_address());
  prf_adrs.set_tree_index(idx);

  sphincs_hashing::prf<n>(pk_seed, sk_seed, prf_adrs.data, skey);
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
template<const size_t n, const sphincs_hashing::variant v>
inline static void
treehash(
  const uint8_t* const __restrict sk_seed, // n -bytes secret key seed
  const uint32_t s_idx,                    // 4 -bytes start index
  const uint32_t n_height,                 // 4 -bytes target node height
  const uint8_t* const __restrict pk_seed, // n -bytes public key seed
  const sphincs_adrs::adrs_t adrs, // 32 -bytes address encoding FORS keypair
  uint8_t* const __restrict root   // n -bytes root of subtree of height
                                   // `n_height`
)
{
  // # -of leafs in the subtree
  const uint32_t leaf_cnt = 1u << n_height;
  assert((s_idx % leaf_cnt) == 0);

  // Stack holding at max `n_height` many intermediate nodes of FORS tree
  std::stack<sphincs_xmss::node_t<n>> stack;
  sphincs_adrs::fors_tree_t tree_adrs{ adrs };
  uint8_t sk_val[n]{}; // n -bytes secret key value

  for (uint32_t i = 0; i < leaf_cnt; i++) {
    skgen<n>(pk_seed, sk_seed, tree_adrs, s_idx + i, sk_val);

    sphincs_xmss::node_t<n> node{};
    sphincs_hashing::f<n, v>(pk_seed, tree_adrs.data, sk_val, node.data);
    node.height = 1u;

    tree_adrs.set_tree_height(1u);
    tree_adrs.set_tree_index(s_idx + i);

    // Two consecutive nodes, each of n -bytes width
    //
    // Used for computing parent node of binary FORS Tree, from two children
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

  const sphincs_xmss::node_t<n> top = stack.top();
  std::memcpy(root, top.data, n);
  stack.pop(); // stack must be empty now !
}

}
