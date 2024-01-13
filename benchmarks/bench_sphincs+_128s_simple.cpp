#include "bench_helper.hpp"
#include "bench_sphincs+.hpp"

BENCHMARK(bench_sphincs_plus::keygen<16, 63, 7, 16, sphincs_plus_hashing::variant::simple>)
  ->Name("sphincs+-128s-simple/keygen")
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_sphincs_plus::sign<16, 63, 7, 12, 14, 16, sphincs_plus_hashing::variant::simple>)
  ->Name("sphincs+-128s-simple/sign")
  ->Arg(32)
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_sphincs_plus::verify<16, 63, 7, 12, 14, 16, sphincs_plus_hashing::variant::simple>)
  ->Name("sphincs+-128s-simple/verify")
  ->Arg(32)
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
