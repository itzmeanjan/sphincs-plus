#include "bench_helper.hpp"
#include "bench_sphincs+.hpp"

BENCHMARK(bench_sphincs_plus::keygen<16, 66, 22, 16, sphincs_plus_hashing::variant::simple>)
  ->Name("sphincs+-128f-simple/keygen")
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_sphincs_plus::sign<16, 66, 22, 6, 33, 16, sphincs_plus_hashing::variant::simple>)
  ->Name("sphincs+-128f-simple/sign")
  ->Arg(32)
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_sphincs_plus::verify<16, 66, 22, 6, 33, 16, sphincs_plus_hashing::variant::simple>)
  ->Name("sphincs+-128f-simple/verify")
  ->Arg(32)
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
