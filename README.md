> [!CAUTION] 
> This implementation is not yet audited. **If you consider using it in production, be careful !**

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

- `sign`: Given M ( > 0 ) -bytes message, SPHINCS+ secret key ( of 4n -bytes ) is used for signing message, by default deterministically. Though one might specifically ask for randomized signing, which will produce random signatures for same message. 
- `verify`: Given M ( > 0 ) -bytes message and SPHINCS+ signature, it uses SPHINCS+ public key ( of 2n -bytes ) for verifying signature, returning boolean result. Truth value is returned if signature is successfully verified.

Here I'm maintaining SPHINCS+ as a header-only, easy-to-use C++ library, which implements SPHINCS+-SHAKE key generation/ signing/ verification algorithms, for all parameter sets ( encompassing NIST security levels {1, 3, 5} ), as suggested in section 7.2 and table 3 of the specification.

> [!TIP]
> *Find the SPHINCS+ specification @ https://sphincs.org/data/sphincs+-r3.1-specification.pdf, which was followed during this work.*

`sha3` is the only dependency of this project, which itself is a zero-dependency, header-only C++ library, implementing SHA3 specification of NIST ( i.e. [FIPS PUB 202](http://dx.doi.org/10.6028/NIST.FIPS.202) ). This is done in order to modularize commonly seen symmetric key dependency in post-quantum cryptographic constructions.

> [!TIP]
> `sha3` is pinned to specific commit, using git submodule. See [usage](#usage) below in order to understand how to use SPHINCS+ in your project.

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
- For benchmarking, you'll need to have `google-benchmark` installed. I found guide @ https://github.com/google/benchmark#installation helpful.

> [!NOTE]
> If you are on a machine running GNU/Linux kernel and you want to obtain CPU cycle count for `keygen`, `sign` and `verify` routines, you should consider building `google-benchmark` library with libPFM support, following https://gist.github.com/itzmeanjan/05dc3e946f635d00c5e0b21aae6203a7, a step-by-step guide. Find more about libPFM @ https://perfmon2.sourceforge.net.

> [!TIP]
> Git submodule based dependencies will generally be imported automatically, but in case that doesn't work, you can manually initialize and update them by issuing `$ git submodule update --init` from inside the root of this repository.

## Testing

For ensuring that SPHINCS+ implementation is functionally correct and compliant with the specification @ https://sphincs.org/data/sphincs+-r3.1-specification.pdf, you may issue

> [!NOTE]
> This implementation of SPHINCS+ specification is **tested** to be compatible and conformant with r3.1 of the specification. That's ensured by generating known answer tests ( KATs ) following https://gist.github.com/itzmeanjan/d483872509b8a1a7c4d6614ec9d43e6c and testing this implementation using those test vectors.

```bash
make -j
```

```bash
Note: Randomizing tests' orders with a seed of 37872 .
[==========] Running 27 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 27 tests from SphincsPlus
[ RUN      ] SphincsPlus.WOTS_PlusNISTSecurityLevel1
[       OK ] SphincsPlus.WOTS_PlusNISTSecurityLevel1 (1 ms)
[ RUN      ] SphincsPlus.WOTS_PlusNISTSecurityLevel5
[       OK ] SphincsPlus.WOTS_PlusNISTSecurityLevel5 (2 ms)
[ RUN      ] SphincsPlus.HyperTreeNISTSecurityLevel5
[       OK ] SphincsPlus.HyperTreeNISTSecurityLevel5 (3870 ms)
[ RUN      ] SphincsPlus.SphincsPlus128sRobustKnownAnswerTests
[       OK ] SphincsPlus.SphincsPlus128sRobustKnownAnswerTests (224245 ms)
[ RUN      ] SphincsPlus.XMSSNISTSecurityLevel5
[       OK ] SphincsPlus.XMSSNISTSecurityLevel5 (808 ms)
[ RUN      ] SphincsPlus.SphincsPlusNISTSecurityLevel1KeygenSignVerify
[       OK ] SphincsPlus.SphincsPlusNISTSecurityLevel1KeygenSignVerify (7228 ms)
[ RUN      ] SphincsPlus.SphincsPlusNISTSecurityLevel3KeygenSignVerify
[       OK ] SphincsPlus.SphincsPlusNISTSecurityLevel3KeygenSignVerify (12139 ms)
[ RUN      ] SphincsPlus.XMSSNISTSecurityLevel1
[       OK ] SphincsPlus.XMSSNISTSecurityLevel1 (811 ms)
[ RUN      ] SphincsPlus.SphincsPlus256fRobustKnownAnswerTests
[       OK ] SphincsPlus.SphincsPlus256fRobustKnownAnswerTests (33483 ms)
[ RUN      ] SphincsPlus.XMSSNISTSecurityLevel3
[       OK ] SphincsPlus.XMSSNISTSecurityLevel3 (1192 ms)
[ RUN      ] SphincsPlus.SphincsPlus128fSimpleKnownAnswerTests
[       OK ] SphincsPlus.SphincsPlus128fSimpleKnownAnswerTests (5626 ms)
[ RUN      ] SphincsPlus.SphincsPlus192fRobustKnownAnswerTests
[       OK ] SphincsPlus.SphincsPlus192fRobustKnownAnswerTests (17153 ms)
[ RUN      ] SphincsPlus.WOTS_PlusNISTSecurityLevel3
[       OK ] SphincsPlus.WOTS_PlusNISTSecurityLevel3 (2 ms)
[ RUN      ] SphincsPlus.SphincsPlusNISTSecurityLevel5KeygenSignVerify
[       OK ] SphincsPlus.SphincsPlusNISTSecurityLevel5KeygenSignVerify (10644 ms)
[ RUN      ] SphincsPlus.SphincsPlus128fRobustKnownAnswerTests
[       OK ] SphincsPlus.SphincsPlus128fRobustKnownAnswerTests (10550 ms)
[ RUN      ] SphincsPlus.HyperTreeNISTSecurityLevel3
[       OK ] SphincsPlus.HyperTreeNISTSecurityLevel3 (4928 ms)
[ RUN      ] SphincsPlus.SphincsPlus192sSimpleKnownAnswerTests
[       OK ] SphincsPlus.SphincsPlus192sSimpleKnownAnswerTests (205202 ms)
[ RUN      ] SphincsPlus.FORSNISTSecurityLevel1
[       OK ] SphincsPlus.FORSNISTSecurityLevel1 (455 ms)
[ RUN      ] SphincsPlus.FORSNISTSecurityLevel3
[       OK ] SphincsPlus.FORSNISTSecurityLevel3 (2194 ms)
[ RUN      ] SphincsPlus.SphincsPlus192fSimpleKnownAnswerTests
[       OK ] SphincsPlus.SphincsPlus192fSimpleKnownAnswerTests (9045 ms)
[ RUN      ] SphincsPlus.HyperTreeNISTSecurityLevel1
[       OK ] SphincsPlus.HyperTreeNISTSecurityLevel1 (3377 ms)
[ RUN      ] SphincsPlus.FORSNISTSecurityLevel5
[       OK ] SphincsPlus.FORSNISTSecurityLevel5 (2883 ms)
[ RUN      ] SphincsPlus.SphincsPlus256sRobustKnownAnswerTests
[       OK ] SphincsPlus.SphincsPlus256sRobustKnownAnswerTests (319577 ms)
[ RUN      ] SphincsPlus.SphincsPlus256fSimpleKnownAnswerTests
[       OK ] SphincsPlus.SphincsPlus256fSimpleKnownAnswerTests (18823 ms)
[ RUN      ] SphincsPlus.SphincsPlus192sRobustKnownAnswerTests
[       OK ] SphincsPlus.SphincsPlus192sRobustKnownAnswerTests (393502 ms)
[ RUN      ] SphincsPlus.SphincsPlus128sSimpleKnownAnswerTests
[       OK ] SphincsPlus.SphincsPlus128sSimpleKnownAnswerTests (120112 ms)
[ RUN      ] SphincsPlus.SphincsPlus256sSimpleKnownAnswerTests
[       OK ] SphincsPlus.SphincsPlus256sSimpleKnownAnswerTests (175160 ms)
[----------] 27 tests from SphincsPlus (1583025 ms total)

[----------] Global test environment tear-down
[==========] 27 tests from 1 test suite ran. (1583025 ms total)
[  PASSED  ] 27 tests.
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
> Ensure you've put all your CPU cores on performance mode before running benchmarks, follow guide @ https://github.com/google/benchmark/blob/main/docs/reducing_variance.md.

> [!NOTE]
> `make perf` - was issued when collecting following benchmarks. Notice, cycles column, denoting cost of executing Dilithium signature scheme routines in terms of CPU cycles. Follow https://github.com/google/benchmark/blob/main/docs/perf_counters.md for more details.

### On 12th Gen Intel(R) Core(TM) i7-1260P [ Compiled with GCC-13.2.0 ]

```bash
2023-12-13T23:09:23+04:00
Running ./build/perf.out
Run on (16 X 1201.29 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 1280 KiB (x8)
  L3 Unified 18432 KiB (x1)
Load Average: 2.05, 0.77, 0.72
----------------------------------------------------------------------------------------------------------------------------
Benchmark                                   Time             CPU   Iterations     CYCLES average_cpu_cycles items_per_second
----------------------------------------------------------------------------------------------------------------------------
sphincs+-128s-robust/keygen_mean          201 ms          201 ms           10    913.02M           501.801M        4.97471/s
sphincs+-128s-robust/keygen_median        201 ms          201 ms           10   912.982M           500.702M        4.98506/s
sphincs+-128s-robust/keygen_stddev       1.83 ms         1.83 ms           10   376.126k           4.57755M      0.0450845/s
sphincs+-128s-robust/keygen_cv           0.91 %          0.91 %            10      0.04%              0.91%            0.91%
sphincs+-128s-robust/sign_mean           1522 ms         1522 ms           10   6.76918G           3.79856G        0.65736/s
sphincs+-128s-robust/sign_median         1511 ms         1510 ms           10   6.76879G           3.77055G       0.662096/s
sphincs+-128s-robust/sign_stddev         30.4 ms         30.4 ms           10   2.24195M           75.7684M      0.0127886/s
sphincs+-128s-robust/sign_cv             1.99 %          2.00 %            10      0.03%              1.99%            1.95%
sphincs+-192s-simple/verify_mean         1.51 ms         1.51 ms           10    6.7627M           3.76358M        664.567/s
sphincs+-192s-simple/verify_median       1.48 ms         1.48 ms           10    6.8958M           3.70029M        674.526/s
sphincs+-192s-simple/verify_stddev      0.057 ms        0.055 ms           10   401.849k           141.616k        24.0437/s
sphincs+-192s-simple/verify_cv           3.76 %          3.68 %            10      5.94%              3.76%            3.62%
sphincs+-128s-simple/verify_mean        0.636 ms        0.636 ms           10   2.71568M           1.58765M       1.57322k/s
sphincs+-128s-simple/verify_median      0.638 ms        0.638 ms           10   2.73004M           1.59338M        1.5665k/s
sphincs+-128s-simple/verify_stddev      0.018 ms        0.018 ms           10    62.343k           44.4747k        44.2209/s
sphincs+-128s-simple/verify_cv           2.80 %          2.80 %            10      2.30%              2.80%            2.81%
sphincs+-256s-robust/verify_mean         2.48 ms         2.48 ms           10   10.4594M            6.1892M        403.416/s
sphincs+-256s-robust/verify_median       2.49 ms         2.49 ms           10   10.4441M           6.21787M        401.426/s
sphincs+-256s-robust/verify_stddev      0.048 ms        0.048 ms           10   206.941k           118.554k        7.88149/s
sphincs+-256s-robust/verify_cv           1.92 %          1.92 %            10      1.98%              1.92%            1.95%
sphincs+-192s-simple/keygen_mean          201 ms          201 ms           10   922.907M           501.552M        4.97952/s
sphincs+-192s-simple/keygen_median        199 ms          199 ms           10   923.117M           496.854M        5.02515/s
sphincs+-192s-simple/keygen_stddev       4.76 ms         4.76 ms           10   4.20723M           11.8833M       0.115355/s
sphincs+-192s-simple/keygen_cv           2.37 %          2.37 %            10      0.46%              2.37%            2.32%
sphincs+-256f-robust/sign_mean            233 ms          233 ms           10   1.02045G           581.342M        4.29636/s
sphincs+-256f-robust/sign_median          230 ms          230 ms           10   1.02039G           575.063M        4.34046/s
sphincs+-256f-robust/sign_stddev         6.21 ms         6.21 ms           10   1.08544M           15.5037M       0.113206/s
sphincs+-256f-robust/sign_cv             2.67 %          2.67 %            10      0.11%              2.67%            2.63%
sphincs+-256s-robust/sign_mean           2094 ms         2094 ms           10   9.22295G           5.22724G       0.477633/s
sphincs+-256s-robust/sign_median         2082 ms         2082 ms           10   9.22408G           5.19652G       0.480336/s
sphincs+-256s-robust/sign_stddev         35.3 ms         35.3 ms           10    6.5308M           88.0669M       7.85146m/s
sphincs+-256s-robust/sign_cv             1.68 %          1.69 %            10      0.07%              1.68%            1.64%
sphincs+-128s-simple/keygen_mean         83.8 ms         83.8 ms           10    345.96M           209.093M        11.9504/s
sphincs+-128s-simple/keygen_median       82.5 ms         82.5 ms           10   346.488M           205.815M        12.1277/s
sphincs+-128s-simple/keygen_stddev       2.91 ms         2.91 ms           10   1.81684M           7.25927M       0.401903/s
sphincs+-128s-simple/keygen_cv           3.47 %          3.47 %            10      0.53%              3.47%            3.36%
sphincs+-128f-robust/sign_mean           74.2 ms         74.2 ms           10   330.664M           185.091M        13.4921/s
sphincs+-128f-robust/sign_median         73.7 ms         73.7 ms           10   330.832M           184.049M        13.5619/s
sphincs+-128f-robust/sign_stddev         1.71 ms         1.71 ms           10   841.062k           4.27533M       0.304928/s
sphincs+-128f-robust/sign_cv             2.31 %          2.31 %            10      0.25%              2.31%            2.26%
sphincs+-256f-robust/verify_mean         4.91 ms         4.91 ms           10    20.515M           12.2672M        203.646/s
sphincs+-256f-robust/verify_median       4.89 ms         4.89 ms           10   20.5203M           12.1998M        204.598/s
sphincs+-256f-robust/verify_stddev      0.151 ms        0.151 ms           10   259.967k           377.261k        6.18987/s
sphincs+-256f-robust/verify_cv           3.07 %          3.08 %            10      1.27%              3.08%            3.04%
sphincs+-192f-robust/verify_mean         5.03 ms         5.03 ms           10   21.2779M           12.5486M        198.948/s
sphincs+-192f-robust/verify_median       5.01 ms         5.01 ms           10   21.3069M           12.4947M        199.773/s
sphincs+-192f-robust/verify_stddev      0.075 ms        0.075 ms           10   162.176k           188.321k        2.95493/s
sphincs+-192f-robust/verify_cv           1.50 %          1.50 %            10      0.76%              1.50%            1.49%
sphincs+-256s-simple/verify_mean         1.37 ms         1.37 ms           10   5.82032M           3.41903M        730.339/s
sphincs+-256s-simple/verify_median       1.36 ms         1.36 ms           10   5.82866M           3.39363M        735.489/s
sphincs+-256s-simple/verify_stddev      0.030 ms        0.030 ms           10   71.0555k           75.4547k        15.7707/s
sphincs+-256s-simple/verify_cv           2.21 %          2.21 %            10      1.22%              2.21%            2.16%
sphincs+-192s-robust/sign_mean           2507 ms         2507 ms           10   11.2156G           6.25818G       0.398988/s
sphincs+-192s-robust/sign_median         2514 ms         2514 ms           10   11.2208G           6.27437G       0.397829/s
sphincs+-192s-robust/sign_stddev         48.3 ms         48.3 ms           10   19.7287M            120.58M       7.74241m/s
sphincs+-192s-robust/sign_cv             1.93 %          1.93 %            10      0.18%              1.93%            1.94%
sphincs+-192f-robust/keygen_mean         4.69 ms         4.69 ms           10   20.9022M           11.6987M        213.454/s
sphincs+-192f-robust/keygen_median       4.65 ms         4.65 ms           10   20.9035M           11.6125M        214.942/s
sphincs+-192f-robust/keygen_stddev      0.104 ms        0.104 ms           10   12.7291k           258.731k        4.62846/s
sphincs+-192f-robust/keygen_cv           2.21 %          2.21 %            10      0.06%              2.21%            2.17%
sphincs+-256s-simple/sign_mean            967 ms          967 ms           10   4.11501G           2.41473G        1.03386/s
sphincs+-256s-simple/sign_median          964 ms          964 ms           10   4.12398G           2.40513G         1.0378/s
sphincs+-256s-simple/sign_stddev         13.2 ms         13.2 ms           10   28.5561M            33.004M      0.0140213/s
sphincs+-256s-simple/sign_cv             1.37 %          1.37 %            10      0.69%              1.37%            1.36%
sphincs+-128f-simple/sign_mean           32.2 ms         32.2 ms           10   135.368M           80.2947M        31.1002/s
sphincs+-128f-simple/sign_median         32.0 ms         32.0 ms           10   135.317M           79.7514M        31.2968/s
sphincs+-128f-simple/sign_stddev        0.747 ms        0.747 ms           10   237.835k           1.86529M       0.707854/s
sphincs+-128f-simple/sign_cv             2.32 %          2.32 %            10      0.18%              2.32%            2.28%
sphincs+-128f-simple/keygen_mean         1.32 ms         1.32 ms           10   5.51898M           3.28985M        759.087/s
sphincs+-128f-simple/keygen_median       1.32 ms         1.32 ms           10    5.5206M           3.29328M        757.941/s
sphincs+-128f-simple/keygen_stddev      0.031 ms        0.031 ms           10   3.46708k           77.0416k        18.2285/s
sphincs+-128f-simple/keygen_cv           2.34 %          2.34 %            10      0.06%              2.34%            2.40%
sphincs+-192f-simple/keygen_mean         2.05 ms         2.05 ms           10   8.68651M           5.11701M        487.889/s
sphincs+-192f-simple/keygen_median       2.04 ms         2.04 ms           10   8.68642M           5.09802M        489.633/s
sphincs+-192f-simple/keygen_stddev      0.030 ms        0.030 ms           10   7.29449k           75.9995k        7.10665/s
sphincs+-192f-simple/keygen_cv           1.49 %          1.49 %            10      0.08%              1.49%            1.46%
sphincs+-128f-simple/verify_mean         1.75 ms         1.75 ms           10   7.45286M           4.37115M        571.721/s
sphincs+-128f-simple/verify_median       1.77 ms         1.77 ms           10   7.47646M           4.41831M        564.933/s
sphincs+-128f-simple/verify_stddev      0.063 ms        0.063 ms           10   172.274k           157.011k        21.2056/s
sphincs+-128f-simple/verify_cv           3.59 %          3.59 %            10      2.31%              3.59%            3.71%
sphincs+-192f-simple/sign_mean           55.0 ms         54.9 ms           10   235.738M           137.212M        18.2088/s
sphincs+-192f-simple/sign_median         54.8 ms         54.8 ms           10   235.612M           136.754M        18.2521/s
sphincs+-192f-simple/sign_stddev         1.29 ms         1.20 ms           10   450.184k           3.21717M       0.382246/s
sphincs+-192f-simple/sign_cv             2.34 %          2.18 %            10      0.19%              2.34%            2.10%
sphincs+-128f-robust/verify_mean         4.81 ms         4.81 ms           10   21.2898M           12.0043M        208.048/s
sphincs+-128f-robust/verify_median       4.87 ms         4.87 ms           10   21.3334M           12.1616M        205.238/s
sphincs+-128f-robust/verify_stddev      0.117 ms        0.117 ms           10   287.522k           290.867k        5.14776/s
sphincs+-128f-robust/verify_cv           2.42 %          2.43 %            10      1.35%              2.42%            2.47%
sphincs+-256f-simple/sign_mean            102 ms          102 ms           10   427.894M            255.49M        9.77595/s
sphincs+-256f-simple/sign_median          101 ms          101 ms           10   428.045M            252.05M        9.90292/s
sphincs+-256f-simple/sign_stddev         2.78 ms         2.78 ms           10    660.29k           6.93806M       0.258873/s
sphincs+-256f-simple/sign_cv             2.72 %          2.72 %            10      0.15%              2.72%            2.65%
sphincs+-256f-robust/keygen_mean         12.1 ms         12.1 ms           10   53.9607M             30.25M        82.5254/s
sphincs+-256f-robust/keygen_median       12.1 ms         12.1 ms           10   53.9587M           30.3139M        82.3423/s
sphincs+-256f-robust/keygen_stddev      0.142 ms        0.142 ms           10   16.3402k           353.976k       0.967165/s
sphincs+-256f-robust/keygen_cv           1.17 %          1.17 %            10      0.03%              1.17%            1.17%
sphincs+-128s-simple/sign_mean            632 ms          632 ms           10   2.65653G           1.57693G        1.58305/s
sphincs+-128s-simple/sign_median          630 ms          630 ms           10   2.65712G           1.57343G        1.58642/s
sphincs+-128s-simple/sign_stddev         7.26 ms         7.27 ms           10   3.32381M           18.1329M      0.0178092/s
sphincs+-128s-simple/sign_cv             1.15 %          1.15 %            10      0.13%              1.15%            1.12%
sphincs+-256s-robust/keygen_mean          195 ms          195 ms           10   864.414M           487.297M        5.13009/s
sphincs+-256s-robust/keygen_median        194 ms          194 ms           10   863.613M           484.862M         5.1481/s
sphincs+-256s-robust/keygen_stddev       5.53 ms         5.06 ms           10   2.71374M           13.8123M       0.129272/s
sphincs+-256s-robust/keygen_cv           2.83 %          2.59 %            10      0.31%              2.83%            2.52%
sphincs+-128s-robust/verify_mean         1.56 ms         1.56 ms           10   7.02791M           3.89326M        642.276/s
sphincs+-128s-robust/verify_median       1.55 ms         1.55 ms           10   6.92317M           3.86731M        645.449/s
sphincs+-128s-robust/verify_stddev      0.071 ms        0.071 ms           10   294.611k           177.099k        28.7489/s
sphincs+-128s-robust/verify_cv           4.55 %          4.55 %            10      4.19%              4.55%            4.48%
sphincs+-192f-simple/verify_mean         2.77 ms         2.77 ms           10   11.7693M           6.91173M         361.36/s
sphincs+-192f-simple/verify_median       2.77 ms         2.77 ms           10   11.7724M           6.90578M        361.446/s
sphincs+-192f-simple/verify_stddev      0.073 ms        0.073 ms           10   177.475k           181.871k        9.58909/s
sphincs+-192f-simple/verify_cv           2.63 %          2.63 %            10      1.51%              2.63%            2.65%
sphincs+-256f-simple/keygen_mean         4.87 ms         4.87 ms           10   20.2736M           12.1663M        205.343/s
sphincs+-256f-simple/keygen_median       4.82 ms         4.82 ms           10    20.278M           12.0239M        207.595/s
sphincs+-256f-simple/keygen_stddev      0.150 ms        0.150 ms           10   22.2275k           374.754k        6.24175/s
sphincs+-256f-simple/keygen_cv           3.08 %          3.08 %            10      0.11%              3.08%            3.04%
sphincs+-192s-robust/verify_mean         2.33 ms         2.33 ms           10   10.3507M           5.80658M        430.412/s
sphincs+-192s-robust/verify_median       2.29 ms         2.29 ms           10   10.3478M           5.72023M        436.328/s
sphincs+-192s-robust/verify_stddev      0.090 ms        0.090 ms           10    232.78k           225.106k         16.143/s
sphincs+-192s-robust/verify_cv           3.88 %          3.88 %            10      2.25%              3.88%            3.75%
sphincs+-256s-simple/keygen_mean         77.8 ms         77.8 ms           10   327.463M           194.173M        12.8629/s
sphincs+-256s-simple/keygen_median       78.2 ms         78.2 ms           10   327.321M           195.248M        12.7839/s
sphincs+-256s-simple/keygen_stddev       2.05 ms         2.05 ms           10   399.286k           5.12261M       0.346322/s
sphincs+-256s-simple/keygen_cv           2.64 %          2.64 %            10      0.12%              2.64%            2.69%
sphincs+-192f-robust/sign_mean            115 ms          115 ms           10   506.492M           287.564M        8.68512/s
sphincs+-192f-robust/sign_median          115 ms          115 ms           10    506.67M           286.736M        8.70699/s
sphincs+-192f-robust/sign_stddev         2.84 ms         2.83 ms           10   1.19232M           7.07564M       0.208781/s
sphincs+-192f-robust/sign_cv             2.46 %          2.46 %            10      0.24%              2.46%            2.40%
sphincs+-256f-simple/verify_mean         2.75 ms         2.75 ms           10   11.5814M           6.85574M        364.184/s
sphincs+-256f-simple/verify_median       2.73 ms         2.73 ms           10   11.5823M           6.82514M        365.721/s
sphincs+-256f-simple/verify_stddev      0.049 ms        0.049 ms           10   154.573k           122.569k        6.49341/s
sphincs+-256f-simple/verify_cv           1.79 %          1.79 %            10      1.33%              1.79%            1.78%
sphincs+-192s-robust/keygen_mean          298 ms          298 ms           10   1.33629G           743.857M        3.35583/s
sphincs+-192s-robust/keygen_median        299 ms          299 ms           10   1.33612G           745.675M         3.3474/s
sphincs+-192s-robust/keygen_stddev       2.67 ms         2.67 ms           10   669.298k           6.66202M      0.0301769/s
sphincs+-192s-robust/keygen_cv           0.90 %          0.90 %            10      0.05%              0.90%            0.90%
sphincs+-128f-robust/keygen_mean         3.24 ms         3.24 ms           10   14.4573M           8.09145M        308.579/s
sphincs+-128f-robust/keygen_median       3.23 ms         3.23 ms           10   14.4576M           8.06411M        309.529/s
sphincs+-128f-robust/keygen_stddev      0.056 ms        0.056 ms           10   13.1193k           139.127k        5.14654/s
sphincs+-128f-robust/keygen_cv           1.72 %          1.72 %            10      0.09%              1.72%            1.67%
sphincs+-192s-simple/sign_mean           1670 ms         1670 ms           10   7.70172G           4.16766G       0.599017/s
sphincs+-192s-simple/sign_median         1665 ms         1665 ms           10   7.70573G           4.15646G       0.600521/s
sphincs+-192s-simple/sign_stddev         19.5 ms         19.5 ms           10   23.2734M           48.6806M       6.94608m/s
sphincs+-192s-simple/sign_cv             1.17 %          1.17 %            10      0.30%              1.17%            1.16%
```

## Usage

`sphincs` is a zero-dependency, header-only C++ library which is pretty easy to use. To get started

- Clone SPHINCS+ repository
- Fetch `sha3` dependency, by enabling git submodule

```bash
pushd sphincs
git submodule update --init
popd
```

- Write program which makes use of SPHINCS+ {keygen, signing, verification} API ( all of these routines live under `sphincs::` namespace ), while importing [include/sphincs.hpp](./include/sphincs.hpp) and correctly parameterizing invoked functions so that you use desired SPHINCS+ instance, among recommended variants.

> [!NOTE]
> C++ compile-time checks are in-place to ensure that you only use SPHINCS+ parameter sets which are suggested in the specification ( it's linked below ). See [utils.hpp](./include/utils.hpp) to understand how it's done using `constexpr` requirements.

- Finally compile your program, while letting your compiler know where it can find SPHINCS+ and Sha3 headers.

> [!NOTE]
> For recommended parameter sets, find table 3 of SPHINCS+ [specification](https://sphincs.org/data/sphincs+-r3.1-specification.pdf).

### SPHINCS+-SHAKE API Usage Flow

- Generate new keypair
- Sign non-empty message using secret key
- Verify signature using message and public key

Step | Header | Namespace | Routine | Input(s) | Output(s)
--- | --- | --- | :-: | :-- | --:
Key Generation | [sphincs.hpp](./include/sphincs.hpp) | `sphincs::` | `keygen<n, h, d, w, v>(skey, pkey)` | - | 4n -bytes secret key, 2n -bytes public key
Signing | [sphincs.hpp](./include/sphincs.hpp) | `sphincs::` | `sign<n, h, d, a, k, w, v, randomize = false>(msg, mlen, skey, sig)` | mlen -bytes message and 4n -bytes secret key | (1 + k * (a + 1) + h + d * len) * n -bytes signature
Verification | [sphincs.hpp](./include/sphincs.hpp) | `sphincs::` | `verify<n, h, d, a, k, w, v>(msg, mlen, sig, pkey)` | mlen -bytes message, (1 + k * (a + 1) + h + d * len) * n -bytes signature and 2n -bytes public key | Boolean value, denoting successful verification by returning truth value 

Here's an example program which demonstrates how to

- Compute ( at compile-time ) how much memory to allocate for public key, secret key and signature, based on chosen parameter set

> [!NOTE]
> Utility routines live under namespace `sphincs_utils::`, in header [utils.hpp](./include/utils.hpp). You won't probably need to import it seperately, it becomes available with inclusion of [sphincs.hpp](./include/sphincs.hpp).

- Generate new keypair, using system randomness
- Sign random 32 -bytes message, using secret key
- Verify signature using message and public key

for SPHINCS+-SHAKE-128s-robust parameter set. It can be adapted for other parameter sets fairly easily. 

```bash
pushd example; g++ -std=c++20 -Wall -O3 -I ../include -I ../sha3/include sphincs+-shake.cpp && ./a.out; popd

SPHINCS+-SHAKE-128s-robust @ NIST Security Level 1
Secret Key   : 49a5ca3d5f151f2c9c0f197d324724c9c2313bdb69720a1bc615434b4874ba41949e000ab275dd6da569ff32757c021586eb2036e1ba14483e809ffa0c5505f5
Public Key   : 949e000ab275dd6da569ff32757c021586eb2036e1ba14483e809ffa0c5505f5
Message      : 80bce832fab336eefdceef28ab89838667d164d255298fe888344760199a8c3c
Signature    : 3afabef1627a6130cc3b020b37e46185f4e711073c79340c8cc67b780ae019ac90ae7d3d4b49fb9ebe3c02c5de2749a74f7d9b5f1fc338dbdb6af4c9dd80f4c2878d2a67b03a5cd00405532accfc030f3f7aa2338156b8cb1af6b6836247f5f7d337c06c9f686077d576388257ef9df0ad1c77675cee73840c44f06dce3c74bead5e55b74f4a143337c437c675cff6394b9bd116649e2155b908d4f32d875fe525ee23a666761798a5364d91b55f4c7cf7d8a78cb99a04e4469edcb55efa9a27e70cae8742ab9199b4558d6ab8033bc648aee3744d93c7e502b07ef5f00be9b9e8582ca6b86a0c0d764b8236ab776c33d9debfa8c0520bdd9ae290805778503a6ec3b101e441540d5ff5bc5a46f65709ed4863f2850da7bd2c2aa91dc76ac915f109eb39f877c9303c49e8b7ea9d927e4fab04c0667a239b3ec532a5e9990c32ae6c890c8d2145e58e84144013f14248d7de5a927ff028c72f85d6203241a62fe78e105012c5269c7cd4eb13a575af000084a3eccdaa4345f10dd3da20ab4f589899375b9e26a86c251373d08202c889d259a176d515b3ac6173b74ebd7a9591d89f5bf53d0f54136b361fcfc7ed96cb9626430299b5edda4fb1a7039845f07dbd29f395c113d5e62115e0f66377e82a9a351cb1f0ddd963769ca9077917a9ca87880648cf1c505fef15574be54f2b5129d1c37bedd83f49c49b2fa29481a5a33417e5a0c169c5d42ca2e67004e971d69f0792d50f5319f6f5325d12ae89096f1b0e2b3e011c34c1ff64ce670211a77e3cd8715652cb277fcc8bf5b6b623f62b63d77c793d82dbac9ce75bc53559dd3c94b7fa8d3c44f2fe7ab36f388becc7bad509f24cc003c1e8f37d69b0375632083f8a69caf078457b64cbb65282820aceb0ffa69417babf68221ea5c551802c34f45d838695cada86c1d85c9f8bc4989d5005f6b799af64acda1ae42847809a6ebb37c670cb5b9fbd226d5d1a0ae9c730003077baa2b4581433b5658988fa1876b86c4dd6f93ad64c0c256377cc7ab8385d259e16b4badb25072690b84026c4632a5231acbe30990c2688e576514c3093157ea802606ed235c016c0cc8ec61c831f0d818d3051e74e5d664a3d1a0994a8d5d0bb864f282961d31b330e12479f9de9a55aec513e712031cef91d93aa89842b4b2ee5b6133b47140e5e031fefc1f587c9a576af2b197632f7ec3dc162c872ce1b64586fd0139f6b00a68299b72a2959ce552702a141f666cb4a8485dcc7ba0d22bce3a35f0be9341f8712367fe55ab75d667c001fccf395a2ac85b97fcf4884566c57a4f93dfe5e99f49ed326e7db3e46900004d0bb693f1674a8c503c1c27511fb0cbb60a76f243876542850179e3eb14ee7aafaa8b19513be7ed58f88af7359c79112164f43b6dc8704f0d9172985712f8c08d3b0b2e600fb6f66c5aae15a89c4551e8a08cf0d54dd5ee6cea45ee0ba80c8a06e1459c14c86ca7148e6658abb5c9e15f652e1f9f582f6fb7c4583a9f61c96835817214a0a609e4a7ef81ec7ccd4ef21c9cb7076aa4d53cdd7797b8ff8ee536a849744e78127055bf562a1a686eba20bc9b66426a6fd868edc40fdd3a4cd7c5a2da94a0d5dc95ddec1434d04e71b7a5e67aa657037cf8f41578b4a2044ddb1988363b698ea59f561e20faa9d83b80e4ff63b649ab7347ef433d0bcc5bedd4e16fccff0df93802b484851ce400561d89aefd3446ac73382280d0bc55862f86f609870aa7123d2aaf862b73b57695b54f90a0d2393b6fa90f312c9c191da3ec378e66ecaf7209ed77acfabb25fcdf680e0f2991049eb83391d067d2b0c16c34f56ed81c318b74c0ce1aeecb9d62b01fdc90477ef276b7ce45773819ade1499fdef3294c1a0f55fc1266ba482de430fd6f2e3bdb368493687fb1b730e002fb73f511e83d15dc097aaeea951cea4923dad55f47c9230a77a307fadb4ee331c1f3aa37479c3371ec3b301c99df1ebea69a6b7b599168fb808654a033b94018ab1f41ae1e6ae5cdbca9601c529af94e17b307e9f9939be66947c882dcd16eb99fa1a8002209717542fb86c6da2904ac5ec03b0562ad804e66536d06efb48de901d42cd70340e6e3693613a084b4d1590210d21b2f3e89cd242e31666ae8de2f3472f87c4b9d61a75749df015b3c35b64ff1f62cde4e877fb3da3a2a1084e4f9729c7151c80633d96cd1996d66178070917c5bb69d9e8a214fcd7bdbffcdb2fcb3ccbbea68097c92c6ca7143e70831a4eaf62599cd3c84442ca10a216557c299cfd2a4fc8fa89876fdc65d62d3d9e787e68f6ed43f8df780ed4ae6d6782ec6d7e12168eaa4e23117224a83343d98932d4e2dfa361033d66aeb875de9a4a5863ea4df606590f4abc62706d922bc4b6222d4b20908dddeafb3029e3b41d581142c56e3748877bdfefb9ed19843a8210447348b1955c770e4ce91bdeca957db8c1c885f6dbda56c837b529cb9b6ffb51ac19ef918e9c7dd83177f44998532675c3e1c19564acd4f89a5b8b97898105616e3644629c749e534bab97a2bf237e29de3526d5a67c6afa81c05ff0cf45e160b5a0f6c8bb8c303783d8f08849f1accab761fa4f40a56e8a1038e79ae805f4b9177538a674f0b9466c3fb32c19c95ff6d39177e4f410802f801987459c8e819214360d0a52c74cd8f5670c133168fc4b385fe4fe5dcd5be725a006c1dfdf61f28e144893df333d43a14016149c0750b1d7bb493c03595ea3b8977beb2e9dffa8218519429a41e104984dec3f303e1691f6ea398a83d61170592729556a55716a0993f6ea8b1a044de164f60d38d585483b5b23d57ee1f91741ea1357ed215a35c58bab2f52812241107346651aedc7292f074baa4bb558202939cd04c7ddb691405cc40701043f9e4e609bbb7f76e1924a3faa6da3f7cbc97034e17251c36b730a26e0e6c81d0bc27857d36995fff8024096d3341460167494adac8031e3932e8f67df6dfc99d80d780f2108422287f87ba8541731991b7d3567319fc64847ef71cd518e77f0918a4f5052a07d3b54cec35d13869dc6a54f10a9578e32929cc94817fa0cea001f9f67744e8cd082f21deb0c186dbd4e9789573f31c0b4c4ce90b03d83c437b1ae22e4382c3f58a955fced6a3a63893feabfbb18d24dd0af0aecfeea2b7b8bf88a2ca98374bfa766ea43ad6504fad8f21cb8d2adedc739864bc72103c07161a0888587ad5705f995a112d1dc23faf1ba7d1899a22ec6f01e3ed096d434fb0f3fa6db106f0df223eb594c95458e27fe094a43e8ddef466e9dc03b5c8d0401a3e46fbba9b34c71535b80744a366aae8893c8e4c04ee9d2392de8e758e78405fdcb7451451f850b031d36de5f2b16f5a4acf7985df95a9e05e569ec850255abe5cb8697d6df99852ac56a8620e70e7946de8c916a23d4c2151bae7307547b0dab6b939f2d949e5850f78a9fd4e1e3edddc980b2a8ac6ef359bb46faf6d1a79d3796b1524ad5cbcef16d945fefc4a80d3ab05a6dd6c516273fd1b9c8c46ad7c9fd4a0b6433c793628fc6ab5a27146cef38af681018b17b4fa56a37fd46feaf36abc01c5311ef6e806fed58f9e79627015b359e7fbb998e9729b4253dd7613e8ac96cd6e361f0a29fac5f6028456db834e89f5cad98121dc281b9174312da391329e081c133b321c3547ff73cba514ee1ca37c59a13f68c14fe091d8ca54b0a8572138df4de9a20d9a419ce139062a37d9b3e6219748d35685d7386faf1b5d6083f8ae27e427f6aed7383f031785a87a26bd1adf3903c91b8ed981d466889dd23365fd3f3c420f184ebe14108ecb24d0c56f4bb0d1854252491d410f2c8de51e28ca748453b9a4e0efe61b174c929b88ae06ecf43f1917972b6cca3b28212a075b963260577a508f50813f6e5838ac3b2ceb805e774c930145e66bf93418a48719a5ed6af3792d420710487a3385b906a2adb6026f28e19a811a2999c3028fd42fafab7ae8ae67c2ad31808554e2aed443791d975e992f33cfc16f05901bee325899d0d260a6ae4811be2af94060881e8ffbdde7e781f44fd4f3dede561b45418f3fab9666937e748c56f8e27f06583d38bd7ee8aea5a4b30a68a9cd5df78ccc939aa788d4c010f5df9995b8ad798321ecbb224e2a8658b2a2197ce900c4f314b171e3039b294e39b164c505d0b7ac865de9309079e76825fc03fed8150bb08cb8a8e6d3c89a99f9a91da1a05c5df8683c39724d685d773019ecedb94fe8f811ec81992f06a228db9b2950910c589b2618bd3734d1fcbbdb86aa3013fd14a849486b03eaa26d22aff6e771a3b2d7ddc83f09b2e6ccd5cfe2fc97a2f1971543dad5d858bac5d35e213fe9d32545a0142c9677630e411bb149e8aae024620a229cbbc6c1e665f64d929220373d66501e69a4afb83a86a867204ae019a974f901d8ed242832e84b8b82fc703a9f558ec5432c319a2e3dcf608cdcbb4aec6014f884a05b74db187022a9f37a276666548d2db4326f3e10ea09d2d8b979df9ac11b9b7cb07d547702c4d28daa6d5a9fdea46eaab305aa816562ce824fda10dd09e51b6417a8a38be7991885d0e541abd9174a4eb3e600c2a8e83f9296424be176efe7c4a59dd1599001b85d9ab0b740ee2263c88f2fe029773963f95aac0af6b6042e19b214d1da501c9a6d8ac304ca4f666b6a7b71d0f85f273bcd39d1772764d0f09aa672962b94c9a0d71bf5f211f631f817728a6b070b00ac90a9f5076b0e47ddd5086d99952debdd32e8982b84683215e77e16fb2e104d8cbbb0581757f784ed162f4b5a69b84ebc03ec09aa85f70956d6bcbcd4601a686f024a44ffd4563863ea8ef9c34400aafce2e4243197fc697387806a7fd0e1fcb1a63c7e1c90fd02a2945761e5b37eadbe4b97b08644eebb2687e58008c72189c92b260e9e6baffe70d7a9baa75e23ce7e78280cc3e56768254d86b44a386f27734923f2c9180c780da3df00ba79a283f5ea9333ea778983c595af1198d15527a861b130dff94f1ba5430b6960497b89eddb87b8befb44d67afcbdb2c2c3385c3292dc5e0bcd4582ae9a481c78af26c677f73175b945e659c8f0250b99d4b712d7d7cf99c1919eb5e3b1445d4ab36d3dac7c5ed15e4b7abd9e0a2c5d486caa64517656567cd6a9f90ab54e409fd340aabdc195a8ef308ef9934cf6fe1fdab5c996bb2e77e40a626cb8d550783fd616753da6ae8dcb4db44ef5cf3c7099155a6e01d63e8ac54e560296ab948d5077e8ad09c9fb2b805b9ab5d4c61318da26249d5cf0e05b266b99b3a6119cd1029d53ab94807ad33f759e04c8c7acabb712bb330cef0ce6e333ce30ae517fb168a4c86b7e4ad3a9b46dbcfd272a4fec0ecbf76996782460a82d1b47c1f09c38d2aa55f4a670d4b01658cf8c07b53b5d0f54d5dd1aab7ec060888bf2373bf1670e1dc506bc3831d0cd6a4de720596674d114b6dbfc7d8d63f773a406c3328d1e87ff8c9d764772a7c9f67d36f9615d5f9d2ab52c9de938e5c2c321672905c061338eddf60ee46d32858f28205dc1b23e9fb72825e6ae1319599c04ad79ded9399fa0d987e4ff0aa61d38fcb24b36de2d157f43872a205d9208e45d63b162f981660455495bc65d6333c260a8c7ff25a95dcc28c2c1d76a6137c0240487d5ef06c1b26721fff971106925996f59aec193c775307a3336a8c2be18c654f343e7d325f6b0f07642340d13742d7d2998e8b6ea5351b5910464a3dd11423749e9ab7eef1b4a1d58eac13328693e13dc95575c586c0173df37ddc606cd9978198c0c7b4eda7228ccd05fbeec7e2baaf348698bdde5beff6e9980b43cd540151e107a94e9a05019084b194308f34549bcb34948f252c88383a1ef0cfdb97de34b1996b5701c7c3895e8e46c3510d5f97c14b4451dd5d774f978851fb07be0abb77e59d8e648ec3a60308086ef87cedf1c8e4aae105115ef8c2fad162c010d5cb7083ff92b41dddb5e810d0d2ea778429009ad3ac7c676c9f26722b021db8f7e63d448057c2f74f96715799733bb7259137a7e12b8151b6c986008e6b94054a908cc839664e49e6845efd610e54ca1b4e54a52a14a5e4f03ab437815cd26d03509024a02e9981089e23ee7c6fd29d5e137ee5a31500547a0b51e22f49f4357b1bd4af443ea139bd9455c7bb509ce649edad74fef3dd29d9cff5f12647e6c0823c6bf3591d3b23b09d815be95e8241e47e90a7b6c6991301cb52d4b089ab92fc2892d469202f877ec9f1a79ddf81a8266287d992633bc4aaf8a69b534da4353fefd7309f47307f587098104b8af2f7bf1d15df9e76daa90f59898fb7d5c2b29aff2f2377a5a2099fdf6a6362f46721a7e2c7cda7d24800d0e8eed254e11868160cfb5fbe4453e98aa97a5736efa0083e72b73bbfab1446571d7dca15817f5396b4d6af68c8c892dfa9341322e571a1326a55861737774064548071594925fd0e1f934431f0091fc87a7fe1d78c970868c9f678482f1de6a7fcd673d447a6d7a3c77dbd31c13fdca81604250206cff0f1e37bf9963c79c1ae49f5605e8983ca9f6d73e5472f38a19c64e8f67972db91f8b3f94416a6b1c605855c9fc0ad4de8cef348981fded65f1dbc2c27c5e53a53d78f1beea13f8b707ccde3c9f70d8b7e85fd6e214bdb20f10f5d84dec3225f9ed10e49cd4735aaf334c40e87cb887af792c419caab89fb5798c42d486e4fe23e506ce585ea8cd97bef49ad288e7eb3af0ab7fa2de8eae41484102a9caafa867417a605353575254d12d9a2041fcb274fcc11fc30981569c82cb9fb55afc695e0fba45383102eea79102c841f6cf03172c6a27e071d35445edce6079a1d4c659fb700e9e28377a901194b6cda9118027d6915a737923849cdb0169a43086df3e19038807d4bc5074ae88378c18d3af53ea1cbf685e1dce84c031e9cab0ddc1b222cf37db1c32fb2e245978f9627b496678c4562f5b6578a547b99cee97170ad0544edcce3c4919161159f75d72068dd3dc1236ceb83e61febd9038779d90a8dd727714d578d9a7687389c27ccef1f0c5f907ba840229b3a35c78eb1a1d70e0ed5cb7e478da5d1c9256a89debdd23cbf2bb996cc8684378a798f0fafb6f0b1c863342d5ef3ef9ffb7aa2fd5a68bdb1f5cb4b18f7c54a154594d72db342d70d18d4ba3d30580c67c07570b054cee23694c190196d0e4dd1680ec2720e54ec261ffa7048901078daddb3232bf3523fb4916c35c9641c9bc85ffbeda6be76a67d2ebb461a70a63ddf2c8bac02460f47ab5e2e7971bed50c27b4308a2e7c842ce54c7b7dc8f3611ed9638cfe2f6f92047b0f61c3918aab1345b531653bc9db850220b907125d7d0029f8aa3ddb6790b3dcc240eb6d658f718bc26b808891dc8359625f3c9c8ce08e212fb71392538bedaa38e5c7e905818df43be6d3fa95051943223f192d98ed9bdebd7b95b60b7b96e7c8ceaa2f54563432575e8bf5537d5d2a69cec457c52d8f9fa627f96b4e57cedfdec26b06d67b8cc1b2a57c3b1d4d0b5b459f7f67f2d6fae5604f8862c16486d4dd9f48f6fe85090829d0c8c8ed0bd7743e33689341a361e9220d10b01a670755827c024765dc90ea550e897b38fcb32b459068e6ae5d5ffeb8181d679cebce7b24e2d7b87897d2383bf27493a2e8fd97b888e1a8ba0b47e4bec35ad1e89df991e6a96d774522706a3f25041a9d0072c0e86a9bd8f51e787839e272cc941779fc4646ccabccee243d80d76493cf8243599d88f17e9785a80b46595057e9f9a96085251008251f67a8aea6d156a11e9d340b3250c3399b98492be02cb708d6be67f4985dc8174592b5ba70f27c23277287ebf5547026b6d04bf86335adfba016949c9c30ecff2d78217a516f647559537f150f2e1044e15ecf9ce2150fee31ec9e82acd6652a649af4e115ff8f123e15d5ece21c82a6f2ed44c8de7ff48b0983f6884f52107345ab727bb26006d2f9c809e8caf7105167fa2f66fe816d98ee3e19109c051138c4ae6b83e6e4593e2ee77b5610d6ec04249373424e35a497c093146fb30702a7b6b42f0a84c0c8ebca4f1277bc591518c01e6d7d76f5c9aeb24969bf277dd7e416564549270fed85d906a76ebcee5637ff1e7a7f8ee37ba717f7e8a78dbc2a38d74a665c8792fb7881a3f086702ab184d64b90fdbb884b1df243db50762b43bf16a508ae6ffdca2ccf960fc4bca5f38a71235210efcc1917bc0c910a665c36e8e78f45d7000e2c7e75ac87253cad97c5be1c4631e4c9fa44229f831402c08ca6a1f3ddb124707c951b40930fc82ee2fdab92e3283cc3f11a5e4a635cbf3da998a7d44eb67dec08cca4be509f206d10f335dd7d1aa0ce8b80e3fa98a773efac39ea5d4978da154d074a78983e086831cb5730e99beed6462aadced31439b8e759b51cf6a8f8785dfa470302ba4b43669fd9e50fc78f27cb0973dca924e1c458f745faaa50471fb96aa30d0760abe235cb9f04b947233c462ba6a5024227157b9a372cdb5331a9761abcef74636ca400625bed7bf55a3b815ffe0be6c1bad9ed0658be921dd4ffd5380b2d3066baaec1a525f96855e64d494ff1312761cf79f8889022015ddfc46eaa0f5a29f4c0904f992a8bad368d4a667860e594c540ba1b1ea1db2061609b66dbe0957ceeed9c3c246c513944d8ae40e7f1d4f773649f90c45b464a864ed4cf670f6501016f91287c3c25b3d4c9e0d7fae1c0607390c0f21d5a80df549af915d8b41e650db87eedd2b4db7968f4e48101450c77346063c31d8ccb6b13f20f5c33f51236d372f82c960aa511aaf16ee42fc032ddd5b286189aba455b5b98529770b98f8f479423451120736d7ed142af2bf2280fe7e843a2b867a98268fb16397742344d73587d1648056120648d8424eb610ff4bab60813cd39615794745f59792d5ed30f4758098c83f5dcfff36b7fba362068a9a60bb8af60b4430e7cb5634cb21149e943d344197302a05dec0227741aa5d3a7ada9175e22ef0b3daf9cda64647f624acb06030ae15a3b49b3bc3c63e20bbab663478d6b2e49a97a387476cf9419a1c253a5f0157e9eaa1608d69e3f4f298e912bf0727c0f8e78368643dc9b60aecfdd681ef3fafd13aa440c462ceee89b9904e7a3dfd77892b864d2dcdcb54835d85abdc844d90a64e7e21e6064454ca4fe3f58897ef2cd699067109671ec1886c39b5249d9f9027efe580fa4d7b4b33ed6ad28833d5e7e97423a7167bf914ac0afaa6aa98eebb7d9e7d38cea3411eed5671fefde50df629610abd69f07c47d6c28ec631b9c27b2d070c9846b19f8638707079725425e7688705cbd30e81264bd333c5c66c6c3ff479272b98e9e2fd2b6332e8c455c0b25db8dc6570fd78381a169982c7ddf2fd546f200aac7ba433aa464ae6a1f79d1042f5468004ea4b5493e94636d618b3fd009f969b34e1ce421bc7340998c438be884423e445de1739258b2edad75a7fe1cd80803dbc87569bb442ab421b1cd2ea6421b3f1bca23e858e5da640f48b40f4cebcee544b51596a147d1b07d300975344a53a56a7fca4a79c61e83c7c0ff1c951b703a5be7efdbcafd4df9a2486688d4caa279e49c423779b1144947779e193fa20921429b873d2d27fb7528766c50af2dceaa84730636714d8a10fa450d1686f93a01333f11b1cc6bbe8d8e49961d151ca883eafe69eb7a1746253fea9902516f53f7c0fdcf99764205b67699f75ad8654de378e461db5a7cf04c8f79a386d3e38e60e229a1131d8eb203fcc551f791a0c5891639ac8c012072f444ec67d04ef165aef3cefd8be9c5f51700149e37e19ddf63a548d181d67181fefccd140241f8e803f25b5fb0c68f65d92f12d4ac1402cab1958ba147ff9b76c34a8efcad19b55f829cc20a0321cd565f35ceac8f94b3467f70c22b0da24dee9085ba4cb01c55ea5bc82caea13cc9e7be56c541ccc5bcd8213ab6e7897a27d62545639261701f1525c705a68e993b2682e20beff3c01b89977795f396e3fb63e38e18724749479d03dd75be08340b319cffb6135d4a49f1ad8843d676952193ef3f603b66ca2dd9d040f41d2d711128689e8747c8f564a346a161c1787b4db87c21a796de1eb2c33fba6ee724204baccb857b9cbf9645b2777de9dc7f2f9c26ebab5d5ccfb6d18cc1a84f687baa357c46f63732ccd475342ad6cb910270786f70c3c3c3afcbff771c2dc6062605ef8794684e39cffcebef709e7ba45a6415918360518385d3c172a14f93e7d0ab6a7c441e4abe85d092c49249d3c6367f9ef2b652737b37566f8ce3819f9388b46879b5e7289f94a4534a4f42675630bbc0a2ff5c35119e25b4af3b0ede3451f83778273af2544de7fd649478be9ee0fb00e9c31e7feb64eed7854a0047b3c737be344353d9b09f5c0af5929ba0a6996860ce0f0aa0af5099141f98de83fd217431336e63f257585f26a01799bb677a2e8e946a7609a230ff3dbee4b47898d814190f6fc3f17551039c8eebec2af4538e7ba821846361c8017f7a5756dab2695ec36b603672e3104254722bdab6741c9c3a314a263d313e000bfc4689307eabeaf52f41d1c1b0d5a221c5a44c0ce5eaa34cb1b540e7ac8a8e7ea277dc0f660434d4d80e6e83688a462aa29f248785d0c001c7f9d9a2cca6df931b0b1bde4e7bd901d41832b06f67f8db120a6074319d9458a2157e3137acee5b756450bfd8de7bcc0328bd8dd67ca3b49e35723bb52861afed7a84f5d87908703f42143e624898cabd67fc54391e6d2b3e3e77f45f9675fc1a400710d35f772f2bd9df5ba873d5d7ef0397a2f6b321aa0ab1dfb8f3643f3404721c66a09d9f8575a6a2495b798c27a6138acf7de865988dcf18aabca967e905bdc0b64f9c057007677760a23f741d1553b48aca0a92613f6dc603184beab93b1e453af1617d3acce56893dbceddc4d1d15a0ea47b9b3c39edb1d551a7f733ea2955412d17fea2b6547db8a7d94253b7dde883e8cc9244ee7fb2ab19c0ae7500178fa22823d433aaf0df035311fea57283be90f9948a9eadf3d1be704655103aca1f37ab1700a4ef1aaf767e8d185aae082d4a50c4635109df1ad524bacc4cd6ab
Verified      : true
```

Find the example program [here](./example/sphincs%2B-shake.cpp), demonstrating keygen/ sign/ verify API usage.
