#include "bench_sphincs+.hpp"

BENCHMARK(bench_sphincs::keygen<16, 66, 22, 16, sphincs_hashing::variant::simple>)->Name("sphincs+-128f-simple/keygen");
BENCHMARK(bench_sphincs::sign<16, 66, 22, 6, 33, 16, sphincs_hashing::variant::simple>)->Name("sphincs+-128f-simple/sign");
BENCHMARK(bench_sphincs::verify<16, 66, 22, 6, 33, 16, sphincs_hashing::variant::simple>)->Name("sphincs+-128f-simple/verify");
