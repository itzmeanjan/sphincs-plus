#include "bench_sphincs+.hpp"

BENCHMARK(bench_sphincs::keygen<16, 63, 7, 16, sphincs_hashing::variant::simple>)->Name("sphincs+-128s-simple/keygen");
BENCHMARK(bench_sphincs::sign<16, 63, 7, 12, 14, 16, sphincs_hashing::variant::simple>)->Name("sphincs+-128s-simple/sign");
BENCHMARK(bench_sphincs::verify<16, 63, 7, 12, 14, 16, sphincs_hashing::variant::simple>)->Name("sphincs+-128s-simple/verify");
