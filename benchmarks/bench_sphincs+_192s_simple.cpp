#include "bench_sphincs+.hpp"

BENCHMARK(bench_sphincs::keygen<24, 63, 7, 16, sphincs_hashing::variant::simple>)->Name("sphincs+-192s-simple/keygen");
BENCHMARK(bench_sphincs::sign<24, 63, 7, 14, 17, 16, sphincs_hashing::variant::simple>)->Name("sphincs+-192s-simple/sign");
BENCHMARK(bench_sphincs::verify<24, 63, 7, 14, 17, 16, sphincs_hashing::variant::simple>)->Name("sphincs+-192s-simple/verify");
