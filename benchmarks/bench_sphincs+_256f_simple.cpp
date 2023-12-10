#include "bench_sphincs+.hpp"

BENCHMARK(bench_sphincs::keygen<32, 68, 17, 16, sphincs_hashing::variant::simple>)->Name("sphincs+-256f-simple/keygen");
BENCHMARK(bench_sphincs::sign<32, 68, 17, 9, 35, 16, sphincs_hashing::variant::simple>)->Name("sphincs+-256f-simple/sign");
BENCHMARK(bench_sphincs::verify<32, 68, 17, 9, 35, 16, sphincs_hashing::variant::simple>)->Name("sphincs+-256f-simple/verify");