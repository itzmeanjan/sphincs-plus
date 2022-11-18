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

  return EXIT_SUCCESS;
}
