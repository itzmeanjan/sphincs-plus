#include "bench_sphincs+.hpp"

BENCHMARK(bench_sphincs::keygen<24, 66, 22, 16, sphincs_hashing::variant::robust>)->Name("sphincs+-192f-robust/keygen");
BENCHMARK(bench_sphincs::sign<24, 66, 22, 8, 33, 16, sphincs_hashing::variant::robust>)->Name("sphincs+-192f-robust/sign");
BENCHMARK(bench_sphincs::verify<24, 66, 22, 8, 33, 16, sphincs_hashing::variant::robust>)->Name("sphincs+-192f-robust/verify");
