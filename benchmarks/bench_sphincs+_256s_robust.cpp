#include "bench_sphincs+.hpp"

BENCHMARK(bench_sphincs::keygen<32, 64, 8, 16, sphincs_hashing::variant::robust>)->Name("sphincs+-256s-robust/keygen");
BENCHMARK(bench_sphincs::sign<32, 64, 8, 14, 22, 16, sphincs_hashing::variant::robust>)->Name("sphincs+-256s-robust/sign");
BENCHMARK(bench_sphincs::verify<32, 64, 8, 14, 22, 16, sphincs_hashing::variant::robust>)->Name("sphincs+-256s-robust/verify");
