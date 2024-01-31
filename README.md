> [!CAUTION] 
> This Sphincs+ implementation is conformant with Sphincs+ specification @ https://sphincs.org/data/sphincs+-r3.1-specification.pdf. I also try to make it timing leakage free, using `dudect` (see https://github.com/oreparaz/dudect) -based tests, but be informed that this implementation is not yet audited. *If you consider using it in production, be careful !*

# sphincs-plus
SPHINCS+: Stateless Hash-based Digital Signature Algorithm

## Introduction

SPHINCS+ is one of those post-quantum digital signature algorithms ( DSA ), which is selected by NIST for standardization. Quoting directly from SPHINCS+ specification

> [!NOTE]
> SPHINCS+ is probably the most conservative design of a post-quantum signature scheme, on the other hand, it is rather inefficient in terms of signature size and speed.

SPHINCS+ builds on SPHINCS with various improvements. I suggest you read section 1.1 of the specification. 

SPHINCS+ DSA offers following three APIs

- `keygen`: Computes random keypair where secret key is 4n -bytes and public key is 2n -bytes | n = security parameter ( in bytes )

> [!NOTE]
> SPHINCS+ secret key holds a copy of public key, which is used when signing messages.

- `sign`: Given M ( > 0 ) -bytes message, SPHINCS+ secret key ( of 4n -bytes ) is used for signing message, by default deterministically. Though one might specifically ask for randomized signing, which will produce random signatures for same message, given that the caller supplies n -bytes random seed.
- `verify`: Given M ( > 0 ) -bytes message and SPHINCS+ signature, it uses SPHINCS+ public key ( of 2n -bytes ) for verifying signature, returning boolean result. Truth value is returned if signature is successfully verified.

Here I'm maintaining SPHINCS+ as a header-only, easy-to-use C++20 library, which implements SPHINCS+-SHAKE key generation/ signing/ verification algorithms, for all parameter sets ( encompassing NIST security levels {1, 3, 5} ), as suggested in section 7.2 and table 3 of the specification.

> [!TIP]
> *Find the SPHINCS+ specification @ https://sphincs.org/data/sphincs+-r3.1-specification.pdf, which was followed during this work.*

> [!NOTE]
> Follow progress of NIST PQC standardization effort [here](https://csrc.nist.gov/projects/post-quantum-cryptography).

## Prerequisites

- A C++ compiler with C++20 standard library support such as `g++` or `clang++`

```bash
$ clang++ --version
Ubuntu clang version 17.0.2 (1~exp1ubuntu2.1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin

$ g++ --version
gcc (Ubuntu 13.2.0-4ubuntu3) 13.2.0
```

- Build systems such as `make` and `cmake`.

```bash
$ make --version
GNU Make 4.3

$ cmake --version
cmake version 3.27.4
```

- For testing correctness and compatibility of this SPHINCS+ implementation, you need to globally install `google-test` library and headers. Follow guide @ https://github.com/google/googletest/tree/main/googletest#standalone-cmake-project, if you don't have it installed.
- For benchmarking, you'll need to have `google-benchmark` system-wide installed. I found guide @ https://github.com/google/benchmark#installation helpful.

> [!NOTE]
> If you are on a machine running GNU/Linux kernel and you want to obtain CPU cycle count for `keygen`, `sign` and `verify` routines, you should consider building `google-benchmark` library with libPFM support, following https://gist.github.com/itzmeanjan/05dc3e946f635d00c5e0b21aae6203a7, a step-by-step guide. Find more about libPFM @ https://perfmon2.sourceforge.net.

> [!TIP]
> Git submodule based dependencies will generally be imported automatically, but in case that doesn't work, you can manually initialize and update them by issuing `$ git submodule update --init` from inside the root of this repository.

## Testing

For ensuring that SPHINCS+ implementation is functionally correct and compliant with the specification @ https://sphincs.org/data/sphincs+-r3.1-specification.pdf, you may issue

> [!NOTE]
> This implementation of SPHINCS+ specification is **tested** to be compatible and conformant with r3.1 of the specification. That's ensured by generating known answer tests ( KATs ) following https://gist.github.com/itzmeanjan/d483872509b8a1a7c4d6614ec9d43e6c and testing this implementation using those test vectors.

```bash
make -j            # Run tests without any sort of sanitizers
make asan_test -j  # Run tests with AddressSanitizer enabled
make ubsan_test -j # Run tests with UndefinedBehaviourSanitizer enabled
```

```bash
[27/27] SphincsPlus.SphincsPlus192sRobustKnownAnswerTests (592956 ms)
PASSED TESTS (27/27):
      17 ms: build/test.out SphincsPlus.WOTS_PlusNISTSecurityLevel5
      18 ms: build/test.out SphincsPlus.WOTS_PlusNISTSecurityLevel1
      20 ms: build/test.out SphincsPlus.WOTS_PlusNISTSecurityLevel3
    1422 ms: build/test.out SphincsPlus.FORSNISTSecurityLevel1
    2470 ms: build/test.out SphincsPlus.XMSSNISTSecurityLevel1
    2470 ms: build/test.out SphincsPlus.XMSSNISTSecurityLevel5
    3603 ms: build/test.out SphincsPlus.XMSSNISTSecurityLevel3
    6446 ms: build/test.out SphincsPlus.FORSNISTSecurityLevel3
    7932 ms: build/test.out SphincsPlus.HyperTreeNISTSecurityLevel1
    8895 ms: build/test.out SphincsPlus.FORSNISTSecurityLevel5
   11539 ms: build/test.out SphincsPlus.SphincsPlus128fSimpleKnownAnswerTests
   12256 ms: build/test.out SphincsPlus.HyperTreeNISTSecurityLevel5
   15019 ms: build/test.out SphincsPlus.HyperTreeNISTSecurityLevel3
   21817 ms: build/test.out SphincsPlus.SphincsPlusNISTSecurityLevel1KeygenSignVerify
   25749 ms: build/test.out SphincsPlus.SphincsPlusNISTSecurityLevel5KeygenSignVerify
   26933 ms: build/test.out SphincsPlus.SphincsPlus192fSimpleKnownAnswerTests
   31361 ms: build/test.out SphincsPlus.SphincsPlus128fRobustKnownAnswerTests
   33611 ms: build/test.out SphincsPlus.SphincsPlusNISTSecurityLevel3KeygenSignVerify
   34051 ms: build/test.out SphincsPlus.SphincsPlus192fRobustKnownAnswerTests
   35977 ms: build/test.out SphincsPlus.SphincsPlus256fSimpleKnownAnswerTests
   67307 ms: build/test.out SphincsPlus.SphincsPlus256fRobustKnownAnswerTests
  206086 ms: build/test.out SphincsPlus.SphincsPlus128sSimpleKnownAnswerTests
  333467 ms: build/test.out SphincsPlus.SphincsPlus256sSimpleKnownAnswerTests
  367931 ms: build/test.out SphincsPlus.SphincsPlus192sSimpleKnownAnswerTests
  413912 ms: build/test.out SphincsPlus.SphincsPlus128sRobustKnownAnswerTests
  540031 ms: build/test.out SphincsPlus.SphincsPlus256sRobustKnownAnswerTests
  592956 ms: build/test.out SphincsPlus.SphincsPlus192sRobustKnownAnswerTests
```

You can run timing leakage tests, using `dudect`; execute following

> [!NOTE]
> `dudect` is integrated into this library implementation of Sphincs+ DSA to find any sort of timing leakages. It checks for constant-timeness of most of both `keygen` and `sign` function implementations, for only one variant i.e. **128f-simple**.

```bash
# Can only be built and run on x86_64 machine.
make dudect_test_build -j

# Before running the constant-time tests, it's a good idea to put all CPU cores on "performance" mode.
# You may find the guide @ https://github.com/google/benchmark/blob/main/docs/reducing_variance.md helpful.

# Given Sphincs+ is slow, compared to Dilithium/ Falcon, following tests are required to be run
# for longer, so that we can collect enough execution timing samples.
timeout 2h taskset -c 0 ./build/dudect/test_sphincs+_128f_simple_keygen.out
timeout 2h taskset -c 0 ./build/dudect/test_sphincs+_128f_simple_sign.out
```

> [!TIP]
> `dudect` documentation says if `t` statistic is `< 10`, we're *probably* good, yes **probably**. You may want to read `dudect` documentation @ https://github.com/oreparaz/dudect. Also you might find the original paper @ https://ia.cr/2016/1123 interesting.

```bash
...
meas:    0.69 M, max t:   +2.58, max tau: 3.11e-03, (5/tau)^2: 2.58e+06. For the moment, maybe constant time.
meas:    0.70 M, max t:   +2.74, max tau: 3.27e-03, (5/tau)^2: 2.34e+06. For the moment, maybe constant time.
meas:    0.71 M, max t:   +2.73, max tau: 3.24e-03, (5/tau)^2: 2.38e+06. For the moment, maybe constant time.
meas:    0.72 M, max t:   +2.62, max tau: 3.09e-03, (5/tau)^2: 2.61e+06. For the moment, maybe constant time.
meas:    0.73 M, max t:   +2.66, max tau: 3.11e-03, (5/tau)^2: 2.58e+06. For the moment, maybe constant time.
meas:    0.74 M, max t:   +2.70, max tau: 3.14e-03, (5/tau)^2: 2.53e+06. For the moment, maybe constant time.
meas:    0.75 M, max t:   +2.62, max tau: 3.03e-03, (5/tau)^2: 2.72e+06. For the moment, maybe constant time.
meas:    0.76 M, max t:   +2.60, max tau: 2.99e-03, (5/tau)^2: 2.80e+06. For the moment, maybe constant time.
meas:    0.77 M, max t:   +2.62, max tau: 2.99e-03, (5/tau)^2: 2.80e+06. For the moment, maybe constant time.
meas:    0.78 M, max t:   +2.52, max tau: 2.85e-03, (5/tau)^2: 3.07e+06. For the moment, maybe constant time.
meas:    0.79 M, max t:   +2.57, max tau: 2.89e-03, (5/tau)^2: 3.00e+06. For the moment, maybe constant time.
meas:    0.80 M, max t:   +2.51, max tau: 2.81e-03, (5/tau)^2: 3.18e+06. For the moment, maybe constant time.
meas:    0.81 M, max t:   +2.49, max tau: 2.77e-03, (5/tau)^2: 3.25e+06. For the moment, maybe constant time.
meas:    0.82 M, max t:   +2.52, max tau: 2.78e-03, (5/tau)^2: 3.23e+06. For the moment, maybe constant time.
```

## Benchmarking

Benchmarking key generation, signing and verification algorithms for various instantiations of SPHINCS+ digital signature scheme can be done, by issuing

```bash
make benchmark -j  # If you haven't built google-benchmark library with libPFM support.
make perf -j       # If you have built google-benchmark library with libPFM support.
```

> [!NOTE]
> Benchmarking expects presence of `google-benchmark` headers and library in global namespace ( so that it can be found by the compiler ) i.e. header and library path must live on `$PATH`.

> [!CAUTION]
> Ensure you've put all your CPU cores on **performance mode** before running benchmarks, follow guide @ https://github.com/google/benchmark/blob/main/docs/reducing_variance.md.

> [!NOTE]
> `make perf` - was issued when collecting following benchmarks. Notice, cycles column, denoting cost of executing SPHINCS+ signature scheme routines in terms of CPU cycles. Follow https://github.com/google/benchmark/blob/main/docs/perf_counters.md for more details. For x86_64 targets, notice `rdtsc` column, showing CPU ticks collected using RD (read) *T*ime *S*tamp *C*ounter.

### On 12th Gen Intel(R) Core(TM) i7-1260P

Compiled with **gcc version 13.2.0 (Ubuntu 13.2.0-4ubuntu3)**.

```bash
$ uname -srm
Linux 6.5.0-14-generic x86_64
```

```bash
2024-01-12T23:13:17+04:00
Running ./build/perf.out
Run on (16 X 2807.24 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 1280 KiB (x8)
  L3 Unified 18432 KiB (x1)
Load Average: 2.02, 1.24, 1.09
-----------------------------------------------------------------------------------------------------------------------
Benchmark                                      Time             CPU   Iterations     CYCLES items_per_second      rdtsc
-----------------------------------------------------------------------------------------------------------------------
sphincs+-256f-simple/sign/32_mean           91.5 ms         91.5 ms           10   420.127M        10.9258/s   228.503M
sphincs+-256f-simple/sign/32_median         91.1 ms         91.1 ms           10   418.845M        10.9745/s   227.437M
sphincs+-256f-simple/sign/32_stddev         1.42 ms         1.42 ms           10   4.03714M       0.166404/s   3.55219M
sphincs+-256f-simple/sign/32_cv             1.55 %          1.56 %            10      0.96%            1.52%      1.55%
sphincs+-256f-simple/sign/32_min            90.3 ms         90.3 ms           10   415.334M        10.5449/s   225.314M
sphincs+-256f-simple/sign/32_max            94.8 ms         94.8 ms           10   424.867M         11.078/s   236.701M
sphincs+-192f-robust/sign/32_mean            134 ms          134 ms           10   624.985M         7.4607/s   335.143M
sphincs+-192f-robust/sign/32_median          138 ms          138 ms           10   646.056M        7.22728/s   345.356M
sphincs+-192f-robust/sign/32_stddev         5.87 ms         5.87 ms           10   28.2531M       0.332029/s   14.6609M
sphincs+-192f-robust/sign/32_cv             4.37 %          4.38 %            10      4.52%            4.45%      4.37%
sphincs+-192f-robust/sign/32_min             127 ms          127 ms           10   590.617M        7.15265/s   317.211M
sphincs+-192f-robust/sign/32_max             140 ms          140 ms           10   647.798M        7.86917/s    348.96M
sphincs+-192f-robust/verify/32_mean         9.61 ms         9.61 ms           10   44.7807M        104.398/s   23.9923M
sphincs+-192f-robust/verify/32_median       9.61 ms         9.61 ms           10   44.8682M        104.194/s    23.987M
sphincs+-192f-robust/verify/32_stddev      0.598 ms        0.598 ms           10   2.82949M        6.49884/s     1.492M
sphincs+-192f-robust/verify/32_cv           6.22 %          6.22 %            10      6.32%            6.23%      6.22%
sphincs+-192f-robust/verify/32_min          8.87 ms         8.87 ms           10   41.4316M        96.9217/s   22.1436M
sphincs+-192f-robust/verify/32_max          10.3 ms         10.3 ms           10   48.1533M        112.718/s    25.754M
sphincs+-256s-simple/sign/32_mean            867 ms          867 ms           10   3.95845G        1.15412/s   2.16327G
sphincs+-256s-simple/sign/32_median          863 ms          863 ms           10   3.95708G        1.15839/s   2.15476G
sphincs+-256s-simple/sign/32_stddev         14.4 ms         14.4 ms           10   43.5014M      0.0189863/s   35.9866M
sphincs+-256s-simple/sign/32_cv             1.66 %          1.66 %            10      1.10%            1.65%      1.66%
sphincs+-256s-simple/sign/32_min             846 ms          846 ms           10   3.91363G        1.12109/s   2.11081G
sphincs+-256s-simple/sign/32_max             892 ms          892 ms           10   4.00517G        1.18253/s   2.22644G
sphincs+-256s-robust/sign/32_mean           2444 ms         2444 ms           10   11.3212G       0.409781/s   6.10009G
sphincs+-256s-robust/sign/32_median         2386 ms         2386 ms           10   11.0348G       0.419183/s   5.95477G
sphincs+-256s-robust/sign/32_stddev         98.5 ms         98.5 ms           10   467.605M      0.0160907/s   245.903M
sphincs+-256s-robust/sign/32_cv             4.03 %          4.03 %            10      4.13%            3.93%      4.03%
sphincs+-256s-robust/sign/32_min            2363 ms         2363 ms           10   10.9753G       0.386185/s   5.89891G
sphincs+-256s-robust/sign/32_max            2590 ms         2589 ms           10   11.9996G       0.423137/s   6.46342G
sphincs+-256s-simple/verify/32_mean         1.24 ms         1.24 ms           10   5.68167M        806.459/s    3.0974M
sphincs+-256s-simple/verify/32_median       1.24 ms         1.24 ms           10   5.62389M        805.352/s   3.09937M
sphincs+-256s-simple/verify/32_stddev      0.036 ms        0.036 ms           10   121.113k        22.6866/s   89.1605k
sphincs+-256s-simple/verify/32_cv           2.88 %          2.87 %            10      2.13%            2.81%      2.88%
sphincs+-256s-simple/verify/32_min          1.20 ms         1.20 ms           10   5.58227M          760.8/s   3.00471M
sphincs+-256s-simple/verify/32_max          1.32 ms         1.31 ms           10   5.90764M        830.675/s   3.28238M
sphincs+-192f-simple/sign/32_mean           48.0 ms         48.0 ms           10   222.215M         20.817/s   119.914M
sphincs+-192f-simple/sign/32_median         48.1 ms         48.1 ms           10   222.493M        20.7849/s   120.091M
sphincs+-192f-simple/sign/32_stddev        0.459 ms        0.459 ms           10   1.81209M       0.199738/s   1.14573M
sphincs+-192f-simple/sign/32_cv             0.96 %          0.96 %            10      0.82%            0.96%      0.96%
sphincs+-192f-simple/sign/32_min            47.3 ms         47.3 ms           10   219.535M        20.5639/s   118.142M
sphincs+-192f-simple/sign/32_max            48.6 ms         48.6 ms           10   224.497M        21.1279/s   121.382M
sphincs+-192f-robust/keygen_mean            6.48 ms         6.48 ms           10   30.1237M        154.773/s   16.1682M
sphincs+-192f-robust/keygen_median          6.31 ms         6.31 ms           10   29.3171M        158.478/s   15.7504M
sphincs+-192f-robust/keygen_stddev         0.358 ms        0.358 ms           10   1.70474M        7.93803/s   894.109k
sphincs+-192f-robust/keygen_cv              5.53 %          5.53 %            10      5.66%            5.13%      5.53%
sphincs+-192f-robust/keygen_min             6.28 ms         6.28 ms           10   29.2526M        139.415/s     15.68M
sphincs+-192f-robust/keygen_max             7.17 ms         7.17 ms           10   33.3905M        159.183/s   17.9037M
sphincs+-192s-robust/verify/32_mean         3.33 ms         3.33 ms           10    15.477M        301.913/s   8.31031M
sphincs+-192s-robust/verify/32_median       3.33 ms         3.33 ms           10   15.4693M        301.365/s   8.30407M
sphincs+-192s-robust/verify/32_stddev      0.251 ms        0.251 ms           10   1.19271M        22.7708/s   626.523k
sphincs+-192s-robust/verify/32_cv           7.54 %          7.53 %            10      7.71%            7.54%      7.54%
sphincs+-192s-robust/verify/32_min          3.04 ms         3.04 ms           10   14.0979M        276.114/s   7.58682M
sphincs+-192s-robust/verify/32_max          3.63 ms         3.62 ms           10   16.8332M        328.988/s   9.04744M
sphincs+-128f-robust/sign/32_mean           80.7 ms         80.7 ms           10   375.056M        12.4156/s   201.382M
sphincs+-128f-robust/sign/32_median         78.7 ms         78.7 ms           10   365.035M        12.7095/s    196.41M
sphincs+-128f-robust/sign/32_stddev         3.52 ms         3.52 ms           10   16.8422M       0.523205/s    8.7823M
sphincs+-128f-robust/sign/32_cv             4.36 %          4.36 %            10      4.49%            4.21%      4.36%
sphincs+-128f-robust/sign/32_min            78.1 ms         78.1 ms           10   363.173M        11.6363/s   195.027M
sphincs+-128f-robust/sign/32_max            85.9 ms         85.9 ms           10   399.601M         12.799/s   214.511M
sphincs+-192s-simple/verify/32_mean        0.858 ms        0.858 ms           10   3.95577M       1.16656k/s    2.1415M
sphincs+-192s-simple/verify/32_median      0.857 ms        0.857 ms           10   3.96467M       1.16699k/s    2.1388M
sphincs+-192s-simple/verify/32_stddev      0.027 ms        0.027 ms           10   121.765k        36.1822/s   66.2661k
sphincs+-192s-simple/verify/32_cv           3.09 %          3.10 %            10      3.08%            3.10%      3.09%
sphincs+-192s-simple/verify/32_min         0.815 ms        0.815 ms           10   3.79805M       1.12043k/s   2.03505M
sphincs+-192s-simple/verify/32_max         0.893 ms        0.893 ms           10   4.13323M       1.22652k/s   2.22764M
sphincs+-128s-simple/sign/32_mean            589 ms          589 ms           10   2.69046G        1.69743/s   1.47078G
sphincs+-128s-simple/sign/32_median          586 ms          586 ms           10   2.70088G        1.70683/s   1.46236G
sphincs+-128s-simple/sign/32_stddev         8.79 ms         8.77 ms           10   36.9169M      0.0248951/s   21.9291M
sphincs+-128s-simple/sign/32_cv             1.49 %          1.49 %            10      1.37%            1.47%      1.49%
sphincs+-128s-simple/sign/32_min             580 ms          580 ms           10   2.59299G        1.64947/s   1.44785G
sphincs+-128s-simple/sign/32_max             606 ms          606 ms           10   2.72398G        1.72395/s   1.51328G
sphincs+-128s-robust/keygen_mean             284 ms          284 ms           10    1.3202G          3.528/s   709.484M
sphincs+-128s-robust/keygen_median           275 ms          275 ms           10    1.2751G        3.63081/s   687.468M
sphincs+-128s-robust/keygen_stddev          16.2 ms         16.2 ms           10   73.8294M       0.192319/s   40.4604M
sphincs+-128s-robust/keygen_cv              5.70 %          5.70 %            10      5.59%            5.45%      5.70%
sphincs+-128s-robust/keygen_min              272 ms          272 ms           10   1.27306G        3.23802/s   679.681M
sphincs+-128s-robust/keygen_max              309 ms          309 ms           10    1.4375G        3.67244/s    770.84M
sphincs+-192s-robust/sign/32_mean           2847 ms         2847 ms           10   13.2272G        0.35184/s   7.10542G
sphincs+-192s-robust/sign/32_median         2854 ms         2854 ms           10    13.273G       0.350796/s   7.12352G
sphincs+-192s-robust/sign/32_stddev          118 ms          118 ms           10   588.975M      0.0146597/s   295.654M
sphincs+-192s-robust/sign/32_cv             4.16 %          4.16 %            10      4.45%            4.17%      4.16%
sphincs+-192s-robust/sign/32_min            2715 ms         2715 ms           10   12.6173G       0.337405/s   6.77648G
sphincs+-192s-robust/sign/32_max            2964 ms         2964 ms           10   13.8004G       0.368342/s   7.39778G
sphincs+-256s-robust/keygen_mean             277 ms          277 ms           10   1.28877G        3.62681/s   690.389M
sphincs+-256s-robust/keygen_median           264 ms          264 ms           10   1.23017G        3.78344/s   659.714M
sphincs+-256s-robust/keygen_stddev          16.5 ms         16.5 ms           10   76.6499M       0.211597/s   41.2225M
sphincs+-256s-robust/keygen_cv              5.97 %          5.97 %            10      5.95%            5.83%      5.97%
sphincs+-256s-robust/keygen_min              263 ms          263 ms           10   1.22795G        3.36902/s    657.38M
sphincs+-256s-robust/keygen_max              297 ms          297 ms           10   1.37885G        3.79697/s   740.907M
sphincs+-192s-simple/sign/32_mean           1046 ms         1046 ms           10   4.78365G       0.956132/s   2.61182G
sphincs+-192s-simple/sign/32_median         1041 ms         1041 ms           10   4.75796G         0.9609/s   2.59762G
sphincs+-192s-simple/sign/32_stddev         24.0 ms         24.0 ms           10   48.2376M      0.0217202/s   59.9911M
sphincs+-192s-simple/sign/32_cv             2.30 %          2.30 %            10      1.01%            2.27%      2.30%
sphincs+-192s-simple/sign/32_min            1020 ms         1020 ms           10   4.73949G        0.92027/s   2.54581G
sphincs+-192s-simple/sign/32_max            1087 ms         1087 ms           10   4.85435G       0.980441/s   2.71234G
sphincs+-128s-robust/verify/32_mean         2.22 ms         2.22 ms           10   10.3345M        451.463/s   5.54406M
sphincs+-128s-robust/verify/32_median       2.25 ms         2.25 ms           10   10.4624M        445.209/s   5.60635M
sphincs+-128s-robust/verify/32_stddev      0.122 ms        0.122 ms           10   571.844k         24.838/s   304.253k
sphincs+-128s-robust/verify/32_cv           5.49 %          5.48 %            10      5.53%            5.50%      5.49%
sphincs+-128s-robust/verify/32_min          2.06 ms         2.06 ms           10   9.47653M        420.588/s   5.14351M
sphincs+-128s-robust/verify/32_max          2.38 ms         2.38 ms           10   11.0357M        485.272/s   5.93813M
sphincs+-128s-simple/verify/32_mean        0.718 ms        0.718 ms           10   3.31151M       1.39435k/s   1.79278M
sphincs+-128s-simple/verify/32_median      0.718 ms        0.718 ms           10   3.28631M       1.39237k/s   1.79272M
sphincs+-128s-simple/verify/32_stddev      0.029 ms        0.029 ms           10   144.956k        56.9857/s   73.4743k
sphincs+-128s-simple/verify/32_cv           4.10 %          4.10 %            10      4.38%            4.09%      4.10%
sphincs+-128s-simple/verify/32_min         0.670 ms        0.670 ms           10   3.11021M       1.29356k/s   1.67155M
sphincs+-128s-simple/verify/32_max         0.773 ms        0.773 ms           10   3.58344M       1.49323k/s   1.92951M
sphincs+-256f-simple/keygen_mean            4.70 ms         4.70 ms           10   21.7321M        212.849/s    11.729M
sphincs+-256f-simple/keygen_median          4.69 ms         4.69 ms           10   21.7256M        213.323/s   11.7026M
sphincs+-256f-simple/keygen_stddev         0.060 ms        0.060 ms           10   93.7117k        2.67482/s   150.166k
sphincs+-256f-simple/keygen_cv              1.28 %          1.28 %            10      0.43%            1.26%      1.28%
sphincs+-256f-simple/keygen_min             4.64 ms         4.64 ms           10    21.584M        206.227/s   11.5872M
sphincs+-256f-simple/keygen_max             4.85 ms         4.85 ms           10    21.851M        215.408/s   12.1036M
sphincs+-256s-simple/keygen_mean            74.7 ms         74.7 ms           10   346.436M        13.3867/s   186.466M
sphincs+-256s-simple/keygen_median          74.7 ms         74.7 ms           10   346.054M        13.3798/s    186.55M
sphincs+-256s-simple/keygen_stddev         0.518 ms        0.518 ms           10   1.38119M      0.0924487/s   1.29127M
sphincs+-256s-simple/keygen_cv              0.69 %          0.69 %            10      0.40%            0.69%      0.69%
sphincs+-256s-simple/keygen_min             74.1 ms         74.1 ms           10   344.587M        13.1929/s   184.856M
sphincs+-256s-simple/keygen_max             75.8 ms         75.8 ms           10   348.838M        13.5023/s   189.192M
sphincs+-128f-simple/keygen_mean            1.50 ms         1.50 ms           10    6.8694M        669.033/s   3.73398M
sphincs+-128f-simple/keygen_median          1.52 ms         1.52 ms           10   6.69887M        659.315/s   3.78587M
sphincs+-128f-simple/keygen_stddev         0.046 ms        0.046 ms           10   229.861k        20.6446/s   113.994k
sphincs+-128f-simple/keygen_cv              3.05 %          3.05 %            10      3.35%            3.09%      3.05%
sphincs+-128f-simple/keygen_min             1.44 ms         1.44 ms           10   6.67201M        648.602/s   3.58951M
sphincs+-128f-simple/keygen_max             1.54 ms         1.54 ms           10   7.19017M        695.389/s   3.84832M
sphincs+-256s-robust/verify/32_mean         4.72 ms         4.72 ms           10   21.9609M        212.737/s   11.7732M
sphincs+-256s-robust/verify/32_median       4.58 ms         4.58 ms           10   21.2866M        218.409/s   11.4283M
sphincs+-256s-robust/verify/32_stddev      0.294 ms        0.294 ms           10   1.39539M         13.021/s   732.843k
sphincs+-256s-robust/verify/32_cv           6.22 %          6.23 %            10      6.35%            6.12%      6.22%
sphincs+-256s-robust/verify/32_min          4.40 ms         4.40 ms           10   20.4143M        195.633/s   10.9807M
sphincs+-256s-robust/verify/32_max          5.11 ms         5.11 ms           10   23.7988M        227.301/s   12.7583M
sphincs+-128s-simple/keygen_mean            97.0 ms         97.0 ms           10   446.767M        10.3205/s   242.063M
sphincs+-128s-simple/keygen_median          98.2 ms         98.2 ms           10   455.303M         10.183/s   245.122M
sphincs+-128s-simple/keygen_stddev          2.98 ms         2.98 ms           10   14.7085M       0.322959/s   7.43731M
sphincs+-128s-simple/keygen_cv              3.07 %          3.07 %            10      3.29%            3.13%      3.07%
sphincs+-128s-simple/keygen_min             92.6 ms         92.6 ms           10   429.091M        9.96556/s   231.103M
sphincs+-128s-simple/keygen_max              100 ms          100 ms           10   461.192M         10.802/s   250.461M
sphincs+-192s-simple/keygen_mean             118 ms          118 ms           10   538.432M        8.48208/s   294.309M
sphincs+-192s-simple/keygen_median           118 ms          118 ms           10   539.079M        8.48371/s   294.209M
sphincs+-192s-simple/keygen_stddev          1.36 ms         1.36 ms           10   7.12578M      0.0979702/s   3.39988M
sphincs+-192s-simple/keygen_cv              1.16 %          1.15 %            10      1.32%            1.16%      1.16%
sphincs+-192s-simple/keygen_min              116 ms          116 ms           10   520.653M        8.32061/s   289.155M
sphincs+-192s-simple/keygen_max              120 ms          120 ms           10   546.704M        8.63248/s   300.002M
sphincs+-256f-robust/verify/32_mean         9.47 ms         9.47 ms           10   43.9846M        105.837/s   23.6443M
sphincs+-256f-robust/verify/32_median       9.30 ms         9.30 ms           10   43.2229M        107.476/s   23.2237M
sphincs+-256f-robust/verify/32_stddev      0.514 ms        0.514 ms           10   2.24695M        5.55579/s    1.2824M
sphincs+-256f-robust/verify/32_cv           5.42 %          5.42 %            10      5.11%            5.25%      5.42%
sphincs+-256f-robust/verify/32_min          8.92 ms         8.92 ms           10   41.6165M        95.9161/s   22.2664M
sphincs+-256f-robust/verify/32_max          10.4 ms         10.4 ms           10   47.7425M        112.103/s   26.0233M
sphincs+-128f-robust/keygen_mean            4.50 ms         4.50 ms           10   20.9405M        222.945/s   11.2344M
sphincs+-128f-robust/keygen_median          4.30 ms         4.30 ms           10   19.9286M        232.394/s   10.7406M
sphincs+-128f-robust/keygen_stddev         0.281 ms        0.281 ms           10    1.3306M         13.609/s   702.095k
sphincs+-128f-robust/keygen_cv              6.25 %          6.25 %            10      6.35%            6.10%      6.25%
sphincs+-128f-robust/keygen_min             4.26 ms         4.26 ms           10   19.8659M        206.167/s   10.6268M
sphincs+-128f-robust/keygen_max             4.85 ms         4.85 ms           10   22.5108M        234.881/s   12.1069M
sphincs+-192s-robust/keygen_mean             422 ms          422 ms           10   1.96139G        2.37499/s   1.05425G
sphincs+-192s-robust/keygen_median           407 ms          407 ms           10   1.89077G         2.4572/s   1.01582G
sphincs+-192s-robust/keygen_stddev          25.5 ms         25.5 ms           10   114.799M       0.136306/s   63.5318M
sphincs+-192s-robust/keygen_cv              6.03 %          6.03 %            10      5.85%            5.74%      6.03%
sphincs+-192s-robust/keygen_min              406 ms          405 ms           10   1.88824G        2.16501/s   1.01213G
sphincs+-192s-robust/keygen_max              462 ms          462 ms           10   2.13148G        2.46612/s   1.15289G
sphincs+-256f-simple/verify/32_mean         2.48 ms         2.48 ms           10    11.451M        403.013/s    6.1977M
sphincs+-256f-simple/verify/32_median       2.49 ms         2.49 ms           10   11.4558M         401.26/s   6.22053M
sphincs+-256f-simple/verify/32_stddev      0.069 ms        0.069 ms           10   217.412k        11.1668/s   172.034k
sphincs+-256f-simple/verify/32_cv           2.78 %          2.77 %            10      1.90%            2.77%      2.78%
sphincs+-256f-simple/verify/32_min          2.39 ms         2.39 ms           10   11.1266M        384.212/s   5.96912M
sphincs+-256f-simple/verify/32_max          2.60 ms         2.60 ms           10   11.7495M        418.146/s   6.49669M
sphincs+-128f-simple/sign/32_mean           29.5 ms         29.5 ms           10   134.362M        33.8729/s   73.7389M
sphincs+-128f-simple/sign/32_median         29.2 ms         29.2 ms           10   134.076M         34.209/s   72.9678M
sphincs+-128f-simple/sign/32_stddev        0.821 ms        0.821 ms           10   1.31186M       0.921139/s   2.04878M
sphincs+-128f-simple/sign/32_cv             2.78 %          2.78 %            10      0.98%            2.72%      2.78%
sphincs+-128f-simple/sign/32_min            28.8 ms         28.8 ms           10   132.826M        32.1265/s   71.8145M
sphincs+-128f-simple/sign/32_max            31.1 ms         31.1 ms           10   136.533M        34.7588/s   77.6969M
sphincs+-256f-robust/keygen_mean            16.9 ms         16.9 ms           10   78.4116M        59.3191/s   42.1649M
sphincs+-256f-robust/keygen_median          16.5 ms         16.5 ms           10   76.4857M        60.4722/s   41.2757M
sphincs+-256f-robust/keygen_stddev         0.838 ms        0.838 ms           10   4.15786M        2.75161/s   2.09243M
sphincs+-256f-robust/keygen_cv              4.96 %          4.96 %            10      5.30%            4.64%      4.96%
sphincs+-256f-robust/keygen_min             16.4 ms         16.4 ms           10   76.1167M        53.9547/s   40.9161M
sphincs+-256f-robust/keygen_max             18.5 ms         18.5 ms           10   86.4745M        61.0025/s   46.2608M
sphincs+-256f-robust/sign/32_mean            264 ms          264 ms           10   1.22088G        3.79742/s    658.22M
sphincs+-256f-robust/sign/32_median          258 ms          258 ms           10   1.19213G         3.8782/s   643.623M
sphincs+-256f-robust/sign/32_stddev         10.4 ms         10.4 ms           10   51.9519M       0.146041/s   25.9863M
sphincs+-256f-robust/sign/32_cv             3.95 %          3.95 %            10      4.26%            3.85%      3.95%
sphincs+-256f-robust/sign/32_min             255 ms          255 ms           10   1.18222G         3.5812/s   636.553M
sphincs+-256f-robust/sign/32_max             279 ms          279 ms           10   1.29621G        3.92142/s    696.97M
sphincs+-128f-robust/verify/32_mean         6.30 ms         6.30 ms           10   28.8489M        159.303/s   15.7294M
sphincs+-128f-robust/verify/32_median       6.13 ms         6.13 ms           10   28.5194M        163.059/s   15.3076M
sphincs+-128f-robust/verify/32_stddev      0.428 ms        0.427 ms           10   1.93596M        9.88859/s   1.06862M
sphincs+-128f-robust/verify/32_cv           6.79 %          6.78 %            10      6.71%            6.21%      6.79%
sphincs+-128f-robust/verify/32_min          5.90 ms         5.90 ms           10   27.0366M        137.308/s   14.7324M
sphincs+-128f-robust/verify/32_max          7.28 ms         7.28 ms           10   34.0488M        169.435/s   18.1779M
sphincs+-192f-simple/keygen_mean            1.86 ms         1.86 ms           10   8.57268M        538.593/s   4.63516M
sphincs+-192f-simple/keygen_median          1.86 ms         1.86 ms           10   8.58642M        539.071/s   4.63055M
sphincs+-192f-simple/keygen_stddev         0.024 ms        0.024 ms           10   104.115k        6.81469/s    58.744k
sphincs+-192f-simple/keygen_cv              1.27 %          1.27 %            10      1.21%            1.27%      1.27%
sphincs+-192f-simple/keygen_min             1.81 ms         1.81 ms           10   8.44192M        524.712/s   4.52966M
sphincs+-192f-simple/keygen_max             1.91 ms         1.91 ms           10   8.70338M        551.114/s   4.75695M
sphincs+-128s-robust/sign/32_mean           1600 ms         1600 ms           10   7.42044G       0.626036/s    3.9931G
sphincs+-128s-robust/sign/32_median         1563 ms         1563 ms           10   7.22637G       0.639876/s   3.90083G
sphincs+-128s-robust/sign/32_stddev         66.5 ms         66.5 ms           10   337.655M      0.0251806/s   165.887M
sphincs+-128s-robust/sign/32_cv             4.15 %          4.15 %            10      4.55%            4.02%      4.15%
sphincs+-128s-robust/sign/32_min            1549 ms         1549 ms           10   7.16617G       0.589586/s   3.86614G
sphincs+-128s-robust/sign/32_max            1696 ms         1696 ms           10   7.92219G       0.645632/s   4.23358G
sphincs+-128f-simple/verify/32_mean         2.04 ms         2.04 ms           10   9.41893M        491.639/s   5.08112M
sphincs+-128f-simple/verify/32_median       2.05 ms         2.05 ms           10   9.41325M        486.859/s    5.1268M
sphincs+-128f-simple/verify/32_stddev      0.062 ms        0.062 ms           10   254.028k        15.0478/s   153.721k
sphincs+-128f-simple/verify/32_cv           3.03 %          3.03 %            10      2.70%            3.06%      3.03%
sphincs+-128f-simple/verify/32_min          1.93 ms         1.93 ms           10   8.98453M        471.983/s   4.81112M
sphincs+-128f-simple/verify/32_max          2.12 ms         2.12 ms           10    9.7153M        518.815/s   5.28825M
sphincs+-192f-simple/verify/32_mean         2.50 ms         2.50 ms           10   11.4917M        400.155/s   6.24137M
sphincs+-192f-simple/verify/32_median       2.49 ms         2.49 ms           10   11.4568M        402.159/s   6.20668M
sphincs+-192f-simple/verify/32_stddev      0.062 ms        0.061 ms           10   110.948k        9.45317/s   154.275k
sphincs+-192f-simple/verify/32_cv           2.47 %          2.45 %            10      0.97%            2.36%      2.47%
sphincs+-192f-simple/verify/32_min          2.44 ms         2.44 ms           10   11.3713M        376.921/s   6.09303M
sphincs+-192f-simple/verify/32_max          2.66 ms         2.65 ms           10    11.718M        409.659/s    6.6275M
```

### On ARM Cortex-A72 i.e. Raspberry Pi 4B

Compiled with **gcc version 13.2.0 (Ubuntu 13.2.0-4ubuntu3)**.

```bash
$ uname -srm
Linux 6.5.0-1008-raspi aarch64
```

```bash
2024-01-12T23:28:17+04:00
Running ./build/perf.out
Run on (4 X 1800 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x4)
  L1 Instruction 48 KiB (x4)
  L2 Unified 1024 KiB (x1)
Load Average: 7.92, 3.65, 1.44
------------------------------------------------------------------------------------------------------------
Benchmark                                      Time             CPU   Iterations     CYCLES items_per_second
------------------------------------------------------------------------------------------------------------
sphincs+-192f-robust/verify/32_mean         29.2 ms         29.2 ms           10   52.5442M        34.2041/s
sphincs+-192f-robust/verify/32_median       29.2 ms         29.2 ms           10   52.4877M        34.2379/s
sphincs+-192f-robust/verify/32_stddev      0.315 ms        0.315 ms           10   566.246k       0.367411/s
sphincs+-192f-robust/verify/32_cv           1.08 %          1.08 %            10      1.08%            1.07%
sphincs+-192f-robust/verify/32_min          28.8 ms         28.8 ms           10   51.8265M        33.6664/s
sphincs+-192f-robust/verify/32_max          29.7 ms         29.7 ms           10   53.3866M        34.6774/s
sphincs+-256s-simple/sign/32_mean           3757 ms         3757 ms           10   6.75193G       0.266196/s
sphincs+-256s-simple/sign/32_median         3757 ms         3757 ms           10   6.75191G       0.266199/s
sphincs+-256s-simple/sign/32_stddev        0.306 ms        0.213 ms           10   276.976k       15.1265u/s
sphincs+-256s-simple/sign/32_cv             0.01 %          0.01 %            10      0.00%            0.01%
sphincs+-256s-simple/sign/32_min            3757 ms         3756 ms           10   6.75161G       0.266164/s
sphincs+-256s-simple/sign/32_max            3758 ms         3757 ms           10   6.75242G       0.266214/s
sphincs+-192s-simple/verify/32_mean         3.59 ms         3.59 ms           10    6.4491M        278.747/s
sphincs+-192s-simple/verify/32_median       3.58 ms         3.58 ms           10   6.44003M        279.021/s
sphincs+-192s-simple/verify/32_stddev      0.071 ms        0.071 ms           10   127.691k        5.43984/s
sphincs+-192s-simple/verify/32_cv           1.98 %          1.98 %            10      1.98%            1.95%
sphincs+-192s-simple/verify/32_min          3.49 ms         3.49 ms           10   6.27699M        267.388/s
sphincs+-192s-simple/verify/32_max          3.74 ms         3.74 ms           10   6.72122M          286.3/s
sphincs+-192s-robust/verify/32_mean         11.1 ms         11.1 ms           10    19.881M        90.4526/s
sphincs+-192s-robust/verify/32_median       11.1 ms         11.1 ms           10   19.9672M         89.999/s
sphincs+-192s-robust/verify/32_stddev      0.305 ms        0.305 ms           10   547.952k        2.50817/s
sphincs+-192s-robust/verify/32_cv           2.76 %          2.76 %            10      2.76%            2.77%
sphincs+-192s-robust/verify/32_min          10.5 ms         10.5 ms           10   18.8613M        86.4954/s
sphincs+-192s-robust/verify/32_max          11.6 ms         11.6 ms           10   20.7782M        95.2843/s
sphincs+-256f-simple/sign/32_mean            379 ms          379 ms           10   680.846M        2.63983/s
sphincs+-256f-simple/sign/32_median          379 ms          379 ms           10   680.831M        2.63997/s
sphincs+-256f-simple/sign/32_stddev        0.119 ms        0.118 ms           10   220.738k       821.658u/s
sphincs+-256f-simple/sign/32_cv             0.03 %          0.03 %            10      0.03%            0.03%
sphincs+-256f-simple/sign/32_min             379 ms          379 ms           10   680.547M        2.63796/s
sphincs+-256f-simple/sign/32_max             379 ms          379 ms           10   681.326M        2.64085/s
sphincs+-128s-robust/keygen_mean             884 ms          884 ms           10   1.58877G        1.13127/s
sphincs+-128s-robust/keygen_median           884 ms          884 ms           10   1.58872G        1.13134/s
sphincs+-128s-robust/keygen_stddev         0.146 ms        0.131 ms           10   195.493k       167.023u/s
sphincs+-128s-robust/keygen_cv              0.02 %          0.01 %            10      0.01%            0.01%
sphincs+-128s-robust/keygen_min              884 ms          884 ms           10   1.58859G        1.13083/s
sphincs+-128s-robust/keygen_max              884 ms          884 ms           10   1.58929G         1.1314/s
sphincs+-192f-robust/keygen_mean            20.4 ms         20.4 ms           10   36.5784M        49.1341/s
sphincs+-192f-robust/keygen_median          20.4 ms         20.4 ms           10    36.579M        49.1315/s
sphincs+-192f-robust/keygen_stddev         0.010 ms        0.010 ms           10    17.532k      0.0243131/s
sphincs+-192f-robust/keygen_cv              0.05 %          0.05 %            10      0.05%            0.05%
sphincs+-192f-robust/keygen_min             20.3 ms         20.3 ms           10   36.5374M        49.1065/s
sphincs+-192f-robust/keygen_max             20.4 ms         20.4 ms           10   36.6029M        49.1956/s
sphincs+-256s-robust/sign/32_mean           9611 ms         9610 ms           10   17.2721G       0.104058/s
sphincs+-256s-robust/sign/32_median         9611 ms         9610 ms           10   17.2716G       0.104061/s
sphincs+-256s-robust/sign/32_stddev        0.798 ms        0.887 ms           10   1.80482M       9.60673u/s
sphincs+-256s-robust/sign/32_cv             0.01 %          0.01 %            10      0.01%            0.01%
sphincs+-256s-robust/sign/32_min            9611 ms         9609 ms           10   17.2701G       0.104034/s
sphincs+-256s-robust/sign/32_max            9613 ms         9612 ms           10   17.2768G        0.10407/s
sphincs+-128s-simple/sign/32_mean           2203 ms         2202 ms           10   3.95806G       0.454082/s
sphincs+-128s-simple/sign/32_median         2203 ms         2202 ms           10   3.95808G       0.454073/s
sphincs+-128s-simple/sign/32_stddev        0.254 ms        0.238 ms           10   435.256k       49.0597u/s
sphincs+-128s-simple/sign/32_cv             0.01 %          0.01 %            10      0.01%            0.01%
sphincs+-128s-simple/sign/32_min            2202 ms         2202 ms           10   3.95751G       0.454024/s
sphincs+-128s-simple/sign/32_max            2203 ms         2203 ms           10   3.95872G       0.454153/s
sphincs+-192s-robust/sign/32_mean          10773 ms        10771 ms           10    19.359G      0.0928392/s
sphincs+-192s-robust/sign/32_median        10773 ms        10771 ms           10    19.359G      0.0928388/s
sphincs+-192s-robust/sign/32_stddev        0.645 ms        0.521 ms           10   632.096k        4.4882u/s
sphincs+-192s-robust/sign/32_cv             0.01 %          0.00 %            10      0.00%            0.00%
sphincs+-192s-robust/sign/32_min           10772 ms        10770 ms           10   19.3579G      0.0928314/s
sphincs+-192s-robust/sign/32_max           10774 ms        10772 ms           10     19.36G      0.0928465/s
sphincs+-192f-robust/sign/32_mean            461 ms          461 ms           10   828.263M        2.16978/s
sphincs+-192f-robust/sign/32_median          461 ms          461 ms           10   828.251M        2.16964/s
sphincs+-192f-robust/sign/32_stddev        0.137 ms        0.117 ms           10   188.681k       550.978u/s
sphincs+-192f-robust/sign/32_cv             0.03 %          0.03 %            10      0.02%            0.03%
sphincs+-192f-robust/sign/32_min             461 ms          461 ms           10   827.996M        2.16912/s
sphincs+-192f-robust/sign/32_max             461 ms          461 ms           10   828.495M        2.17053/s
sphincs+-192f-simple/sign/32_mean            188 ms          188 ms           10   337.478M         5.3247/s
sphincs+-192f-simple/sign/32_median          188 ms          188 ms           10   337.471M        5.32499/s
sphincs+-192f-simple/sign/32_stddev        0.128 ms        0.121 ms           10   221.551k       3.41885m/s
sphincs+-192f-simple/sign/32_cv             0.07 %          0.06 %            10      0.07%            0.06%
sphincs+-192f-simple/sign/32_min             188 ms          188 ms           10   337.104M        5.31868/s
sphincs+-192f-simple/sign/32_max             188 ms          188 ms           10    337.88M        5.32962/s
sphincs+-256f-robust/verify/32_mean         33.5 ms         33.5 ms           10   60.1374M        29.8849/s
sphincs+-256f-robust/verify/32_median       33.6 ms         33.6 ms           10   60.3595M        29.7745/s
sphincs+-256f-robust/verify/32_stddev      0.448 ms        0.449 ms           10   806.298k       0.403911/s
sphincs+-256f-robust/verify/32_cv           1.34 %          1.34 %            10      1.34%            1.35%
sphincs+-256f-robust/verify/32_min          32.7 ms         32.7 ms           10   58.7467M         29.407/s
sphincs+-256f-robust/verify/32_max          34.0 ms         34.0 ms           10   61.1033M        30.5847/s
sphincs+-128f-simple/keygen_mean            5.01 ms         5.01 ms           10   9.00288M          199.6/s
sphincs+-128f-simple/keygen_median          5.01 ms         5.01 ms           10    9.0034M        199.605/s
sphincs+-128f-simple/keygen_stddev         0.002 ms        0.001 ms           10   1.74969k      0.0500185/s
sphincs+-128f-simple/keygen_cv              0.04 %          0.03 %            10      0.02%            0.03%
sphincs+-128f-simple/keygen_min             5.01 ms         5.01 ms           10   9.00022M        199.541/s
sphincs+-128f-simple/keygen_max             5.01 ms         5.01 ms           10   9.00534M        199.699/s
sphincs+-192s-robust/keygen_mean            1476 ms         1476 ms           10   2.65293G       0.677468/s
sphincs+-192s-robust/keygen_median          1476 ms         1476 ms           10   2.65287G       0.677478/s
sphincs+-192s-robust/keygen_stddev         0.185 ms        0.170 ms           10   249.868k       78.0824u/s
sphincs+-192s-robust/keygen_cv              0.01 %          0.01 %            10      0.01%            0.01%
sphincs+-192s-robust/keygen_min             1476 ms         1476 ms           10   2.65261G       0.677291/s
sphincs+-192s-robust/keygen_max             1477 ms         1476 ms           10   2.65355G       0.677581/s
sphincs+-192s-simple/sign/32_mean           4145 ms         4145 ms           10   7.44952G       0.241274/s
sphincs+-192s-simple/sign/32_median         4145 ms         4145 ms           10   7.44953G       0.241272/s
sphincs+-192s-simple/sign/32_stddev        0.276 ms        0.217 ms           10   357.825k        12.656u/s
sphincs+-192s-simple/sign/32_cv             0.01 %          0.01 %            10      0.00%            0.01%
sphincs+-192s-simple/sign/32_min            4145 ms         4144 ms           10   7.44895G        0.24125/s
sphincs+-192s-simple/sign/32_max            4146 ms         4145 ms           10   7.45012G       0.241292/s
sphincs+-192f-simple/verify/32_mean         10.2 ms         10.2 ms           10   18.3144M        98.1658/s
sphincs+-192f-simple/verify/32_median       10.2 ms         10.2 ms           10   18.2726M        98.3337/s
sphincs+-192f-simple/verify/32_stddev      0.244 ms        0.244 ms           10   437.779k        2.36102/s
sphincs+-192f-simple/verify/32_cv           2.40 %          2.40 %            10      2.39%            2.41%
sphincs+-192f-simple/verify/32_min          9.76 ms         9.76 ms           10   17.5367M        94.4989/s
sphincs+-192f-simple/verify/32_max          10.6 ms         10.6 ms           10   19.0174M        102.469/s
sphincs+-128s-simple/keygen_mean             321 ms          320 ms           10   575.972M        3.12029/s
sphincs+-128s-simple/keygen_median           321 ms          321 ms           10   576.046M        3.11984/s
sphincs+-128s-simple/keygen_stddev         0.161 ms        0.134 ms           10   217.014k       1.30309m/s
sphincs+-128s-simple/keygen_cv              0.05 %          0.04 %            10      0.04%            0.04%
sphincs+-128s-simple/keygen_min              320 ms          320 ms           10   575.557M        3.11909/s
sphincs+-128s-simple/keygen_max              321 ms          321 ms           10   576.168M        3.12252/s
sphincs+-256f-robust/sign/32_mean           1044 ms         1044 ms           10   1.87618G       0.957925/s
sphincs+-256f-robust/sign/32_median         1044 ms         1044 ms           10   1.87608G       0.957973/s
sphincs+-256f-robust/sign/32_stddev        0.333 ms        0.335 ms           10   618.912k       307.615u/s
sphincs+-256f-robust/sign/32_cv             0.03 %          0.03 %            10      0.03%            0.03%
sphincs+-256f-robust/sign/32_min            1044 ms         1043 ms           10    1.8753G       0.957384/s
sphincs+-256f-robust/sign/32_max            1045 ms         1045 ms           10   1.87718G       0.958403/s
sphincs+-256f-robust/keygen_mean            60.6 ms         60.6 ms           10   108.892M        16.5027/s
sphincs+-256f-robust/keygen_median          60.6 ms         60.6 ms           10   108.889M        16.5037/s
sphincs+-256f-robust/keygen_stddev         0.025 ms        0.021 ms           10    30.144k       5.76279m/s
sphincs+-256f-robust/keygen_cv              0.04 %          0.03 %            10      0.03%            0.03%
sphincs+-256f-robust/keygen_min             60.6 ms         60.6 ms           10   108.849M        16.4928/s
sphincs+-256f-robust/keygen_max             60.6 ms         60.6 ms           10   108.956M        16.5109/s
sphincs+-256s-simple/keygen_mean             310 ms          310 ms           10   556.533M        3.22939/s
sphincs+-256s-simple/keygen_median           310 ms          310 ms           10    556.56M        3.22921/s
sphincs+-256s-simple/keygen_stddev         0.112 ms        0.113 ms           10   212.021k       1.17704m/s
sphincs+-256s-simple/keygen_cv              0.04 %          0.04 %            10      0.04%            0.04%
sphincs+-256s-simple/keygen_min              309 ms          309 ms           10   556.234M        3.22758/s
sphincs+-256s-simple/keygen_max              310 ms          310 ms           10   556.872M        3.23116/s
sphincs+-128s-robust/verify/32_mean         6.63 ms         6.63 ms           10   11.9081M        151.038/s
sphincs+-128s-robust/verify/32_median       6.64 ms         6.64 ms           10   11.9309M        150.638/s
sphincs+-128s-robust/verify/32_stddev      0.207 ms        0.207 ms           10   371.353k        4.71517/s
sphincs+-128s-robust/verify/32_cv           3.12 %          3.12 %            10      3.12%            3.12%
sphincs+-128s-robust/verify/32_min          6.28 ms         6.28 ms           10   11.2947M        142.958/s
sphincs+-128s-robust/verify/32_max          7.00 ms         7.00 ms           10   12.5684M        159.129/s
sphincs+-256f-simple/verify/32_mean         10.2 ms         10.2 ms           10   18.3038M        98.1893/s
sphincs+-256f-simple/verify/32_median       10.2 ms         10.2 ms           10   18.3288M        98.0286/s
sphincs+-256f-simple/verify/32_stddev      0.150 ms        0.150 ms           10   269.937k        1.45888/s
sphincs+-256f-simple/verify/32_cv           1.47 %          1.48 %            10      1.47%            1.49%
sphincs+-256f-simple/verify/32_min          9.93 ms         9.92 ms           10   17.8289M          96.29/s
sphincs+-256f-simple/verify/32_max          10.4 ms         10.4 ms           10   18.6597M        100.767/s
sphincs+-256s-robust/verify/32_mean         16.5 ms         16.5 ms           10   29.6917M        60.5499/s
sphincs+-256s-robust/verify/32_median       16.6 ms         16.6 ms           10   29.8024M        60.3059/s
sphincs+-256s-robust/verify/32_stddev      0.352 ms        0.351 ms           10   630.552k        1.28124/s
sphincs+-256s-robust/verify/32_cv           2.13 %          2.13 %            10      2.12%            2.12%
sphincs+-256s-robust/verify/32_min          16.1 ms         16.1 ms           10   28.9572M          58.29/s
sphincs+-256s-robust/verify/32_max          17.2 ms         17.2 ms           10   30.8239M        62.0663/s
sphincs+-128f-robust/keygen_mean            13.8 ms         13.8 ms           10   24.8303M        72.3769/s
sphincs+-128f-robust/keygen_median          13.8 ms         13.8 ms           10   24.8305M        72.3734/s
sphincs+-128f-robust/keygen_stddev         0.003 ms        0.002 ms           10   2.47394k       0.010313/s
sphincs+-128f-robust/keygen_cv              0.02 %          0.01 %            10      0.01%            0.01%
sphincs+-128f-robust/keygen_min             13.8 ms         13.8 ms           10   24.8261M        72.3687/s
sphincs+-128f-robust/keygen_max             13.8 ms         13.8 ms           10   24.8334M        72.4024/s
sphincs+-128f-simple/verify/32_mean         9.13 ms         9.13 ms           10   16.4055M        109.646/s
sphincs+-128f-simple/verify/32_median       9.10 ms         9.10 ms           10   16.3469M        109.928/s
sphincs+-128f-simple/verify/32_stddev      0.301 ms        0.301 ms           10   540.292k        3.60203/s
sphincs+-128f-simple/verify/32_cv           3.30 %          3.30 %            10      3.29%            3.29%
sphincs+-128f-simple/verify/32_min          8.63 ms         8.63 ms           10   15.5097M         103.48/s
sphincs+-128f-simple/verify/32_max          9.66 ms         9.66 ms           10   17.3681M        115.885/s
sphincs+-192s-simple/keygen_mean             486 ms          486 ms           10   873.566M        2.05734/s
sphincs+-192s-simple/keygen_median           486 ms          486 ms           10   873.567M        2.05728/s
sphincs+-192s-simple/keygen_stddev         0.159 ms        0.156 ms           10   271.325k       659.867u/s
sphincs+-192s-simple/keygen_cv              0.03 %          0.03 %            10      0.03%            0.03%
sphincs+-192s-simple/keygen_min              486 ms          486 ms           10   873.193M        2.05647/s
sphincs+-192s-simple/keygen_max              486 ms          486 ms           10   873.958M        2.05823/s
sphincs+-128f-robust/sign/32_mean            271 ms          270 ms           10   486.071M        3.69712/s
sphincs+-128f-robust/sign/32_median          270 ms          270 ms           10    486.09M        3.69715/s
sphincs+-128f-robust/sign/32_stddev        0.113 ms        0.088 ms           10    134.21k       1.20834m/s
sphincs+-128f-robust/sign/32_cv             0.04 %          0.03 %            10      0.03%            0.03%
sphincs+-128f-robust/sign/32_min             270 ms          270 ms           10   485.891M        3.69513/s
sphincs+-128f-robust/sign/32_max             271 ms          271 ms           10   486.265M        3.69906/s
sphincs+-128s-robust/sign/32_mean           5465 ms         5464 ms           10   9.82046G        0.18301/s
sphincs+-128s-robust/sign/32_median         5465 ms         5464 ms           10   9.82043G        0.18301/s
sphincs+-128s-robust/sign/32_stddev        0.166 ms        0.141 ms           10   294.814k       4.70817u/s
sphincs+-128s-robust/sign/32_cv             0.00 %          0.00 %            10      0.00%            0.00%
sphincs+-128s-robust/sign/32_min            5465 ms         5464 ms           10   9.82011G          0.183/s
sphincs+-128s-robust/sign/32_max            5465 ms         5464 ms           10     9.821G       0.183017/s
sphincs+-128s-simple/verify/32_mean         3.01 ms         3.01 ms           10   5.40181M        332.845/s
sphincs+-128s-simple/verify/32_median       3.01 ms         3.01 ms           10   5.40749M        332.315/s
sphincs+-128s-simple/verify/32_stddev      0.076 ms        0.077 ms           10   138.072k        8.53613/s
sphincs+-128s-simple/verify/32_cv           2.54 %          2.55 %            10      2.56%            2.56%
sphincs+-128s-simple/verify/32_min          2.89 ms         2.89 ms           10   5.18755M        321.462/s
sphincs+-128s-simple/verify/32_max          3.11 ms         3.11 ms           10   5.58958M        346.409/s
sphincs+-128f-robust/verify/32_mean         19.7 ms         19.7 ms           10   35.3166M        50.9193/s
sphincs+-128f-robust/verify/32_median       19.9 ms         19.9 ms           10   35.6877M        50.3482/s
sphincs+-128f-robust/verify/32_stddev      0.523 ms        0.524 ms           10   943.007k        1.40489/s
sphincs+-128f-robust/verify/32_cv           2.66 %          2.67 %            10      2.67%            2.76%
sphincs+-128f-robust/verify/32_min          18.5 ms         18.5 ms           10   33.1682M        49.3277/s
sphincs+-128f-robust/verify/32_max          20.3 ms         20.3 ms           10   36.4365M        54.1802/s
sphincs+-256s-robust/keygen_mean             978 ms          978 ms           10   1.75689G          1.023/s
sphincs+-256s-robust/keygen_median           978 ms          978 ms           10   1.75694G        1.02298/s
sphincs+-256s-robust/keygen_stddev         0.190 ms        0.186 ms           10   362.628k       194.701u/s
sphincs+-256s-robust/keygen_cv              0.02 %          0.02 %            10      0.02%            0.02%
sphincs+-256s-robust/keygen_min              977 ms          977 ms           10   1.75612G        1.02275/s
sphincs+-256s-robust/keygen_max              978 ms          978 ms           10   1.75734G        1.02345/s
sphincs+-128f-simple/sign/32_mean            120 ms          120 ms           10   215.957M        8.32063/s
sphincs+-128f-simple/sign/32_median          120 ms          120 ms           10   215.943M        8.32141/s
sphincs+-128f-simple/sign/32_stddev        0.036 ms        0.043 ms           10   86.1536k       2.98705m/s
sphincs+-128f-simple/sign/32_cv             0.03 %          0.04 %            10      0.04%            0.04%
sphincs+-128f-simple/sign/32_min             120 ms          120 ms           10   215.836M        8.31591/s
sphincs+-128f-simple/sign/32_max             120 ms          120 ms           10   216.091M        8.32442/s
sphincs+-256s-simple/verify/32_mean         5.06 ms         5.06 ms           10   9.08566M        197.822/s
sphincs+-256s-simple/verify/32_median       5.05 ms         5.05 ms           10   9.07831M        197.898/s
sphincs+-256s-simple/verify/32_stddev      0.091 ms        0.091 ms           10   164.307k        3.54907/s
sphincs+-256s-simple/verify/32_cv           1.80 %          1.80 %            10      1.81%            1.79%
sphincs+-256s-simple/verify/32_min          4.91 ms         4.91 ms           10   8.82804M        191.356/s
sphincs+-256s-simple/verify/32_max          5.23 ms         5.23 ms           10   9.39158M        203.543/s
sphincs+-192f-simple/keygen_mean            7.62 ms         7.62 ms           10   13.6917M         131.24/s
sphincs+-192f-simple/keygen_median          7.62 ms         7.62 ms           10   13.6914M        131.247/s
sphincs+-192f-simple/keygen_stddev         0.008 ms        0.007 ms           10   13.1032k       0.128408/s
sphincs+-192f-simple/keygen_cv              0.10 %          0.10 %            10      0.10%            0.10%
sphincs+-192f-simple/keygen_min             7.61 ms         7.61 ms           10   13.6672M        131.036/s
sphincs+-192f-simple/keygen_max             7.63 ms         7.63 ms           10   13.7111M         131.48/s
sphincs+-256f-simple/keygen_mean            19.4 ms         19.4 ms           10   34.8104M        51.6241/s
sphincs+-256f-simple/keygen_median          19.4 ms         19.4 ms           10    34.808M        51.6292/s
sphincs+-256f-simple/keygen_stddev         0.008 ms        0.008 ms           10   13.7667k      0.0207193/s
sphincs+-256f-simple/keygen_cv              0.04 %          0.04 %            10      0.04%            0.04%
sphincs+-256f-simple/keygen_min             19.4 ms         19.4 ms           10   34.7937M        51.5876/s
sphincs+-256f-simple/keygen_max             19.4 ms         19.4 ms           10   34.8376M        51.6496/s
```

### On Apple M1 Max

Compiled with **Apple clang version 15.0.0 (clang-1500.1.0.2.5)**.

```bash
uname -srm
Darwin 23.2.0 arm64
```

```bash
2024-01-22T20:57:36+04:00
Running ./build/bench.out
Run on (10 X 24 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB
  L1 Instruction 128 KiB
  L2 Unified 4096 KiB (x10)
Load Average: 4.08, 1.88, 1.82
-------------------------------------------------------------------------------------------------
Benchmark                                      Time             CPU   Iterations items_per_second
-------------------------------------------------------------------------------------------------
sphincs+-256s-simple/sign/32_mean            956 ms          955 ms           10        1.04694/s
sphincs+-256s-simple/sign/32_median          952 ms          951 ms           10        1.05179/s
sphincs+-256s-simple/sign/32_stddev         14.2 ms         14.3 ms           10      0.0151226/s
sphincs+-256s-simple/sign/32_cv             1.48 %          1.50 %            10            1.44%
sphincs+-256s-simple/sign/32_min             952 ms          951 ms           10         1.0039/s
sphincs+-256s-simple/sign/32_max             997 ms          996 ms           10        1.05184/s
sphincs+-192f-simple/verify/32_mean         2.63 ms         2.63 ms           10         380.76/s
sphincs+-192f-simple/verify/32_median       2.64 ms         2.63 ms           10        379.595/s
sphincs+-192f-simple/verify/32_stddev      0.047 ms        0.047 ms           10        6.92005/s
sphincs+-192f-simple/verify/32_cv           1.80 %          1.81 %            10            1.82%
sphincs+-192f-simple/verify/32_min          2.55 ms         2.55 ms           10        370.813/s
sphincs+-192f-simple/verify/32_max          2.70 ms         2.70 ms           10        392.735/s
sphincs+-192s-simple/sign/32_mean           1088 ms         1087 ms           10       0.920116/s
sphincs+-192s-simple/sign/32_median         1088 ms         1087 ms           10       0.920072/s
sphincs+-192s-simple/sign/32_stddev        0.163 ms        0.110 ms           10        93.278u/s
sphincs+-192s-simple/sign/32_cv             0.01 %          0.01 %            10            0.01%
sphincs+-192s-simple/sign/32_min            1088 ms         1087 ms           10       0.920022/s
sphincs+-192s-simple/sign/32_max            1088 ms         1087 ms           10       0.920261/s
sphincs+-128s-simple/keygen_mean            83.2 ms         83.1 ms           10        12.0375/s
sphincs+-128s-simple/keygen_median          83.2 ms         83.1 ms           10        12.0375/s
sphincs+-128s-simple/keygen_stddev         0.022 ms        0.005 ms           10       733.148u/s
sphincs+-128s-simple/keygen_cv              0.03 %          0.01 %            10            0.01%
sphincs+-128s-simple/keygen_min             83.1 ms         83.1 ms           10        12.0364/s
sphincs+-128s-simple/keygen_max             83.2 ms         83.1 ms           10        12.0387/s
sphincs+-256s-robust/sign/32_mean           1726 ms         1724 ms           10       0.580078/s
sphincs+-256s-robust/sign/32_median         1719 ms         1716 ms           10       0.582602/s
sphincs+-256s-robust/sign/32_stddev         18.0 ms         18.0 ms           10       5.92884m/s
sphincs+-256s-robust/sign/32_cv             1.04 %          1.04 %            10            1.02%
sphincs+-256s-robust/sign/32_min            1718 ms         1716 ms           10       0.564604/s
sphincs+-256s-robust/sign/32_max            1774 ms         1771 ms           10       0.582721/s
sphincs+-128f-simple/keygen_mean            1.30 ms         1.30 ms           10        770.521/s
sphincs+-128f-simple/keygen_median          1.30 ms         1.30 ms           10        770.518/s
sphincs+-128f-simple/keygen_stddev         0.001 ms        0.000 ms           10      0.0452933/s
sphincs+-128f-simple/keygen_cv              0.04 %          0.01 %            10            0.01%
sphincs+-128f-simple/keygen_min             1.30 ms         1.30 ms           10         770.46/s
sphincs+-128f-simple/keygen_max             1.30 ms         1.30 ms           10        770.625/s
sphincs+-256f-simple/keygen_mean            5.01 ms         5.01 ms           10        199.687/s
sphincs+-256f-simple/keygen_median          5.01 ms         5.01 ms           10        199.689/s
sphincs+-256f-simple/keygen_stddev         0.001 ms        0.000 ms           10      0.0144266/s
sphincs+-256f-simple/keygen_cv              0.03 %          0.01 %            10            0.01%
sphincs+-256f-simple/keygen_min             5.01 ms         5.01 ms           10        199.663/s
sphincs+-256f-simple/keygen_max             5.02 ms         5.01 ms           10        199.712/s
sphincs+-256s-robust/verify/32_mean         2.55 ms         2.55 ms           10        392.218/s
sphincs+-256s-robust/verify/32_median       2.56 ms         2.55 ms           10        391.584/s
sphincs+-256s-robust/verify/32_stddev      0.040 ms        0.040 ms           10        6.19041/s
sphincs+-256s-robust/verify/32_cv           1.59 %          1.57 %            10            1.58%
sphincs+-256s-robust/verify/32_min          2.48 ms         2.48 ms           10        383.864/s
sphincs+-256s-robust/verify/32_max          2.61 ms         2.61 ms           10        403.425/s
sphincs+-256s-simple/keygen_mean            80.2 ms         80.1 ms           10        12.4794/s
sphincs+-256s-simple/keygen_median          80.2 ms         80.1 ms           10        12.4797/s
sphincs+-256s-simple/keygen_stddev         0.030 ms        0.012 ms           10       1.88862m/s
sphincs+-256s-simple/keygen_cv              0.04 %          0.02 %            10            0.02%
sphincs+-256s-simple/keygen_min             80.2 ms         80.1 ms           10        12.4759/s
sphincs+-256s-simple/keygen_max             80.3 ms         80.2 ms           10        12.4817/s
sphincs+-192f-simple/keygen_mean            1.90 ms         1.90 ms           10        527.157/s
sphincs+-192f-simple/keygen_median          1.90 ms         1.90 ms           10        527.158/s
sphincs+-192f-simple/keygen_stddev         0.001 ms        0.000 ms           10      0.0439028/s
sphincs+-192f-simple/keygen_cv              0.04 %          0.01 %            10            0.01%
sphincs+-192f-simple/keygen_min             1.90 ms         1.90 ms           10        527.081/s
sphincs+-192f-simple/keygen_max             1.90 ms         1.90 ms           10        527.223/s
sphincs+-128s-robust/sign/32_mean           1173 ms         1172 ms           10       0.853268/s
sphincs+-128s-robust/sign/32_median         1173 ms         1172 ms           10       0.853432/s
sphincs+-128s-robust/sign/32_stddev        0.992 ms        0.812 ms           10       590.275u/s
sphincs+-128s-robust/sign/32_cv             0.08 %          0.07 %            10            0.07%
sphincs+-128s-robust/sign/32_min            1173 ms         1172 ms           10       0.851615/s
sphincs+-128s-robust/sign/32_max            1176 ms         1174 ms           10       0.853585/s
sphincs+-192f-robust/keygen_mean            3.56 ms         3.56 ms           10        281.038/s
sphincs+-192f-robust/keygen_median          3.56 ms         3.56 ms           10        281.015/s
sphincs+-192f-robust/keygen_stddev         0.001 ms        0.001 ms           10      0.0645186/s
sphincs+-192f-robust/keygen_cv              0.03 %          0.02 %            10            0.02%
sphincs+-192f-robust/keygen_min             3.56 ms         3.56 ms           10        280.978/s
sphincs+-192f-robust/keygen_max             3.56 ms         3.56 ms           10        281.162/s
sphincs+-192f-simple/sign/32_mean           48.9 ms         48.8 ms           10        20.4728/s
sphincs+-192f-simple/sign/32_median         48.9 ms         48.8 ms           10        20.4818/s
sphincs+-192f-simple/sign/32_stddev        0.073 ms        0.069 ms           10      0.0286755/s
sphincs+-192f-simple/sign/32_cv             0.15 %          0.14 %            10            0.14%
sphincs+-192f-simple/sign/32_min            48.8 ms         48.8 ms           10        20.3917/s
sphincs+-192f-simple/sign/32_max            49.1 ms         49.0 ms           10        20.4852/s
sphincs+-128f-simple/sign/32_mean           30.3 ms         30.3 ms           10        33.0301/s
sphincs+-128f-simple/sign/32_median         30.3 ms         30.3 ms           10        33.0311/s
sphincs+-128f-simple/sign/32_stddev        0.013 ms        0.010 ms           10       0.010819/s
sphincs+-128f-simple/sign/32_cv             0.04 %          0.03 %            10            0.03%
sphincs+-128f-simple/sign/32_min            30.3 ms         30.3 ms           10        33.0129/s
sphincs+-128f-simple/sign/32_max            30.3 ms         30.3 ms           10        33.0478/s
sphincs+-192f-robust/verify/32_mean         5.09 ms         5.08 ms           10        196.916/s
sphincs+-192f-robust/verify/32_median       5.09 ms         5.08 ms           10        196.843/s
sphincs+-192f-robust/verify/32_stddev      0.107 ms        0.107 ms           10        4.14326/s
sphincs+-192f-robust/verify/32_cv           2.11 %          2.11 %            10            2.10%
sphincs+-192f-robust/verify/32_min          4.92 ms         4.92 ms           10        190.444/s
sphincs+-192f-robust/verify/32_max          5.26 ms         5.25 ms           10        203.318/s
sphincs+-128f-robust/verify/32_mean         3.44 ms         3.44 ms           10        290.687/s
sphincs+-128f-robust/verify/32_median       3.45 ms         3.44 ms           10        290.344/s
sphincs+-128f-robust/verify/32_stddev      0.042 ms        0.043 ms           10        3.61899/s
sphincs+-128f-robust/verify/32_cv           1.23 %          1.24 %            10            1.24%
sphincs+-128f-robust/verify/32_min          3.37 ms         3.37 ms           10        286.211/s
sphincs+-128f-robust/verify/32_max          3.50 ms         3.49 ms           10        296.826/s
sphincs+-128s-simple/sign/32_mean            631 ms          630 ms           10        1.58651/s
sphincs+-128s-simple/sign/32_median          631 ms          630 ms           10        1.58658/s
sphincs+-128s-simple/sign/32_stddev        0.207 ms        0.118 ms           10       297.925u/s
sphincs+-128s-simple/sign/32_cv             0.03 %          0.02 %            10            0.02%
sphincs+-128s-simple/sign/32_min             631 ms          630 ms           10        1.58568/s
sphincs+-128s-simple/sign/32_max             632 ms          631 ms           10        1.58671/s
sphincs+-128f-robust/sign/32_mean           56.4 ms         56.3 ms           10        17.7478/s
sphincs+-128f-robust/sign/32_median         56.4 ms         56.3 ms           10        17.7467/s
sphincs+-128f-robust/sign/32_stddev        0.026 ms        0.026 ms           10        8.2731m/s
sphincs+-128f-robust/sign/32_cv             0.05 %          0.05 %            10            0.05%
sphincs+-128f-robust/sign/32_min            56.4 ms         56.3 ms           10        17.7384/s
sphincs+-128f-robust/sign/32_max            56.4 ms         56.4 ms           10        17.7621/s
sphincs+-128f-simple/verify/32_mean         1.80 ms         1.80 ms           10        556.312/s
sphincs+-128f-simple/verify/32_median       1.79 ms         1.79 ms           10         559.06/s
sphincs+-128f-simple/verify/32_stddev      0.049 ms        0.049 ms           10        14.9518/s
sphincs+-128f-simple/verify/32_cv           2.70 %          2.71 %            10            2.69%
sphincs+-128f-simple/verify/32_min          1.73 ms         1.72 ms           10        532.058/s
sphincs+-128f-simple/verify/32_max          1.88 ms         1.88 ms           10        580.051/s
sphincs+-192s-simple/keygen_mean             121 ms          121 ms           10        8.24687/s
sphincs+-192s-simple/keygen_median           121 ms          121 ms           10        8.24698/s
sphincs+-192s-simple/keygen_stddev         0.033 ms        0.011 ms           10        772.41u/s
sphincs+-192s-simple/keygen_cv              0.03 %          0.01 %            10            0.01%
sphincs+-192s-simple/keygen_min              121 ms          121 ms           10        8.24579/s
sphincs+-192s-simple/keygen_max              121 ms          121 ms           10        8.24797/s
sphincs+-256f-simple/sign/32_mean            101 ms          100 ms           10        9.96101/s
sphincs+-256f-simple/sign/32_median          100 ms          100 ms           10        9.96204/s
sphincs+-256f-simple/sign/32_stddev        0.053 ms        0.039 ms           10       3.91328m/s
sphincs+-256f-simple/sign/32_cv             0.05 %          0.04 %            10            0.04%
sphincs+-256f-simple/sign/32_min             100 ms          100 ms           10        9.95045/s
sphincs+-256f-simple/sign/32_max             101 ms          100 ms           10        9.96393/s
sphincs+-192s-robust/sign/32_mean           1988 ms         1985 ms           10       0.503742/s
sphincs+-192s-robust/sign/32_median         1986 ms         1983 ms           10       0.504245/s
sphincs+-192s-robust/sign/32_stddev         6.13 ms         5.98 ms           10       1.50493m/s
sphincs+-192s-robust/sign/32_cv             0.31 %          0.30 %            10            0.30%
sphincs+-192s-robust/sign/32_min            1985 ms         1983 ms           10        0.49947/s
sphincs+-192s-robust/sign/32_max            2005 ms         2002 ms           10       0.504295/s
sphincs+-128s-simple/verify/32_mean        0.618 ms        0.617 ms           10       1.62283k/s
sphincs+-128s-simple/verify/32_median      0.616 ms        0.615 ms           10       1.62509k/s
sphincs+-128s-simple/verify/32_stddev      0.022 ms        0.022 ms           10        57.5421/s
sphincs+-128s-simple/verify/32_cv           3.57 %          3.56 %            10            3.55%
sphincs+-128s-simple/verify/32_min         0.586 ms        0.585 ms           10       1.52839k/s
sphincs+-128s-simple/verify/32_max         0.655 ms        0.654 ms           10       1.70824k/s
sphincs+-128s-robust/verify/32_mean         1.17 ms         1.17 ms           10        854.079/s
sphincs+-128s-robust/verify/32_median       1.17 ms         1.17 ms           10        857.166/s
sphincs+-128s-robust/verify/32_stddev      0.031 ms        0.031 ms           10        22.3277/s
sphincs+-128s-robust/verify/32_cv           2.64 %          2.65 %            10            2.61%
sphincs+-128s-robust/verify/32_min          1.13 ms         1.13 ms           10        813.641/s
sphincs+-128s-robust/verify/32_max          1.23 ms         1.23 ms           10        885.665/s
sphincs+-256f-robust/sign/32_mean            186 ms          186 ms           10        5.38278/s
sphincs+-256f-robust/sign/32_median          186 ms          186 ms           10        5.39002/s
sphincs+-256f-robust/sign/32_stddev        0.715 ms        0.747 ms           10      0.0214275/s
sphincs+-256f-robust/sign/32_cv             0.38 %          0.40 %            10            0.40%
sphincs+-256f-robust/sign/32_min             186 ms          186 ms           10        5.32189/s
sphincs+-256f-robust/sign/32_max             188 ms          188 ms           10        5.39049/s
sphincs+-128s-robust/keygen_mean             156 ms          156 ms           10        6.41065/s
sphincs+-128s-robust/keygen_median           156 ms          156 ms           10        6.41071/s
sphincs+-128s-robust/keygen_stddev         0.052 ms        0.014 ms           10       574.443u/s
sphincs+-128s-robust/keygen_cv              0.03 %          0.01 %            10            0.01%
sphincs+-128s-robust/keygen_min              156 ms          156 ms           10        6.40968/s
sphincs+-128s-robust/keygen_max              156 ms          156 ms           10        6.41161/s
sphincs+-256f-simple/verify/32_mean         2.68 ms         2.68 ms           10        373.394/s
sphincs+-256f-simple/verify/32_median       2.69 ms         2.69 ms           10        371.807/s
sphincs+-256f-simple/verify/32_stddev      0.047 ms        0.047 ms           10        6.60918/s
sphincs+-256f-simple/verify/32_cv           1.75 %          1.77 %            10            1.77%
sphincs+-256f-simple/verify/32_min          2.61 ms         2.60 ms           10        361.898/s
sphincs+-256f-simple/verify/32_max          2.76 ms         2.76 ms           10        384.206/s
sphincs+-192s-robust/verify/32_mean         1.73 ms         1.72 ms           10        580.372/s
sphincs+-192s-robust/verify/32_median       1.74 ms         1.74 ms           10         575.88/s
sphincs+-192s-robust/verify/32_stddev      0.040 ms        0.040 ms           10        13.6261/s
sphincs+-192s-robust/verify/32_cv           2.32 %          2.33 %            10            2.35%
sphincs+-192s-robust/verify/32_min          1.66 ms         1.66 ms           10        561.004/s
sphincs+-192s-robust/verify/32_max          1.79 ms         1.78 ms           10        603.469/s
sphincs+-256s-simple/verify/32_mean         1.32 ms         1.32 ms           10        758.553/s
sphincs+-256s-simple/verify/32_median       1.32 ms         1.32 ms           10        759.077/s
sphincs+-256s-simple/verify/32_stddev      0.023 ms        0.023 ms           10        13.5255/s
sphincs+-256s-simple/verify/32_cv           1.77 %          1.77 %            10            1.78%
sphincs+-256s-simple/verify/32_min          1.28 ms         1.27 ms           10        741.999/s
sphincs+-256s-simple/verify/32_max          1.35 ms         1.35 ms           10        784.793/s
sphincs+-256f-robust/verify/32_mean         5.19 ms         5.18 ms           10        192.933/s
sphincs+-256f-robust/verify/32_median       5.19 ms         5.18 ms           10         193.06/s
sphincs+-256f-robust/verify/32_stddev      0.097 ms        0.097 ms           10        3.63415/s
sphincs+-256f-robust/verify/32_cv           1.88 %          1.87 %            10            1.88%
sphincs+-256f-robust/verify/32_min          5.01 ms         5.00 ms           10         187.86/s
sphincs+-256f-robust/verify/32_max          5.33 ms         5.32 ms           10        199.816/s
sphincs+-192f-robust/sign/32_mean           90.0 ms         89.9 ms           10        11.1264/s
sphincs+-192f-robust/sign/32_median         90.0 ms         89.9 ms           10        11.1257/s
sphincs+-192f-robust/sign/32_stddev        0.024 ms        0.016 ms           10       2.01766m/s
sphincs+-192f-robust/sign/32_cv             0.03 %          0.02 %            10            0.02%
sphincs+-192f-robust/sign/32_min            89.9 ms         89.8 ms           10        11.1239/s
sphincs+-192f-robust/sign/32_max            90.0 ms         89.9 ms           10        11.1303/s
sphincs+-192s-simple/verify/32_mean        0.902 ms        0.901 ms           10       1.11103k/s
sphincs+-192s-simple/verify/32_median      0.897 ms        0.896 ms           10       1.11658k/s
sphincs+-192s-simple/verify/32_stddev      0.029 ms        0.029 ms           10        34.9975/s
sphincs+-192s-simple/verify/32_cv           3.20 %          3.19 %            10            3.15%
sphincs+-192s-simple/verify/32_min         0.871 ms        0.870 ms           10       1.05571k/s
sphincs+-192s-simple/verify/32_max         0.948 ms        0.947 ms           10       1.14974k/s
sphincs+-256s-robust/keygen_mean             152 ms          151 ms           10         6.6089/s
sphincs+-256s-robust/keygen_median           151 ms          151 ms           10        6.60882/s
sphincs+-256s-robust/keygen_stddev         0.050 ms        0.015 ms           10       639.449u/s
sphincs+-256s-robust/keygen_cv              0.03 %          0.01 %            10            0.01%
sphincs+-256s-robust/keygen_min              151 ms          151 ms           10        6.60812/s
sphincs+-256s-robust/keygen_max              152 ms          151 ms           10        6.60995/s
sphincs+-256f-robust/keygen_mean            9.47 ms         9.46 ms           10        105.719/s
sphincs+-256f-robust/keygen_median          9.47 ms         9.46 ms           10        105.724/s
sphincs+-256f-robust/keygen_stddev         0.005 ms        0.003 ms           10      0.0296424/s
sphincs+-256f-robust/keygen_cv              0.05 %          0.03 %            10            0.03%
sphincs+-256f-robust/keygen_min             9.47 ms         9.46 ms           10        105.645/s
sphincs+-256f-robust/keygen_max             9.48 ms         9.47 ms           10        105.746/s
sphincs+-192s-robust/keygen_mean             228 ms          228 ms           10        4.39032/s
sphincs+-192s-robust/keygen_median           228 ms          228 ms           10        4.39021/s
sphincs+-192s-robust/keygen_stddev         0.070 ms        0.048 ms           10       929.561u/s
sphincs+-192s-robust/keygen_cv              0.03 %          0.02 %            10            0.02%
sphincs+-192s-robust/keygen_min              228 ms          228 ms           10        4.38935/s
sphincs+-192s-robust/keygen_max              228 ms          228 ms           10        4.39282/s
sphincs+-128f-robust/keygen_mean            2.44 ms         2.44 ms           10        410.636/s
sphincs+-128f-robust/keygen_median          2.44 ms         2.44 ms           10        410.644/s
sphincs+-128f-robust/keygen_stddev         0.001 ms        0.000 ms           10      0.0326569/s
sphincs+-128f-robust/keygen_cv              0.04 %          0.01 %            10            0.01%
sphincs+-128f-robust/keygen_min             2.44 ms         2.44 ms           10        410.568/s
sphincs+-128f-robust/keygen_max             2.44 ms         2.44 ms           10        410.672/s
```


## Usage

`sphincs+` is a header-only C++20 library which is pretty easy to use. To get started

- Clone SPHINCS+ repository

```bash
cd

# Multi-step cloning and importing of submodules
git clone https://github.com/itzmeanjan/sphincs-plus.git && pushd sphincs-plus && git submodule update --init && popd
# Or do single step cloning and importing of submodules
git clone https://github.com/itzmeanjan/sphincs-plus.git --recurse-submodules
```

- Write a program which makes use of SPHINCS+-{128,192,256}{s,f}-{robust,simple} key generation, signing and verification API ( all of these routines and constants, representing how many bytes to allocate for holding public key, secret key and signature, live under `sphincs_plus_{128,192,256}{s,f}_{robust,simple}::` namespace ), while importing correct header file.
- Finally compile your program, while letting your compiler know where it can find SPHINCS+ and Sha3 headers.

```bash
# Assuming `sphincs-plus` was cloned just under $HOME

SPHINCS_PLUS_HEADERS=~/sphincs-plus/include
SHA3_HEADERS=~/sphincs-plus/sha3/include

g++ -std=c++20 -Wall -Wextra -pedantic -O3 -march=native -I $SPHINCS_PLUS_HEADERS -I $SHA3_HEADERS main.cpp
```

> [!TIP]
> For recommended parameter sets, find table 3 of SPHINCS+ [specification](https://sphincs.org/data/sphincs+-r3.1-specification.pdf).

See the example program [sphincs+_shake_128s_robust.cpp](./examples/sphincs+_shake_128s_robust.cpp), demonstrating usage of keygen/ sign/ verify API.

```bash
pushd examples; g++ -std=c++20 -Wall -Wextra -pedantic -O3 -march=native -I ../include -I ../sha3/include sphincs+_shake_128s_robust.cpp && ./a.out; popd

SPHINCS+-SHAKE-128s-robust @ NIST Security Level 1
Secret Key   : 49a5ca3d5f151f2c9c0f197d324724c9c2313bdb69720a1bc615434b4874ba41949e000ab275dd6da569ff32757c021586eb2036e1ba14483e809ffa0c5505f5
Public Key   : 949e000ab275dd6da569ff32757c021586eb2036e1ba14483e809ffa0c5505f5
Message      : 80bce832fab336eefdceef28ab89838667d164d255298fe888344760199a8c3c
Signature    : 3afabef1627a6130cc3b020b37e46185f4e711073c79340c8cc67b780ae019ac90ae7d3d4b49fb9ebe3c02c5de2749a74f7d9b5f1fc338dbdb6af4c9dd80f4c2878d2a67b03a5cd00405532accfc030f3f7aa2338156b8cb1af6b6836247f5f7d337c06c9f686077d576388257ef9df0ad1c77675cee73840c44f06dce3c74bead5e55b74f4a143337c437c675cff6394b9bd116649e2155b908d4f32d875fe525ee23a666761798a5364d91b55f4c7cf7d8a78cb99a04e4469edcb55efa9a27e70cae8742ab9199b4558d6ab8033bc648aee3744d93c7e502b07ef5f00be9b9e8582ca6b86a0c0d764b8236ab776c33d9debfa8c0520bdd9ae290805778503a6ec3b101e441540d5ff5bc5a46f65709ed4863f2850da7bd2c2aa91dc76ac915f109eb39f877c9303c49e8b7ea9d927e4fab04c0667a239b3ec532a5e9990c32ae6c890c8d2145e58e84144013f14248d7de5a927ff028c72f85d6203241a62fe78e105012c5269c7cd4eb13a575af000084a3eccdaa4345f10dd3da20ab4f589899375b9e26a86c251373d08202c889d259a176d515b3ac6173b74ebd7a9591d89f5bf53d0f54136b361fcfc7ed96cb9626430299b5edda4fb1a7039845f07dbd29f395c113d5e62115e0f66377e82a9a351cb1f0ddd963769ca9077917a9ca87880648cf1c505fef15574be54f2b5129d1c37bedd83f49c49b2fa29481a5a33417e5a0c169c5d42ca2e67004e971d69f0792d50f5319f6f5325d12ae89096f1b0e2b3e011c34c1ff64ce670211a77e3cd8715652cb277fcc8bf5b6b623f62b63d77c793d82dbac9ce75bc53559dd3c94b7fa8d3c44f2fe7ab36f388becc7bad509f24cc003c1e8f37d69b0375632083f8a69caf078457b64cbb65282820aceb0ffa69417babf68221ea5c551802c34f45d838695cada86c1d85c9f8bc4989d5005f6b799af64acda1ae42847809a6ebb37c670cb5b9fbd226d5d1a0ae9c730003077baa2b4581433b5658988fa1876b86c4dd6f93ad64c0c256377cc7ab8385d259e16b4badb25072690b84026c4632a5231acbe30990c2688e576514c3093157ea802606ed235c016c0cc8ec61c831f0d818d3051e74e5d664a3d1a0994a8d5d0bb864f282961d31b330e12479f9de9a55aec513e712031cef91d93aa89842b4b2ee5b6133b47140e5e031fefc1f587c9a576af2b197632f7ec3dc162c872ce1b64586fd0139f6b00a68299b72a2959ce552702a141f666cb4a8485dcc7ba0d22bce3a35f0be9341f8712367fe55ab75d667c001fccf395a2ac85b97fcf4884566c57a4f93dfe5e99f49ed326e7db3e46900004d0bb693f1674a8c503c1c27511fb0cbb60a76f243876542850179e3eb14ee7aafaa8b19513be7ed58f88af7359c79112164f43b6dc8704f0d9172985712f8c08d3b0b2e600fb6f66c5aae15a89c4551e8a08cf0d54dd5ee6cea45ee0ba80c8a06e1459c14c86ca7148e6658abb5c9e15f652e1f9f582f6fb7c4583a9f61c96835817214a0a609e4a7ef81ec7ccd4ef21c9cb7076aa4d53cdd7797b8ff8ee536a849744e78127055bf562a1a686eba20bc9b66426a6fd868edc40fdd3a4cd7c5a2da94a0d5dc95ddec1434d04e71b7a5e67aa657037cf8f41578b4a2044ddb1988363b698ea59f561e20faa9d83b80e4ff63b649ab7347ef433d0bcc5bedd4e16fccff0df93802b484851ce400561d89aefd3446ac73382280d0bc55862f86f609870aa7123d2aaf862b73b57695b54f90a0d2393b6fa90f312c9c191da3ec378e66ecaf7209ed77acfabb25fcdf680e0f2991049eb83391d067d2b0c16c34f56ed81c318b74c0ce1aeecb9d62b01fdc90477ef276b7ce45773819ade1499fdef3294c1a0f55fc1266ba482de430fd6f2e3bdb368493687fb1b730e002fb73f511e83d15dc097aaeea951cea4923dad55f47c9230a77a307fadb4ee331c1f3aa37479c3371ec3b301c99df1ebea69a6b7b599168fb808654a033b94018ab1f41ae1e6ae5cdbca9601c529af94e17b307e9f9939be66947c882dcd16eb99fa1a8002209717542fb86c6da2904ac5ec03b0562ad804e66536d06efb48de901d42cd70340e6e3693613a084b4d1590210d21b2f3e89cd242e31666ae8de2f3472f87c4b9d61a75749df015b3c35b64ff1f62cde4e877fb3da3a2a1084e4f9729c7151c80633d96cd1996d66178070917c5bb69d9e8a214fcd7bdbffcdb2fcb3ccbbea68097c92c6ca7143e70831a4eaf62599cd3c84442ca10a216557c299cfd2a4fc8fa89876fdc65d62d3d9e787e68f6ed43f8df780ed4ae6d6782ec6d7e12168eaa4e23117224a83343d98932d4e2dfa361033d66aeb875de9a4a5863ea4df606590f4abc62706d922bc4b6222d4b20908dddeafb3029e3b41d581142c56e3748877bdfefb9ed19843a8210447348b1955c770e4ce91bdeca957db8c1c885f6dbda56c837b529cb9b6ffb51ac19ef918e9c7dd83177f44998532675c3e1c19564acd4f89a5b8b97898105616e3644629c749e534bab97a2bf237e29de3526d5a67c6afa81c05ff0cf45e160b5a0f6c8bb8c303783d8f08849f1accab761fa4f40a56e8a1038e79ae805f4b9177538a674f0b9466c3fb32c19c95ff6d39177e4f410802f801987459c8e819214360d0a52c74cd8f5670c133168fc4b385fe4fe5dcd5be725a006c1dfdf61f28e144893df333d43a14016149c0750b1d7bb493c03595ea3b8977beb2e9dffa8218519429a41e104984dec3f303e1691f6ea398a83d61170592729556a55716a0993f6ea8b1a044de164f60d38d585483b5b23d57ee1f91741ea1357ed215a35c58bab2f52812241107346651aedc7292f074baa4bb558202939cd04c7ddb691405cc40701043f9e4e609bbb7f76e1924a3faa6da3f7cbc97034e17251c36b730a26e0e6c81d0bc27857d36995fff8024096d3341460167494adac8031e3932e8f67df6dfc99d80d780f2108422287f87ba8541731991b7d3567319fc64847ef71cd518e77f0918a4f5052a07d3b54cec35d13869dc6a54f10a9578e32929cc94817fa0cea001f9f67744e8cd082f21deb0c186dbd4e9789573f31c0b4c4ce90b03d83c437b1ae22e4382c3f58a955fced6a3a63893feabfbb18d24dd0af0aecfeea2b7b8bf88a2ca98374bfa766ea43ad6504fad8f21cb8d2adedc739864bc72103c07161a0888587ad5705f995a112d1dc23faf1ba7d1899a22ec6f01e3ed096d434fb0f3fa6db106f0df223eb594c95458e27fe094a43e8ddef466e9dc03b5c8d0401a3e46fbba9b34c71535b80744a366aae8893c8e4c04ee9d2392de8e758e78405fdcb7451451f850b031d36de5f2b16f5a4acf7985df95a9e05e569ec850255abe5cb8697d6df99852ac56a8620e70e7946de8c916a23d4c2151bae7307547b0dab6b939f2d949e5850f78a9fd4e1e3edddc980b2a8ac6ef359bb46faf6d1a79d3796b1524ad5cbcef16d945fefc4a80d3ab05a6dd6c516273fd1b9c8c46ad7c9fd4a0b6433c793628fc6ab5a27146cef38af681018b17b4fa56a37fd46feaf36abc01c5311ef6e806fed58f9e79627015b359e7fbb998e9729b4253dd7613e8ac96cd6e361f0a29fac5f6028456db834e89f5cad98121dc281b9174312da391329e081c133b321c3547ff73cba514ee1ca37c59a13f68c14fe091d8ca54b0a8572138df4de9a20d9a419ce139062a37d9b3e6219748d35685d7386faf1b5d6083f8ae27e427f6aed7383f031785a87a26bd1adf3903c91b8ed981d466889dd23365fd3f3c420f184ebe14108ecb24d0c56f4bb0d1854252491d410f2c8de51e28ca748453b9a4e0efe61b174c929b88ae06ecf43f1917972b6cca3b28212a075b963260577a508f50813f6e5838ac3b2ceb805e774c930145e66bf93418a48719a5ed6af3792d420710487a3385b906a2adb6026f28e19a811a2999c3028fd42fafab7ae8ae67c2ad31808554e2aed443791d975e992f33cfc16f05901bee325899d0d260a6ae4811be2af94060881e8ffbdde7e781f44fd4f3dede561b45418f3fab9666937e748c56f8e27f06583d38bd7ee8aea5a4b30a68a9cd5df78ccc939aa788d4c010f5df9995b8ad798321ecbb224e2a8658b2a2197ce900c4f314b171e3039b294e39b164c505d0b7ac865de9309079e76825fc03fed8150bb08cb8a8e6d3c89a99f9a91da1a05c5df8683c39724d685d773019ecedb94fe8f811ec81992f06a228db9b2950910c589b2618bd3734d1fcbbdb86aa3013fd14a849486b03eaa26d22aff6e771a3b2d7ddc83f09b2e6ccd5cfe2fc97a2f1971543dad5d858bac5d35e213fe9d32545a0142c9677630e411bb149e8aae024620a229cbbc6c1e665f64d929220373d66501e69a4afb83a86a867204ae019a974f901d8ed242832e84b8b82fc703a9f558ec5432c319a2e3dcf608cdcbb4aec6014f884a05b74db187022a9f37a276666548d2db4326f3e10ea09d2d8b979df9ac11b9b7cb07d547702c4d28daa6d5a9fdea46eaab305aa816562ce824fda10dd09e51b6417a8a38be7991885d0e541abd9174a4eb3e600c2a8e83f9296424be176efe7c4a59dd1599001b85d9ab0b740ee2263c88f2fe029773963f95aac0af6b6042e19b214d1da501c9a6d8ac304ca4f666b6a7b71d0f85f273bcd39d1772764d0f09aa672962b94c9a0d71bf5f211f631f817728a6b070b00ac90a9f5076b0e47ddd5086d99952debdd32e8982b84683215e77e16fb2e104d8cbbb0581757f784ed162f4b5a69b84ebc03ec09aa85f70956d6bcbcd4601a686f024a44ffd4563863ea8ef9c34400aafce2e4243197fc697387806a7fd0e1fcb1a63c7e1c90fd02a2945761e5b37eadbe4b97b08644eebb2687e58008c72189c92b260e9e6baffe70d7a9baa75e23ce7e78280cc3e56768254d86b44a386f27734923f2c9180c780da3df00ba79a283f5ea9333ea778983c595af1198d15527a861b130dff94f1ba5430b6960497b89eddb87b8befb44d67afcbdb2c2c3385c3292dc5e0bcd4582ae9a481c78af26c677f73175b945e659c8f0250b99d4b712d7d7cf99c1919eb5e3b1445d4ab36d3dac7c5ed15e4b7abd9e0a2c5d486caa64517656567cd6a9f90ab54e409fd340aabdc195a8ef308ef9934cf6fe1fdab5c996bb2e77e40a626cb8d550783fd616753da6ae8dcb4db44ef5cf3c7099155a6e01d63e8ac54e560296ab948d5077e8ad09c9fb2b805b9ab5d4c61318da26249d5cf0e05b266b99b3a6119cd1029d53ab94807ad33f759e04c8c7acabb712bb330cef0ce6e333ce30ae517fb168a4c86b7e4ad3a9b46dbcfd272a4fec0ecbf76996782460a82d1b47c1f09c38d2aa55f4a670d4b01658cf8c07b53b5d0f54d5dd1aab7ec060888bf2373bf1670e1dc506bc3831d0cd6a4de720596674d114b6dbfc7d8d63f773a406c3328d1e87ff8c9d764772a7c9f67d36f9615d5f9d2ab52c9de938e5c2c321672905c061338eddf60ee46d32858f28205dc1b23e9fb72825e6ae1319599c04ad79ded9399fa0d987e4ff0aa61d38fcb24b36de2d157f43872a205d9208e45d63b162f981660455495bc65d6333c260a8c7ff25a95dcc28c2c1d76a6137c0240487d5ef06c1b26721fff971106925996f59aec193c775307a3336a8c2be18c654f343e7d325f6b0f07642340d13742d7d2998e8b6ea5351b5910464a3dd11423749e9ab7eef1b4a1d58eac13328693e13dc95575c586c0173df37ddc606cd9978198c0c7b4eda7228ccd05fbeec7e2baaf348698bdde5beff6e9980b43cd540151e107a94e9a05019084b194308f34549bcb34948f252c88383a1ef0cfdb97de34b1996b5701c7c3895e8e46c3510d5f97c14b4451dd5d774f978851fb07be0abb77e59d8e648ec3a60308086ef87cedf1c8e4aae105115ef8c2fad162c010d5cb7083ff92b41dddb5e810d0d2ea778429009ad3ac7c676c9f26722b021db8f7e63d448057c2f74f96715799733bb7259137a7e12b8151b6c986008e6b94054a908cc839664e49e6845efd610e54ca1b4e54a52a14a5e4f03ab437815cd26d03509024a02e9981089e23ee7c6fd29d5e137ee5a31500547a0b51e22f49f4357b1bd4af443ea139bd9455c7bb509ce649edad74fef3dd29d9cff5f12647e6c0823c6bf3591d3b23b09d815be95e8241e47e90a7b6c6991301cb52d4b089ab92fc2892d469202f877ec9f1a79ddf81a8266287d992633bc4aaf8a69b534da4353fefd7309f47307f587098104b8af2f7bf1d15df9e76daa90f59898fb7d5c2b29aff2f2377a5a2099fdf6a6362f46721a7e2c7cda7d24800d0e8eed254e11868160cfb5fbe4453e98aa97a5736efa0083e72b73bbfab1446571d7dca15817f5396b4d6af68c8c892dfa9341322e571a1326a55861737774064548071594925fd0e1f934431f0091fc87a7fe1d78c970868c9f678482f1de6a7fcd673d447a6d7a3c77dbd31c13fdca81604250206cff0f1e37bf9963c79c1ae49f5605e8983ca9f6d73e5472f38a19c64e8f67972db91f8b3f94416a6b1c605855c9fc0ad4de8cef348981fded65f1dbc2c27c5e53a53d78f1beea13f8b707ccde3c9f70d8b7e85fd6e214bdb20f10f5d84dec3225f9ed10e49cd4735aaf334c40e87cb887af792c419caab89fb5798c42d486e4fe23e506ce585ea8cd97bef49ad288e7eb3af0ab7fa2de8eae41484102a9caafa867417a605353575254d12d9a2041fcb274fcc11fc30981569c82cb9fb55afc695e0fba45383102eea79102c841f6cf03172c6a27e071d35445edce6079a1d4c659fb700e9e28377a901194b6cda9118027d6915a737923849cdb0169a43086df3e19038807d4bc5074ae88378c18d3af53ea1cbf685e1dce84c031e9cab0ddc1b222cf37db1c32fb2e245978f9627b496678c4562f5b6578a547b99cee97170ad0544edcce3c4919161159f75d72068dd3dc1236ceb83e61febd9038779d90a8dd727714d578d9a7687389c27ccef1f0c5f907ba840229b3a35c78eb1a1d70e0ed5cb7e478da5d1c9256a89debdd23cbf2bb996cc8684378a798f0fafb6f0b1c863342d5ef3ef9ffb7aa2fd5a68bdb1f5cb4b18f7c54a154594d72db342d70d18d4ba3d30580c67c07570b054cee23694c190196d0e4dd1680ec2720e54ec261ffa7048901078daddb3232bf3523fb4916c35c9641c9bc85ffbeda6be76a67d2ebb461a70a63ddf2c8bac02460f47ab5e2e7971bed50c27b4308a2e7c842ce54c7b7dc8f3611ed9638cfe2f6f92047b0f61c3918aab1345b531653bc9db850220b907125d7d0029f8aa3ddb6790b3dcc240eb6d658f718bc26b808891dc8359625f3c9c8ce08e212fb71392538bedaa38e5c7e905818df43be6d3fa95051943223f192d98ed9bdebd7b95b60b7b96e7c8ceaa2f54563432575e8bf5537d5d2a69cec457c52d8f9fa627f96b4e57cedfdec26b06d67b8cc1b2a57c3b1d4d0b5b459f7f67f2d6fae5604f8862c16486d4dd9f48f6fe85090829d0c8c8ed0bd7743e33689341a361e9220d10b01a670755827c024765dc90ea550e897b38fcb32b459068e6ae5d5ffeb8181d679cebce7b24e2d7b87897d2383bf27493a2e8fd97b888e1a8ba0b47e4bec35ad1e89df991e6a96d774522706a3f25041a9d0072c0e86a9bd8f51e787839e272cc941779fc4646ccabccee243d80d76493cf8243599d88f17e9785a80b46595057e9f9a96085251008251f67a8aea6d156a11e9d340b3250c3399b98492be02cb708d6be67f4985dc8174592b5ba70f27c23277287ebf5547026b6d04bf86335adfba016949c9c30ecff2d78217a516f647559537f150f2e1044e15ecf9ce2150fee31ec9e82acd6652a649af4e115ff8f123e15d5ece21c82a6f2ed44c8de7ff48b0983f6884f52107345ab727bb26006d2f9c809e8caf7105167fa2f66fe816d98ee3e19109c051138c4ae6b83e6e4593e2ee77b5610d6ec04249373424e35a497c093146fb30702a7b6b42f0a84c0c8ebca4f1277bc591518c01e6d7d76f5c9aeb24969bf277dd7e416564549270fed85d906a76ebcee5637ff1e7a7f8ee37ba717f7e8a78dbc2a38d74a665c8792fb7881a3f086702ab184d64b90fdbb884b1df243db50762b43bf16a508ae6ffdca2ccf960fc4bca5f38a71235210efcc1917bc0c910a665c36e8e78f45d7000e2c7e75ac87253cad97c5be1c4631e4c9fa44229f831402c08ca6a1f3ddb124707c951b40930fc82ee2fdab92e3283cc3f11a5e4a635cbf3da998a7d44eb67dec08cca4be509f206d10f335dd7d1aa0ce8b80e3fa98a773efac39ea5d4978da154d074a78983e086831cb5730e99beed6462aadced31439b8e759b51cf6a8f8785dfa470302ba4b43669fd9e50fc78f27cb0973dca924e1c458f745faaa50471fb96aa30d0760abe235cb9f04b947233c462ba6a5024227157b9a372cdb5331a9761abcef74636ca400625bed7bf55a3b815ffe0be6c1bad9ed0658be921dd4ffd5380b2d3066baaec1a525f96855e64d494ff1312761cf79f8889022015ddfc46eaa0f5a29f4c0904f992a8bad368d4a667860e594c540ba1b1ea1db2061609b66dbe0957ceeed9c3c246c513944d8ae40e7f1d4f773649f90c45b464a864ed4cf670f6501016f91287c3c25b3d4c9e0d7fae1c0607390c0f21d5a80df549af915d8b41e650db87eedd2b4db7968f4e48101450c77346063c31d8ccb6b13f20f5c33f51236d372f82c960aa511aaf16ee42fc032ddd5b286189aba455b5b98529770b98f8f479423451120736d7ed142af2bf2280fe7e843a2b867a98268fb16397742344d73587d1648056120648d8424eb610ff4bab60813cd39615794745f59792d5ed30f4758098c83f5dcfff36b7fba362068a9a60bb8af60b4430e7cb5634cb21149e943d344197302a05dec0227741aa5d3a7ada9175e22ef0b3daf9cda64647f624acb06030ae15a3b49b3bc3c63e20bbab663478d6b2e49a97a387476cf9419a1c253a5f0157e9eaa1608d69e3f4f298e912bf0727c0f8e78368643dc9b60aecfdd681ef3fafd13aa440c462ceee89b9904e7a3dfd77892b864d2dcdcb54835d85abdc844d90a64e7e21e6064454ca4fe3f58897ef2cd699067109671ec1886c39b5249d9f9027efe580fa4d7b4b33ed6ad28833d5e7e97423a7167bf914ac0afaa6aa98eebb7d9e7d38cea3411eed5671fefde50df629610abd69f07c47d6c28ec631b9c27b2d070c9846b19f8638707079725425e7688705cbd30e81264bd333c5c66c6c3ff479272b98e9e2fd2b6332e8c455c0b25db8dc6570fd78381a169982c7ddf2fd546f200aac7ba433aa464ae6a1f79d1042f5468004ea4b5493e94636d618b3fd009f969b34e1ce421bc7340998c438be884423e445de1739258b2edad75a7fe1cd80803dbc87569bb442ab421b1cd2ea6421b3f1bca23e858e5da640f48b40f4cebcee544b51596a147d1b07d300975344a53a56a7fca4a79c61e83c7c0ff1c951b703a5be7efdbcafd4df9a2486688d4caa279e49c423779b1144947779e193fa20921429b873d2d27fb7528766c50af2dceaa84730636714d8a10fa450d1686f93a01333f11b1cc6bbe8d8e49961d151ca883eafe69eb7a1746253fea9902516f53f7c0fdcf99764205b67699f75ad8654de378e461db5a7cf04c8f79a386d3e38e60e229a1131d8eb203fcc551f791a0c5891639ac8c012072f444ec67d04ef165aef3cefd8be9c5f51700149e37e19ddf63a548d181d67181fefccd140241f8e803f25b5fb0c68f65d92f12d4ac1402cab1958ba147ff9b76c34a8efcad19b55f829cc20a0321cd565f35ceac8f94b3467f70c22b0da24dee9085ba4cb01c55ea5bc82caea13cc9e7be56c541ccc5bcd8213ab6e7897a27d62545639261701f1525c705a68e993b2682e20beff3c01b89977795f396e3fb63e38e18724749479d03dd75be08340b319cffb6135d4a49f1ad8843d676952193ef3f603b66ca2dd9d040f41d2d711128689e8747c8f564a346a161c1787b4db87c21a796de1eb2c33fba6ee724204baccb857b9cbf9645b2777de9dc7f2f9c26ebab5d5ccfb6d18cc1a84f687baa357c46f63732ccd475342ad6cb910270786f70c3c3c3afcbff771c2dc6062605ef8794684e39cffcebef709e7ba45a6415918360518385d3c172a14f93e7d0ab6a7c441e4abe85d092c49249d3c6367f9ef2b652737b37566f8ce3819f9388b46879b5e7289f94a4534a4f42675630bbc0a2ff5c35119e25b4af3b0ede3451f83778273af2544de7fd649478be9ee0fb00e9c31e7feb64eed7854a0047b3c737be344353d9b09f5c0af5929ba0a6996860ce0f0aa0af5099141f98de83fd217431336e63f257585f26a01799bb677a2e8e946a7609a230ff3dbee4b47898d814190f6fc3f17551039c8eebec2af4538e7ba821846361c8017f7a5756dab2695ec36b603672e3104254722bdab6741c9c3a314a263d313e000bfc4689307eabeaf52f41d1c1b0d5a221c5a44c0ce5eaa34cb1b540e7ac8a8e7ea277dc0f660434d4d80e6e83688a462aa29f248785d0c001c7f9d9a2cca6df931b0b1bde4e7bd901d41832b06f67f8db120a6074319d9458a2157e3137acee5b756450bfd8de7bcc0328bd8dd67ca3b49e35723bb52861afed7a84f5d87908703f42143e624898cabd67fc54391e6d2b3e3e77f45f9675fc1a400710d35f772f2bd9df5ba873d5d7ef0397a2f6b321aa0ab1dfb8f3643f3404721c66a09d9f8575a6a2495b798c27a6138acf7de865988dcf18aabca967e905bdc0b64f9c057007677760a23f741d1553b48aca0a92613f6dc603184beab93b1e453af1617d3acce56893dbceddc4d1d15a0ea47b9b3c39edb1d551a7f733ea2955412d17fea2b6547db8a7d94253b7dde883e8cc9244ee7fb2ab19c0ae7500178fa22823d433aaf0df035311fea57283be90f9948a9eadf3d1be704655103aca1f37ab1700a4ef1aaf767e8d185aae082d4a50c4635109df1ad524bacc4cd6ab
Verified      : true
```

> [!CAUTION]
> Before you consider using Psuedo Random Number Generator, which comes with this library implementation, I **strongly** advice you to go through the comments in [include/prng.hpp](./include/prng.hpp).

> [!NOTE]
> Looking at the API documentation, in header files, can give you a pretty good idea of how to use Sphincs+ API. Note, this library doesn't expose any raw pointer based interface, rather everything is wrapped under statically defined `std::span` - which one can easily create from `std::{array, vector}`. I opt for using statically defined `std::span` based function interfaces because we always know, *at compile-time*, how many bytes the seeds/ keys/ signatures are, for various different Sphincs+ instantiations. This gives much better type safety and compile-time error reporting.
