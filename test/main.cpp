#include "test/test_sphincs.hpp"
#include <iostream>

int
main()
{
  test_sphincs::test_wots<16, 16, sphincs_hashing::variant::robust>();
  test_sphincs::test_wots<16, 16, sphincs_hashing::variant::simple>();

  test_sphincs::test_wots<24, 16, sphincs_hashing::variant::robust>();
  test_sphincs::test_wots<24, 16, sphincs_hashing::variant::simple>();

  test_sphincs::test_wots<32, 16, sphincs_hashing::variant::robust>();
  test_sphincs::test_wots<32, 16, sphincs_hashing::variant::simple>();

  std::cout << "[test] WOTS+ one-time signature\n";

  test_sphincs::test_xmss<63 / 7, 16, 16, sphincs_hashing::variant::robust>();
  test_sphincs::test_xmss<63 / 7, 16, 16, sphincs_hashing::variant::simple>();
  test_sphincs::test_xmss<66 / 22, 16, 16, sphincs_hashing::variant::robust>();
  test_sphincs::test_xmss<66 / 22, 16, 16, sphincs_hashing::variant::simple>();

  test_sphincs::test_xmss<63 / 7, 24, 16, sphincs_hashing::variant::robust>();
  test_sphincs::test_xmss<63 / 7, 24, 16, sphincs_hashing::variant::simple>();
  test_sphincs::test_xmss<66 / 22, 24, 16, sphincs_hashing::variant::robust>();
  test_sphincs::test_xmss<66 / 22, 24, 16, sphincs_hashing::variant::simple>();

  test_sphincs::test_xmss<64 / 8, 32, 16, sphincs_hashing::variant::robust>();
  test_sphincs::test_xmss<64 / 8, 32, 16, sphincs_hashing::variant::simple>();
  test_sphincs::test_xmss<68 / 17, 32, 16, sphincs_hashing::variant::robust>();
  test_sphincs::test_xmss<68 / 17, 32, 16, sphincs_hashing::variant::simple>();

  std::cout << "[test] Fixed Input-Length XMSS signature\n";

  {
    using namespace test_sphincs;

    test_hypertree<63, 7, 16, 16, sphincs_hashing::variant::robust>();
    test_hypertree<63, 7, 16, 16, sphincs_hashing::variant::simple>();
    test_hypertree<66, 22, 16, 16, sphincs_hashing::variant::robust>();
    test_hypertree<66, 22, 16, 16, sphincs_hashing::variant::simple>();

    test_hypertree<63, 7, 24, 16, sphincs_hashing::variant::robust>();
    test_hypertree<63, 7, 24, 16, sphincs_hashing::variant::simple>();
    test_hypertree<66, 22, 24, 16, sphincs_hashing::variant::robust>();
    test_hypertree<66, 22, 24, 16, sphincs_hashing::variant::simple>();

    test_hypertree<64, 8, 32, 16, sphincs_hashing::variant::robust>();
    test_hypertree<64, 8, 32, 16, sphincs_hashing::variant::simple>();
    test_hypertree<68, 17, 32, 16, sphincs_hashing::variant::robust>();
    test_hypertree<68, 17, 32, 16, sphincs_hashing::variant::simple>();

    std::cout << "[test] HyperTree signature\n";
  }

  return EXIT_SUCCESS;
}
