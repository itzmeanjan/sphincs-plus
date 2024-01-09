#include "bench_sphincs+.hpp"

BENCHMARK(bench_sphincs_plus::keygen<24, 63, 7, 16, sphincs_plus_hashing::variant::robust>)->Name("sphincs+-192s-robust/keygen");
BENCHMARK(bench_sphincs_plus::sign<24, 63, 7, 14, 17, 16, sphincs_plus_hashing::variant::robust>)->Name("sphincs+-192s-robust/sign")->Arg(32);
BENCHMARK(bench_sphincs_plus::verify<24, 63, 7, 14, 17, 16, sphincs_plus_hashing::variant::robust>)->Name("sphincs+-192s-robust/verify")->Arg(32);
