#include "bench_helper.hpp"
#include "bench_sphincs+.hpp"

BENCHMARK(bench_sphincs_plus::keygen<24, 63, 7, 16, sphincs_plus_hashing::variant::simple>)
  ->Name("sphincs+-192s-simple/keygen")
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_sphincs_plus::sign<24, 63, 7, 14, 17, 16, sphincs_plus_hashing::variant::simple>)
  ->Name("sphincs+-192s-simple/sign")
  ->Arg(32)
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_sphincs_plus::verify<24, 63, 7, 14, 17, 16, sphincs_plus_hashing::variant::simple>)
  ->Name("sphincs+-192s-simple/verify")
  ->Arg(32)
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
