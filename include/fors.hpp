#pragma once
#include "address.hpp"
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
skgen(const uint8_t* const __restrict pk_seed, // n -bytes public key seed
      const uint8_t* const __restrict sk_seed, // n -bytes secret key seed
      const sphincs_adrs::fors_tree_t adrs,    // 32 -bytes FORS address
      const uint32_t idx,            // 4 -bytes index of FORS private key value
      uint8_t* const __restrict skey // FORS private key value, living at `idx`
)
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
  sphincs_adrs::fors_tree_t adrs, // 32 -bytes address encoding FORS keypair
  uint8_t* const __restrict root  // n -bytes root of subtree of `n_height`
)
{
  // # -of leafs in the subtree
  const uint32_t leaf_cnt = 1u << n_height;
  assert((s_idx % leaf_cnt) == 0);

  // Stack holding at max `n_height` many intermediate nodes of FORS tree
  std::stack<sphincs_xmss::node_t<n>> stack;
  uint8_t sk_val[n]{}; // n -bytes secret key value

  for (uint32_t i = 0; i < leaf_cnt; i++) {
    skgen<n>(pk_seed, sk_seed, adrs, s_idx + i, sk_val);

    adrs.set_tree_height(0u);
    adrs.set_tree_index(s_idx + i);

    sphincs_xmss::node_t<n> node{};
    sphincs_hashing::f<n, v>(pk_seed, adrs.data, sk_val, node.data);
    node.height = 1u;

    adrs.set_tree_height(1u);
    adrs.set_tree_index(s_idx + i);

    // Two consecutive nodes, each of n -bytes width
    //
    // Used for computing parent node of binary FORS Tree, from two children
    uint8_t c_nodes[n + n]{};

    while (!stack.empty()) {
      const auto top = stack.top();
      if (top.height != node.height) {
        break;
      }

      adrs.set_tree_index((adrs.get_tree_index() - 1u) >> 1);

      std::memcpy(c_nodes + 0, top.data, n);
      std::memcpy(c_nodes + n, node.data, n);

      sphincs_hashing::h<n, v>(pk_seed, adrs.data, c_nodes, node.data);
      node.height = adrs.get_tree_height() + 1u;

      adrs.set_tree_height(adrs.get_tree_height() + 1u);
      stack.pop();
    }

    stack.push(node);
  }

  const sphincs_xmss::node_t<n> top = stack.top();
  std::memcpy(root, top.data, n);
  stack.pop(); // stack must be empty now !
}

// Computes a n -bytes FORS public key, given n -bytes secret key seed, n -bytes
// public key seed and 32 -bytes FORS address, encoding the position of FORS
// instance within SPHINCS+, using algorithm 16, as described in section 5.4 of
// the specification https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<const size_t n,
         const uint32_t a,
         const uint32_t k,
         const sphincs_hashing::variant v>
inline static void
pkgen(const uint8_t* const __restrict sk_seed, // n -bytes secret key seed
      const uint8_t* const __restrict pk_seed, // n -bytes public key seed
      const sphincs_adrs::fors_tree_t adrs,    // 32 -bytes FORS address
      uint8_t* const __restrict pkey           // n -bytes public key
)
{
  constexpr uint32_t t = 1u << a; // # -of leaves in FORS subtree
  uint8_t roots[k * n]{};

  for (uint32_t i = 0; i < k; i++) {
    const size_t off = static_cast<size_t>(i) * n;
    treehash<n, v>(sk_seed, i * t, a, pk_seed, adrs, roots + off);
  }

  sphincs_adrs::fors_roots_t roots_adrs{ adrs };

  roots_adrs.set_type(sphincs_adrs::type_t::FORS_ROOTS);
  roots_adrs.set_keypair_address(adrs.get_keypair_address());

  sphincs_hashing::t_l<n, k, v>(pk_seed, roots_adrs.data, roots, pkey);
}

// Computes k * n * (a + 1) -bytes FORS signature over message of ⌈(k * a) / 8⌉
// -bytes ( i.e. more specifically k * a -bit string ), given n -bytes secret
// key seed, n -bytes public key seed and 32 -bytes address, encapsulating
// position of FORS instance within SPHINCS+ structure, using algorithm 17, as
// described in section 5.5 of SPHINCS+ specification
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<const size_t n,
         const uint32_t a,
         const uint32_t k,
         const sphincs_hashing::variant v>
inline static void
sign(const uint8_t* const __restrict msg, // ⌈(k * a) / 8⌉ -bytes message
     const uint8_t* const __restrict sk_seed, // n -bytes secret key seed
     const uint8_t* const __restrict pk_seed, // n -bytes public key seed
     const sphincs_adrs::fors_tree_t adrs,    // 32 -bytes FORS address
     uint8_t* const __restrict sig // k * n * (a + 1) -bytes FORS signature
)
{
  constexpr uint32_t t = 1u << a; // # -of leaves in FORS subtree

  constexpr size_t skey_val_len = n;
  constexpr size_t auth_path_len = static_cast<size_t>(a) * n;
  constexpr size_t sig_elm_len = skey_val_len + auth_path_len;

  for (uint32_t i = 0; i < k; i++) {
    const size_t frm = i * a;
    const size_t to = (i + 1) * a - 1;

    uint32_t idx = sphincs_utils::extract_contiguous_bits_as_u32(msg, frm, to);

    const size_t off0 = i * sig_elm_len;
    const size_t off1 = off0 + skey_val_len;

    skgen<n>(pk_seed, sk_seed, adrs, i * t + idx, sig + off0);

    for (uint32_t j = 0; j < a; j++) {
      const size_t off2 = off1 + j * n;

      const uint32_t s = (idx >> j) ^ 1u;
      treehash<n, v>(sk_seed, i * t + (s << j), j, pk_seed, adrs, sig + off2);
    }
  }
}

// Computes n -bytes FORS public key, from k * n * (a + 1) -bytes FORS
// signature and ⌈(k * a) / 8⌉ -bytes message ( i.e. actually k * a -bit array )
// while using n -bytes public key seed and 32 -bytes address, encapsulating
// address of FORS instance within SPHINCS+ virtual structure, following
// algorithm 18, as described in section
// https://sphincs.org/data/sphincs+-r3.1-specification.pdf
template<const size_t n,
         const uint32_t a,
         const uint32_t k,
         const sphincs_hashing::variant v>
inline static void
pk_from_sig(
  const uint8_t* const __restrict sig, // k * n * (a + 1) -bytes FORS signature
  const uint8_t* const __restrict msg, // ⌈(k * a) / 8⌉ -bytes message
  const uint8_t* const __restrict pk_seed, // n -bytes public key seed
  sphincs_adrs::fors_tree_t adrs,          // 32 -bytes FORS address
  uint8_t* const __restrict pkey           // n -bytes FORS public key
)
{
  constexpr uint32_t t = 1u << a; // # -of leaves in FORS subtree

  constexpr size_t skey_val_len = n;
  constexpr size_t auth_path_len = static_cast<size_t>(a) * n;
  constexpr size_t sig_elm_len = skey_val_len + auth_path_len;

  uint8_t c_nodes[n + n]{}; // a pair of sibling nodes, each of n -bytes
  uint8_t tmp[n]{};
  uint8_t roots[n * k]{};

  for (uint32_t i = 0; i < k; i++) {
    const size_t frm = i * a;
    const size_t to = (i + 1) * a - 1;

    uint32_t idx = sphincs_utils::extract_contiguous_bits_as_u32(msg, frm, to);

    const size_t off0 = i * sig_elm_len;     // next n -bytes secret key value
    const size_t off1 = off0 + skey_val_len; // next a * n -bytes auth path

    adrs.set_tree_height(0u);
    adrs.set_tree_index(i * t + idx);

    sphincs_hashing::f<n, v>(pk_seed, adrs.data, sig + off0, c_nodes + 0);

    for (uint32_t j = 0; j < a; j++) {
      const size_t off2 = off1 + j * n;
      adrs.set_tree_height(j + 1u);

      const bool flg = static_cast<bool>((idx >> j) & 1u);
      if (!flg) {
        adrs.set_tree_index(adrs.get_tree_index() >> 1);

        std::memcpy(c_nodes + n, sig + off2, n);
        sphincs_hashing::h<n, v>(pk_seed, adrs.data, c_nodes, tmp);
        std::memcpy(c_nodes + n, tmp, n);
      } else {
        adrs.set_tree_index((adrs.get_tree_index() - 1u) >> 1);

        std::memcpy(c_nodes + n, c_nodes + 0, n);
        std::memcpy(c_nodes + 0, sig + off2, n);
        sphincs_hashing::h<n, v>(pk_seed, adrs.data, c_nodes, tmp);
        std::memcpy(c_nodes + n, tmp, n);
      }

      std::memcpy(c_nodes + 0, c_nodes + n, n);
    }

    const size_t off2 = i * n; // next n -bytes i -th FORS tree root
    std::memcpy(roots + off2, c_nodes + 0, n);
  }

  sphincs_adrs::fors_roots_t roots_adrs{ adrs };

  roots_adrs.set_type(sphincs_adrs::type_t::FORS_ROOTS);
  roots_adrs.set_keypair_address(adrs.get_keypair_address());

  sphincs_hashing::t_l<n, k, v>(pk_seed, roots_adrs.data, roots, pkey);
}

}
