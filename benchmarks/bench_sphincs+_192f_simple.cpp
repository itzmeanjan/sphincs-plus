#include "bench_sphincs+.hpp"

BENCHMARK(bench_sphincs_plus::keygen<24, 66, 22, 16, sphincs_plus_hashing::variant::simple>)->Name("sphincs+-192f-simple/keygen");
BENCHMARK(bench_sphincs_plus::sign<24, 66, 22, 8, 33, 16, sphincs_plus_hashing::variant::simple>)->Name("sphincs+-192f-simple/sign")->Arg(32);
BENCHMARK(bench_sphincs_plus::verify<24, 66, 22, 8, 33, 16, sphincs_plus_hashing::variant::simple>)->Name("sphincs+-192f-simple/verify")->Arg(32);
