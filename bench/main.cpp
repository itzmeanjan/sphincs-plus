#include "bench/bench_sphincs.hpp"

using namespace sphincs_hashing;

// SPHINCS+-128s-robust
BENCHMARK(bench_sphincs::keygen<16, 63, 7, 16, variant::robust>);
BENCHMARK(bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::robust>);
BENCHMARK(bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::robust>);

// SPHINCS+-128s-simple
BENCHMARK(bench_sphincs::keygen<16, 63, 7, 16, variant::simple>);
BENCHMARK(bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::simple>);
BENCHMARK(bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::simple>);

// SPHINCS+-128f-robust
BENCHMARK(bench_sphincs::keygen<16, 66, 22, 16, variant::robust>);
BENCHMARK(bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::robust>);
BENCHMARK(bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::robust>);

// SPHINCS+-128f-simple
BENCHMARK(bench_sphincs::keygen<16, 66, 22, 16, variant::simple>);
BENCHMARK(bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::simple>);
BENCHMARK(bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::simple>);

// SPHINCS+-192s-robust
BENCHMARK(bench_sphincs::keygen<24, 63, 7, 16, variant::robust>);
BENCHMARK(bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::robust>);
BENCHMARK(bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::robust>);

// SPHINCS+-192s-simple
BENCHMARK(bench_sphincs::keygen<24, 63, 7, 16, variant::simple>);
BENCHMARK(bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::simple>);
BENCHMARK(bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::simple>);

// SPHINCS+-192f-robust
BENCHMARK(bench_sphincs::keygen<24, 66, 22, 16, variant::robust>);
BENCHMARK(bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::robust>);
BENCHMARK(bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::robust>);

// SPHINCS+-192f-simple
BENCHMARK(bench_sphincs::keygen<24, 66, 22, 16, variant::simple>);
BENCHMARK(bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::simple>);
BENCHMARK(bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::simple>);

// SPHINCS+-256s-robust
BENCHMARK(bench_sphincs::keygen<32, 64, 8, 16, variant::robust>);
BENCHMARK(bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::robust>);
BENCHMARK(bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::robust>);

// SPHINCS+-256s-simple
BENCHMARK(bench_sphincs::keygen<32, 64, 8, 16, variant::simple>);
BENCHMARK(bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::simple>);
BENCHMARK(bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::simple>);

// SPHINCS+-256f-robust
BENCHMARK(bench_sphincs::keygen<32, 68, 17, 16, variant::robust>);
BENCHMARK(bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::robust>);
BENCHMARK(bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::robust>);

// SPHINCS+-256f-simple
BENCHMARK(bench_sphincs::keygen<32, 68, 17, 16, variant::simple>);
BENCHMARK(bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::simple>);
BENCHMARK(bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::simple>);

BENCHMARK_MAIN();
