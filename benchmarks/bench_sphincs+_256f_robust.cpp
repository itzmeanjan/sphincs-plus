#include "bench_sphincs+.hpp"

BENCHMARK(bench_sphincs::keygen<32, 68, 17, 16, sphincs_hashing::variant::robust>)->Name("sphincs+-256f-robust/keygen");
BENCHMARK(bench_sphincs::sign<32, 68, 17, 9, 35, 16, sphincs_hashing::variant::robust>)->Name("sphincs+-256f-robust/sign");
BENCHMARK(bench_sphincs::verify<32, 68, 17, 9, 35, 16, sphincs_hashing::variant::robust>)->Name("sphincs+-256f-robust/verify");