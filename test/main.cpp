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

  test_sphincs::test_fors<16, 12, 14, sphincs_hashing::variant::robust>();
  test_sphincs::test_fors<16, 12, 14, sphincs_hashing::variant::simple>();
  test_sphincs::test_fors<16, 6, 33, sphincs_hashing::variant::robust>();
  test_sphincs::test_fors<16, 6, 33, sphincs_hashing::variant::simple>();

  test_sphincs::test_fors<24, 14, 17, sphincs_hashing::variant::robust>();
  test_sphincs::test_fors<24, 14, 17, sphincs_hashing::variant::simple>();
  test_sphincs::test_fors<24, 8, 33, sphincs_hashing::variant::robust>();
  test_sphincs::test_fors<24, 8, 33, sphincs_hashing::variant::simple>();

  test_sphincs::test_fors<32, 14, 22, sphincs_hashing::variant::robust>();
  test_sphincs::test_fors<32, 14, 22, sphincs_hashing::variant::simple>();
  test_sphincs::test_fors<32, 9, 35, sphincs_hashing::variant::robust>();
  test_sphincs::test_fors<32, 9, 35, sphincs_hashing::variant::simple>();

  std::cout << "[test] FORS signature\n";

  {
    using namespace test_sphincs;
    using namespace sphincs_hashing;

    test_sphincs_plus<16, 63, 7, 12, 14, 16, variant::robust>(32);
    test_sphincs_plus<16, 63, 7, 12, 14, 16, variant::robust, true>(32);
    test_sphincs_plus<16, 63, 7, 12, 14, 16, variant::simple>(32);
    test_sphincs_plus<16, 63, 7, 12, 14, 16, variant::simple, true>(32);

    test_sphincs_plus<16, 66, 22, 6, 33, 16, variant::robust>(32);
    test_sphincs_plus<16, 66, 22, 6, 33, 16, variant::robust, true>(32);
    test_sphincs_plus<16, 66, 22, 6, 33, 16, variant::simple>(32);
    test_sphincs_plus<16, 66, 22, 6, 33, 16, variant::simple, true>(32);

    test_sphincs_plus<24, 63, 7, 14, 17, 16, variant::robust>(32);
    test_sphincs_plus<24, 63, 7, 14, 17, 16, variant::robust, true>(32);
    test_sphincs_plus<24, 63, 7, 14, 17, 16, variant::simple>(32);
    test_sphincs_plus<24, 63, 7, 14, 17, 16, variant::simple, true>(32);

    test_sphincs_plus<24, 66, 22, 8, 33, 16, variant::robust>(32);
    test_sphincs_plus<24, 66, 22, 8, 33, 16, variant::robust, true>(32);
    test_sphincs_plus<24, 66, 22, 8, 33, 16, variant::simple>(32);
    test_sphincs_plus<24, 66, 22, 8, 33, 16, variant::simple, true>(32);

    test_sphincs_plus<32, 64, 8, 14, 22, 16, variant::robust>(32);
    test_sphincs_plus<32, 64, 8, 14, 22, 16, variant::robust, true>(32);
    test_sphincs_plus<32, 64, 8, 14, 22, 16, variant::simple>(32);
    test_sphincs_plus<32, 64, 8, 14, 22, 16, variant::simple, true>(32);

    test_sphincs_plus<32, 68, 17, 9, 35, 16, variant::robust>(32);
    test_sphincs_plus<32, 68, 17, 9, 35, 16, variant::robust, true>(32);
    test_sphincs_plus<32, 68, 17, 9, 35, 16, variant::simple>(32);
    test_sphincs_plus<32, 68, 17, 9, 35, 16, variant::simple, true>(32);

    std::cout << "[test] SPHINCS+ signature\n";
  }

  return EXIT_SUCCESS;
}
