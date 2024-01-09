#include "bench_sphincs+.hpp"

BENCHMARK(bench_sphincs_plus::keygen<32, 64, 8, 16, sphincs_plus_hashing::variant::simple>)->Name("sphincs+-256s-simple/keygen");
BENCHMARK(bench_sphincs_plus::sign<32, 64, 8, 14, 22, 16, sphincs_plus_hashing::variant::simple>)->Name("sphincs+-256s-simple/sign")->Arg(32);
BENCHMARK(bench_sphincs_plus::verify<32, 64, 8, 14, 22, 16, sphincs_plus_hashing::variant::simple>)->Name("sphincs+-256s-simple/verify")->Arg(32);
