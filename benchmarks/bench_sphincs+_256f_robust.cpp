#include "bench_sphincs+.hpp"

BENCHMARK(bench_sphincs_plus::keygen<32, 68, 17, 16, sphincs_plus_hashing::variant::robust>)->Name("sphincs+-256f-robust/keygen");
BENCHMARK(bench_sphincs_plus::sign<32, 68, 17, 9, 35, 16, sphincs_plus_hashing::variant::robust>)->Name("sphincs+-256f-robust/sign")->Arg(32);
BENCHMARK(bench_sphincs_plus::verify<32, 68, 17, 9, 35, 16, sphincs_plus_hashing::variant::robust>)->Name("sphincs+-256f-robust/verify")->Arg(32);
