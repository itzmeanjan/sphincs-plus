#include "bench_sphincs+.hpp"

BENCHMARK(bench_sphincs::keygen<24, 63, 7, 16, sphincs_hashing::variant::robust>)->Name("sphincs+-192s-robust/keygen");
BENCHMARK(bench_sphincs::sign<24, 63, 7, 14, 17, 16, sphincs_hashing::variant::robust>)->Name("sphincs+-192s-robust/sign");
BENCHMARK(bench_sphincs::verify<24, 63, 7, 14, 17, 16, sphincs_hashing::variant::robust>)->Name("sphincs+-192s-robust/verify");
