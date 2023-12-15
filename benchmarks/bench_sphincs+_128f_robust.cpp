#include "bench_sphincs+.hpp"

BENCHMARK(bench_sphincs_plus::keygen<16, 66, 22, 16, sphincs_plus_hashing::variant::robust>)->Name("sphincs+-128f-robust/keygen");
BENCHMARK(bench_sphincs_plus::sign<16, 66, 22, 6, 33, 16, sphincs_plus_hashing::variant::robust>)->Name("sphincs+-128f-robust/sign");
BENCHMARK(bench_sphincs_plus::verify<16, 66, 22, 6, 33, 16, sphincs_plus_hashing::variant::robust>)->Name("sphincs+-128f-robust/verify");
