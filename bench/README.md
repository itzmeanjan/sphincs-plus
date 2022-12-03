## Benchmarking SPHINCS+ DSA

For benchmarking SPHINCS+ implementation for different parameter sets ( as suggested in the specification's section 7.2 and table 3 ), you may issue


```bash
make benchmark
```

> **Note**

> For benchmarking ensure presence of `google-benchmark` header & library in well known locations.

>**Warning**

> CPU frequency scaling based on demand **must** be disabled when collecting benchmark results, you may follow [this](https://github.com/google/benchmark/blob/2257fa4/docs/user_guide.md#disabling-cpu-frequency-scaling) guide for doing so.

> **Note**

> On x86_64 architecture average CPU cycle spent in keygen/ sign/ verify routine is measured using compiler built-in `__rdtsc`. I've tested it with both Clang and GCC.

### On Intel(R) Core(TM) i5-8279U CPU @ 2.40GHz [ compiled with Clang ]

```bash
2022-12-03T10:23:05+04:00
Running ./bench/a.out
Run on (8 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB
  L1 Instruction 32 KiB
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB
Load Average: 1.68, 2.56, 2.43
--------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                                              Time             CPU   Iterations average_cpu_cycles items_per_second
--------------------------------------------------------------------------------------------------------------------------------------------
bench_sphincs::keygen<16, 63, 7, 16, variant::robust>                256 ms          251 ms            3           614.072M         3.9787/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::robust>         1795 ms         1777 ms            1           4.30759G       0.562762/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::robust>       1.65 ms         1.65 ms          394           3.96955M        605.673/s
bench_sphincs::keygen<16, 63, 7, 16, variant::simple>                116 ms          116 ms            6           278.159M        8.63912/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::simple>          889 ms          888 ms            1           2.13458G        1.12629/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::simple>      0.894 ms        0.893 ms          762           2.14532M        1.1201k/s
bench_sphincs::keygen<16, 66, 22, 16, variant::robust>              3.51 ms         3.50 ms          197           8.41719M        285.495/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::robust>         80.0 ms         79.9 ms            9            191.93M        12.5168/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::robust>       5.00 ms         4.99 ms          129           11.9951M        200.243/s
bench_sphincs::keygen<16, 66, 22, 16, variant::simple>              1.88 ms         1.88 ms          371           4.51078M        533.263/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::simple>         42.2 ms         42.1 ms           16           101.278M        23.7285/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::simple>       2.64 ms         2.64 ms          281           6.34082M        379.495/s
bench_sphincs::keygen<24, 63, 7, 16, variant::robust>                331 ms          331 ms            2           795.119M        3.02216/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::robust>         2895 ms         2889 ms            1           6.94883G       0.346083/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::robust>       2.70 ms         2.63 ms          288           6.47717M        380.694/s
bench_sphincs::keygen<24, 63, 7, 16, variant::simple>                180 ms          179 ms            4            431.49M        5.59681/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::simple>         1614 ms         1603 ms            1           3.87382G       0.623782/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::simple>       1.30 ms         1.29 ms          507            3.1291M        775.846/s
bench_sphincs::keygen<24, 66, 22, 16, variant::robust>              5.45 ms         5.32 ms          125           13.0726M        188.005/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::robust>          132 ms          132 ms            5            316.37M        7.60192/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::robust>       7.50 ms         7.49 ms           87           17.9886M        133.563/s
bench_sphincs::keygen<24, 66, 22, 16, variant::simple>              2.79 ms         2.76 ms          253           6.69528M        362.399/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::simple>         69.9 ms         69.8 ms           10           167.689M        14.3299/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::simple>       3.81 ms         3.80 ms          183           9.13306M        263.229/s
bench_sphincs::keygen<32, 64, 8, 16, variant::robust>                212 ms          212 ms            3           508.027M        4.72717/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::robust>         2529 ms         2517 ms            1           6.07026G       0.397274/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::robust>       3.62 ms         3.61 ms          195           8.68335M        276.873/s
bench_sphincs::keygen<32, 64, 8, 16, variant::simple>                117 ms          115 ms            6           281.547M        8.70046/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::simple>         1367 ms         1365 ms            1            3.2814G       0.732507/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::simple>       1.83 ms         1.83 ms          377           4.39515M        546.518/s
bench_sphincs::keygen<32, 68, 17, 16, variant::robust>              13.3 ms         13.3 ms           52            31.987M        75.0685/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::robust>          262 ms          262 ms            3            629.96M        3.81368/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::robust>       7.14 ms         7.13 ms           91           17.1247M        140.257/s
bench_sphincs::keygen<32, 68, 17, 16, variant::simple>              6.99 ms         6.98 ms           98           16.7714M        143.315/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::simple>          140 ms          140 ms            5           336.049M        7.14717/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::simple>       3.77 ms         3.76 ms          184           9.04923M        265.843/s
```

### On ARM Cortex-A72 [ compiled with GCC ]

```bash
2022-12-03T06:26:40+00:00
Running ./bench/a.out
Run on (16 X 166.66 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 48 KiB (x16)
  L2 Unified 2048 KiB (x4)
Load Average: 0.23, 0.06, 0.02
-------------------------------------------------------------------------------------------------------------------------
Benchmark                                                              Time             CPU   Iterations items_per_second
-------------------------------------------------------------------------------------------------------------------------
bench_sphincs::keygen<16, 63, 7, 16, variant::robust>                996 ms          996 ms            1        1.00379/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::robust>         7502 ms         7502 ms            1       0.133296/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::robust>       7.93 ms         7.93 ms           91        126.181/s
bench_sphincs::keygen<16, 63, 7, 16, variant::simple>                533 ms          533 ms            1         1.8775/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::simple>         4050 ms         4050 ms            1       0.246935/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::simple>       4.13 ms         4.13 ms          175        242.128/s
bench_sphincs::keygen<16, 66, 22, 16, variant::robust>              15.6 ms         15.6 ms           45        64.2132/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::robust>          362 ms          362 ms            2        2.76177/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::robust>       23.1 ms         23.1 ms           31        43.3178/s
bench_sphincs::keygen<16, 66, 22, 16, variant::simple>              8.33 ms         8.33 ms           84        120.028/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::simple>          194 ms          194 ms            4        5.14218/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::simple>       11.3 ms         11.3 ms           61        88.2496/s
bench_sphincs::keygen<24, 63, 7, 16, variant::robust>               1495 ms         1495 ms            1       0.669077/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::robust>        13025 ms        13025 ms            1      0.0767778/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::robust>       11.0 ms         11.0 ms           64        90.6761/s
bench_sphincs::keygen<24, 63, 7, 16, variant::simple>                778 ms          778 ms            1        1.28599/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::simple>         6989 ms         6989 ms            1       0.143075/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::simple>       5.79 ms         5.79 ms          119        172.769/s
bench_sphincs::keygen<24, 66, 22, 16, variant::robust>              23.4 ms         23.4 ms           30        42.8054/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::robust>          593 ms          593 ms            1        1.68592/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::robust>       33.8 ms         33.8 ms           20        29.6261/s
bench_sphincs::keygen<24, 66, 22, 16, variant::simple>              12.2 ms         12.2 ms           58        82.2358/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::simple>          313 ms          313 ms            2        3.19054/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::simple>       17.2 ms         17.2 ms           41        58.3067/s
bench_sphincs::keygen<32, 64, 8, 16, variant::robust>                984 ms          984 ms            1        1.01665/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::robust>        11183 ms        11183 ms            1       0.089422/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::robust>       16.3 ms         16.3 ms           41        61.2408/s
bench_sphincs::keygen<32, 64, 8, 16, variant::simple>                512 ms          512 ms            1         1.9518/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::simple>         6097 ms         6097 ms            1       0.164008/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::simple>       8.71 ms         8.71 ms           84        114.796/s
bench_sphincs::keygen<32, 68, 17, 16, variant::robust>              61.5 ms         61.5 ms           11        16.2648/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::robust>         1215 ms         1215 ms            1       0.822877/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::robust>       33.5 ms         33.5 ms           21        29.8238/s
bench_sphincs::keygen<32, 68, 17, 16, variant::simple>              32.0 ms         32.0 ms           22        31.2194/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::simple>          643 ms          643 ms            1        1.55555/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::simple>       17.3 ms         17.3 ms           40        57.9554/s
```

### On ARM Cortex-A72 [ compiled with Clang ]

```bash
2022-12-03T06:31:59+00:00
Running ./bench/a.out
Run on (16 X 166.66 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 48 KiB (x16)
  L2 Unified 2048 KiB (x4)
Load Average: 0.59, 0.55, 0.27
-------------------------------------------------------------------------------------------------------------------------
Benchmark                                                              Time             CPU   Iterations items_per_second
-------------------------------------------------------------------------------------------------------------------------
bench_sphincs::keygen<16, 63, 7, 16, variant::robust>                876 ms          876 ms            1        1.14101/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::robust>         6588 ms         6588 ms            1       0.151795/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::robust>       6.88 ms         6.88 ms          108        145.432/s
bench_sphincs::keygen<16, 63, 7, 16, variant::simple>                456 ms          456 ms            2        2.19159/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::simple>         3465 ms         3465 ms            1       0.288632/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::simple>       3.36 ms         3.36 ms          210        297.973/s
bench_sphincs::keygen<16, 66, 22, 16, variant::robust>              13.7 ms         13.7 ms           51        72.9851/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::robust>          317 ms          317 ms            2        3.15369/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::robust>       19.4 ms         19.4 ms           37        51.5415/s
bench_sphincs::keygen<16, 66, 22, 16, variant::simple>              7.14 ms         7.14 ms           98        140.064/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::simple>          166 ms          166 ms            4        6.01157/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::simple>       9.90 ms         9.90 ms           72        101.015/s
bench_sphincs::keygen<24, 63, 7, 16, variant::robust>               1281 ms         1281 ms            1       0.780672/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::robust>        11158 ms        11158 ms            1       0.089623/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::robust>       9.88 ms         9.88 ms           70        101.179/s
bench_sphincs::keygen<24, 63, 7, 16, variant::simple>                663 ms          663 ms            1        1.50726/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::simple>         5967 ms         5967 ms            1       0.167592/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::simple>       5.05 ms         5.05 ms          100         198.06/s
bench_sphincs::keygen<24, 66, 22, 16, variant::robust>              20.0 ms         20.0 ms           35        49.9389/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::robust>          506 ms          506 ms            1        1.97751/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::robust>       28.8 ms         28.8 ms           25        34.6769/s
bench_sphincs::keygen<24, 66, 22, 16, variant::simple>              10.4 ms         10.4 ms           67        96.3674/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::simple>          268 ms          268 ms            3        3.73551/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::simple>       14.4 ms         14.4 ms           48         69.381/s
bench_sphincs::keygen<32, 64, 8, 16, variant::robust>                844 ms          844 ms            1        1.18464/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::robust>         9588 ms         9588 ms            1       0.104294/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::robust>       14.6 ms         14.6 ms           50        68.3203/s
bench_sphincs::keygen<32, 64, 8, 16, variant::simple>                437 ms          437 ms            2        2.28716/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::simple>         5207 ms         5207 ms            1       0.192043/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::simple>       7.05 ms         7.05 ms           97         141.78/s
bench_sphincs::keygen<32, 68, 17, 16, variant::robust>              52.8 ms         52.8 ms           13        18.9511/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::robust>         1036 ms         1036 ms            1       0.965233/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::robust>       29.5 ms         29.5 ms           24        33.9418/s
bench_sphincs::keygen<32, 68, 17, 16, variant::simple>              27.3 ms         27.3 ms           26        36.5798/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::simple>          549 ms          549 ms            1        1.82213/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::simple>       15.0 ms         15.0 ms           49        66.6681/s
```

### On ARM Neoverse-V1 [ compiled with GCC ]

```bash
2022-12-03T06:37:52+00:00
Running ./bench/a.out
Run on (64 X 2100 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x64)
  L1 Instruction 64 KiB (x64)
  L2 Unified 1024 KiB (x64)
  L3 Unified 32768 KiB (x1)
Load Average: 0.11, 0.03, 0.01
-------------------------------------------------------------------------------------------------------------------------
Benchmark                                                              Time             CPU   Iterations items_per_second
-------------------------------------------------------------------------------------------------------------------------
bench_sphincs::keygen<16, 63, 7, 16, variant::robust>                487 ms          487 ms            2         2.0536/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::robust>         3513 ms         3513 ms            1       0.284661/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::robust>       3.84 ms         3.84 ms          182        260.357/s
bench_sphincs::keygen<16, 63, 7, 16, variant::simple>                241 ms          241 ms            3        4.15161/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::simple>         1829 ms         1829 ms            1       0.546655/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::simple>       1.80 ms         1.80 ms          367        556.754/s
bench_sphincs::keygen<16, 66, 22, 16, variant::robust>              7.61 ms         7.61 ms           92        131.421/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::robust>          180 ms          180 ms            4        5.55005/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::robust>       12.3 ms         12.3 ms           61        81.0357/s
bench_sphincs::keygen<16, 66, 22, 16, variant::simple>              3.76 ms         3.76 ms          186        265.761/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::simple>         88.3 ms         88.3 ms            8        11.3253/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::simple>       5.50 ms         5.50 ms          126        181.733/s
bench_sphincs::keygen<24, 63, 7, 16, variant::robust>                686 ms          686 ms            1        1.45869/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::robust>         6067 ms         6067 ms            1       0.164819/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::robust>       5.05 ms         5.05 ms          131        197.854/s
bench_sphincs::keygen<24, 63, 7, 16, variant::simple>                354 ms          354 ms            2        2.82576/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::simple>         3183 ms         3183 ms            1       0.314138/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::simple>       2.60 ms         2.60 ms          257        384.706/s
bench_sphincs::keygen<24, 66, 22, 16, variant::robust>              10.7 ms         10.7 ms           65        93.4217/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::robust>          271 ms          271 ms            3        3.69439/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::robust>       15.5 ms         15.5 ms           45        64.6045/s
bench_sphincs::keygen<24, 66, 22, 16, variant::simple>              5.53 ms         5.53 ms          127        180.716/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::simple>          143 ms          143 ms            5        6.97557/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::simple>       7.72 ms         7.71 ms           91        129.618/s
bench_sphincs::keygen<32, 64, 8, 16, variant::robust>                451 ms          451 ms            2        2.21959/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::robust>         5228 ms         5228 ms            1       0.191293/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::robust>       7.48 ms         7.48 ms           94        133.724/s
bench_sphincs::keygen<32, 64, 8, 16, variant::simple>                229 ms          229 ms            3        4.36064/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::simple>         2771 ms         2771 ms            1       0.360925/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::simple>       3.77 ms         3.77 ms          191        265.451/s
bench_sphincs::keygen<32, 68, 17, 16, variant::robust>              28.2 ms         28.2 ms           25        35.5134/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::robust>          553 ms          553 ms            1        1.80732/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::robust>       15.4 ms         15.4 ms           45        65.1215/s
bench_sphincs::keygen<32, 68, 17, 16, variant::simple>              14.3 ms         14.3 ms           49        69.7652/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::simple>          295 ms          295 ms            2        3.38706/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::simple>       7.97 ms         7.97 ms           85        125.467/s
```

### On ARM Neoverse-V1 [ compiled with Clang ]

```bash
2022-12-03T06:41:46+00:00
Running ./bench/a.out
Run on (64 X 2100 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x64)
  L1 Instruction 64 KiB (x64)
  L2 Unified 1024 KiB (x64)
  L3 Unified 32768 KiB (x1)
Load Average: 0.36, 0.33, 0.15
-------------------------------------------------------------------------------------------------------------------------
Benchmark                                                              Time             CPU   Iterations items_per_second
-------------------------------------------------------------------------------------------------------------------------
bench_sphincs::keygen<16, 63, 7, 16, variant::robust>                290 ms          290 ms            2        3.44301/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::robust>         2181 ms         2181 ms            1       0.458455/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::robust>       2.31 ms         2.31 ms          299        432.887/s
bench_sphincs::keygen<16, 63, 7, 16, variant::simple>                150 ms          150 ms            5        6.64974/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::simple>         1154 ms         1154 ms            1       0.866394/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::simple>       1.10 ms         1.10 ms          577        912.603/s
bench_sphincs::keygen<16, 66, 22, 16, variant::robust>              4.54 ms         4.54 ms          154        220.156/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::robust>          105 ms          105 ms            7        9.53219/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::robust>       6.40 ms         6.40 ms          108        156.286/s
bench_sphincs::keygen<16, 66, 22, 16, variant::simple>              2.35 ms         2.35 ms          298        424.876/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::simple>         55.4 ms         55.4 ms           13        18.0517/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::simple>       3.18 ms         3.18 ms          212        314.024/s
bench_sphincs::keygen<24, 63, 7, 16, variant::robust>                433 ms          433 ms            2        2.30986/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::robust>         3767 ms         3766 ms            1       0.265502/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::robust>       3.39 ms         3.39 ms          219        294.634/s
bench_sphincs::keygen<24, 63, 7, 16, variant::simple>                223 ms          223 ms            3        4.47648/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::simple>         1996 ms         1996 ms            1        0.50112/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::simple>       1.72 ms         1.72 ms          404        580.379/s
bench_sphincs::keygen<24, 66, 22, 16, variant::robust>              6.77 ms         6.77 ms          103        147.718/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::robust>          173 ms          173 ms            4        5.79366/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::robust>       9.82 ms         9.82 ms           74        101.842/s
bench_sphincs::keygen<24, 66, 22, 16, variant::simple>              3.49 ms         3.49 ms          200        286.207/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::simple>         89.9 ms         89.9 ms            8         11.129/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::simple>       5.01 ms         5.01 ms          140        199.582/s
bench_sphincs::keygen<32, 64, 8, 16, variant::robust>                282 ms          282 ms            2        3.54719/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::robust>         3210 ms         3210 ms            1       0.311554/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::robust>       4.70 ms         4.70 ms          148        212.749/s
bench_sphincs::keygen<32, 64, 8, 16, variant::simple>                145 ms          145 ms            5        6.89775/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::simple>         1741 ms         1741 ms            1       0.574371/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::simple>       2.32 ms         2.32 ms          307        431.509/s
bench_sphincs::keygen<32, 68, 17, 16, variant::robust>              17.6 ms         17.6 ms           40        56.7595/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::robust>          348 ms          348 ms            2        2.87119/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::robust>       9.64 ms         9.64 ms           71        103.692/s
bench_sphincs::keygen<32, 68, 17, 16, variant::simple>              9.06 ms         9.06 ms           77         110.33/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::simple>          181 ms          181 ms            4        5.53838/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::simple>       4.71 ms         4.71 ms          145        212.312/s
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz [ compiled with GCC ]

```bash
2022-12-03T06:45:15+00:00
Running ./bench/a.out
Run on (4 X 2300.25 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.23, 0.07, 0.02
--------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                                              Time             CPU   Iterations average_cpu_cycles items_per_second
--------------------------------------------------------------------------------------------------------------------------------------------
bench_sphincs::keygen<16, 63, 7, 16, variant::robust>                606 ms          606 ms            1            1.3942G        1.64999/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::robust>         4569 ms         4569 ms            1           10.5092G       0.218865/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::robust>       5.03 ms         5.03 ms          144           11.5658M        198.868/s
bench_sphincs::keygen<16, 63, 7, 16, variant::simple>                327 ms          327 ms            2           752.444M        3.05676/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::simple>         2541 ms         2541 ms            1            5.8446G       0.393541/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::simple>       2.50 ms         2.50 ms          273           5.75719M        399.505/s
bench_sphincs::keygen<16, 66, 22, 16, variant::robust>              9.47 ms         9.47 ms           74           21.7891M        105.563/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::robust>          220 ms          220 ms            3            506.66M        4.53987/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::robust>       13.9 ms         13.9 ms           51           31.9107M        72.0771/s
bench_sphincs::keygen<16, 66, 22, 16, variant::simple>              5.25 ms         5.25 ms          133           12.0761M         190.46/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::simple>          124 ms          124 ms            6            284.57M        8.08282/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::simple>       6.71 ms         6.71 ms          100            15.422M        149.138/s
bench_sphincs::keygen<24, 63, 7, 16, variant::robust>                913 ms          913 ms            1           2.10015G        1.09522/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::robust>         7973 ms         7973 ms            1           18.3383G       0.125426/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::robust>       7.13 ms         7.13 ms           97           16.4073M        140.184/s
bench_sphincs::keygen<24, 63, 7, 16, variant::simple>                478 ms          478 ms            2           1098.72M        2.09349/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::simple>         4292 ms         4292 ms            1           9.87112G       0.233017/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::simple>       3.64 ms         3.64 ms          197           8.36139M        275.086/s
bench_sphincs::keygen<24, 66, 22, 16, variant::robust>              14.3 ms         14.3 ms           49           32.8344M        70.0496/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::robust>          362 ms          362 ms            2           832.938M        2.76134/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::robust>       20.9 ms         20.9 ms           33           48.1603M        47.7598/s
bench_sphincs::keygen<24, 66, 22, 16, variant::simple>              7.47 ms         7.47 ms           94           17.1805M        133.873/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::simple>          193 ms          193 ms            4           443.678M        5.18417/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::simple>       10.5 ms         10.5 ms           68           24.1866M        95.1006/s
bench_sphincs::keygen<32, 64, 8, 16, variant::robust>                601 ms          601 ms            1            1.3816G        1.66476/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::robust>         6912 ms         6912 ms            1           15.8974G       0.144685/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::robust>       9.95 ms         9.95 ms           66           22.8939M         100.46/s
bench_sphincs::keygen<32, 64, 8, 16, variant::simple>                314 ms          314 ms            2           722.326M        3.18422/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::simple>         3726 ms         3726 ms            1           8.56965G         0.2684/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::simple>       5.03 ms         5.03 ms          134           11.5695M        198.795/s
bench_sphincs::keygen<32, 68, 17, 16, variant::robust>              37.5 ms         37.5 ms           19           86.3636M        26.6323/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::robust>          739 ms          739 ms            1           1.69913G        1.35367/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::robust>       20.9 ms         20.9 ms           33           48.1533M        47.7678/s
bench_sphincs::keygen<32, 68, 17, 16, variant::simple>              19.6 ms         19.6 ms           36           45.1732M        50.9187/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::simple>          394 ms          394 ms            2           905.854M        2.53922/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::simple>       10.1 ms         10.1 ms           67           23.1833M        99.2144/s
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz [ compiled with Clang ]

```bash
2022-12-03T06:51:54+00:00
Running ./bench/a.out
Run on (4 X 2300.25 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.19, 0.28, 0.17
--------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                                              Time             CPU   Iterations average_cpu_cycles items_per_second
--------------------------------------------------------------------------------------------------------------------------------------------
bench_sphincs::keygen<16, 63, 7, 16, variant::robust>                315 ms          314 ms            2           723.402M        3.17971/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::robust>         2361 ms         2361 ms            1           5.43084G       0.423521/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::robust>       2.48 ms         2.48 ms          313           5.70916M        402.874/s
bench_sphincs::keygen<16, 63, 7, 16, variant::simple>                163 ms          163 ms            4           373.887M        6.15232/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::simple>         1240 ms         1240 ms            1           2.85145G       0.806669/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::simple>       1.20 ms         1.20 ms          586           2.75683M        834.273/s
bench_sphincs::keygen<16, 66, 22, 16, variant::robust>              4.92 ms         4.92 ms          142           11.3134M        203.304/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::robust>          113 ms          113 ms            6           260.085M         8.8438/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::robust>       6.95 ms         6.95 ms           99           15.9773M        143.959/s
bench_sphincs::keygen<16, 66, 22, 16, variant::simple>              2.55 ms         2.55 ms          275           5.86064M        392.453/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::simple>         59.4 ms         59.4 ms           12           136.552M        16.8445/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::simple>       3.64 ms         3.64 ms          204           8.36728M        274.876/s
bench_sphincs::keygen<24, 63, 7, 16, variant::robust>                468 ms          468 ms            2           1075.28M        2.13897/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::robust>         4060 ms         4060 ms            1           9.33886G       0.246288/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::robust>       3.49 ms         3.49 ms          198           8.02285M         286.69/s
bench_sphincs::keygen<24, 63, 7, 16, variant::simple>                238 ms          238 ms            3           547.851M        4.19833/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::simple>         2156 ms         2156 ms            1           4.95823G       0.463906/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::simple>       1.81 ms         1.81 ms          397            4.1576M        553.218/s
bench_sphincs::keygen<24, 66, 22, 16, variant::robust>              7.31 ms         7.31 ms           96           16.8216M        136.738/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::robust>          183 ms          183 ms            4           421.527M        5.45673/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::robust>       10.2 ms         10.2 ms           69           23.4615M        98.0396/s
bench_sphincs::keygen<24, 66, 22, 16, variant::simple>              3.73 ms         3.73 ms          188           8.57885M        268.113/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::simple>         96.3 ms         96.3 ms            7           221.477M        10.3854/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::simple>       5.26 ms         5.26 ms          136           12.1009M        190.078/s
bench_sphincs::keygen<32, 64, 8, 16, variant::robust>                292 ms          292 ms            2           672.216M        3.42189/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::robust>         3348 ms         3348 ms            1           7.70073G       0.298682/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::robust>       5.23 ms         5.23 ms          100           12.0347M        191.113/s
bench_sphincs::keygen<32, 64, 8, 16, variant::simple>                152 ms          152 ms            5           349.201M         6.5864/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::simple>         1813 ms         1813 ms            1           4.16943G       0.551642/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::simple>       2.56 ms         2.56 ms          282           5.89233M        390.337/s
bench_sphincs::keygen<32, 68, 17, 16, variant::robust>              18.2 ms         18.2 ms           38           41.9678M        54.8066/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::robust>          359 ms          359 ms            2           826.454M        2.78299/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::robust>       10.2 ms         10.2 ms           71           23.5242M        97.7736/s
bench_sphincs::keygen<32, 68, 17, 16, variant::simple>              9.49 ms         9.49 ms           74           21.8289M        105.369/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::simple>          191 ms          191 ms            4           438.787M        5.24206/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::simple>       5.13 ms         5.13 ms          139           11.7881M        195.112/s
```
