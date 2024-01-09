#include "bench_sphincs+.hpp"

BENCHMARK(bench_sphincs_plus::keygen<32, 68, 17, 16, sphincs_plus_hashing::variant::simple>)->Name("sphincs+-256f-simple/keygen");
BENCHMARK(bench_sphincs_plus::sign<32, 68, 17, 9, 35, 16, sphincs_plus_hashing::variant::simple>)->Name("sphincs+-256f-simple/sign")->Arg(32);
BENCHMARK(bench_sphincs_plus::verify<32, 68, 17, 9, 35, 16, sphincs_plus_hashing::variant::simple>)->Name("sphincs+-256f-simple/verify")->Arg(32);
