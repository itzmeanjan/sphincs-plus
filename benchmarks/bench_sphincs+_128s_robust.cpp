#include "bench_sphincs+.hpp"

BENCHMARK(bench_sphincs_plus::keygen<16, 63, 7, 16, sphincs_plus_hashing::variant::robust>)->Name("sphincs+-128s-robust/keygen");
BENCHMARK(bench_sphincs_plus::sign<16, 63, 7, 12, 14, 16, sphincs_plus_hashing::variant::robust>)->Name("sphincs+-128s-robust/sign");
BENCHMARK(bench_sphincs_plus::verify<16, 63, 7, 12, 14, 16, sphincs_plus_hashing::variant::robust>)->Name("sphincs+-128s-robust/verify");
