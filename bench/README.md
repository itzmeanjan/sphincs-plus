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
2022-12-08T11:41:59+04:00
Running ./bench/a.out
Run on (8 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB
  L1 Instruction 32 KiB
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB
Load Average: 1.46, 1.53, 1.69
--------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                                              Time             CPU   Iterations average_cpu_cycles items_per_second
--------------------------------------------------------------------------------------------------------------------------------------------
bench_sphincs::keygen<16, 63, 7, 16, variant::robust>                210 ms          210 ms            3           504.931M        4.75978/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::robust>         1578 ms         1576 ms            1           3.78599G        0.63435/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::robust>       1.51 ms         1.51 ms          416           3.63011M        662.274/s
bench_sphincs::keygen<16, 63, 7, 16, variant::simple>                111 ms          111 ms            6           266.835M        9.00062/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::simple>          846 ms          845 ms            1           2.02988G        1.18341/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::simple>      0.850 ms        0.849 ms          876           2.03983M       1.17732k/s
bench_sphincs::keygen<16, 66, 22, 16, variant::robust>              3.33 ms         3.33 ms          211           7.99807M        300.396/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::robust>         75.8 ms         75.8 ms            9           181.994M        13.2003/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::robust>       4.96 ms         4.84 ms          138           11.9145M        206.514/s
bench_sphincs::keygen<16, 66, 22, 16, variant::simple>              1.80 ms         1.80 ms          393           4.31831M        556.671/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::simple>         40.6 ms         40.6 ms           17            97.456M        24.6474/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::simple>       2.37 ms         2.37 ms          288           5.68473M        422.546/s
bench_sphincs::keygen<24, 63, 7, 16, variant::robust>                312 ms          311 ms            2           747.875M         3.2126/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::robust>         2697 ms         2695 ms            1           6.47293G       0.371115/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::robust>       2.41 ms         2.40 ms          300           5.77255M        416.308/s
bench_sphincs::keygen<24, 63, 7, 16, variant::simple>                165 ms          164 ms            4           394.937M        6.08015/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::simple>         1498 ms         1480 ms            1           3.59627G       0.675678/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::simple>       1.25 ms         1.25 ms          613           2.99999M        800.775/s
bench_sphincs::keygen<24, 66, 22, 16, variant::robust>              4.85 ms         4.85 ms          140           11.6377M         206.35/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::robust>          121 ms          121 ms            6           290.756M        8.26078/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::robust>       6.95 ms         6.95 ms          102           16.6855M        143.928/s
bench_sphincs::keygen<24, 66, 22, 16, variant::simple>              2.69 ms         2.65 ms          266           6.46234M        377.866/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::simple>         66.4 ms         66.3 ms           10           159.457M        15.0779/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::simple>       3.50 ms         3.49 ms          197           8.39536M        286.143/s
bench_sphincs::keygen<32, 64, 8, 16, variant::robust>                202 ms          201 ms            3            483.93M        4.96475/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::robust>         2281 ms         2280 ms            1           5.47549G       0.438681/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::robust>       3.45 ms         3.44 ms          203           8.26846M        290.527/s
bench_sphincs::keygen<32, 64, 8, 16, variant::simple>                113 ms          113 ms            6             272.3M        8.84044/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::simple>         1287 ms         1286 ms            1            3.0882G       0.777884/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::simple>       1.80 ms         1.79 ms          378           4.31596M        557.136/s
bench_sphincs::keygen<32, 68, 17, 16, variant::robust>              12.7 ms         12.7 ms           56           30.4591M        78.8608/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::robust>          246 ms          246 ms            3           591.167M        4.06221/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::robust>       7.00 ms         7.00 ms          100           16.8086M        142.908/s
bench_sphincs::keygen<32, 68, 17, 16, variant::simple>              6.75 ms         6.75 ms          100           16.1965M        148.237/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::simple>          136 ms          136 ms            5           327.065M        7.34905/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::simple>       3.56 ms         3.55 ms          190           8.53629M        281.395/s
```

### On ARM Cortex-A72 [ compiled with GCC ]

```bash
2022-12-08T07:14:45+00:00
Running ./bench/a.out
Run on (16 X 166.66 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 48 KiB (x16)
  L2 Unified 2048 KiB (x4)
Load Average: 0.22, 0.05, 0.02
-------------------------------------------------------------------------------------------------------------------------
Benchmark                                                              Time             CPU   Iterations items_per_second
-------------------------------------------------------------------------------------------------------------------------
bench_sphincs::keygen<16, 63, 7, 16, variant::robust>                690 ms          690 ms            1        1.44943/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::robust>         5161 ms         5161 ms            1       0.193771/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::robust>       5.06 ms         5.06 ms          124        197.787/s
bench_sphincs::keygen<16, 63, 7, 16, variant::simple>                361 ms          361 ms            2        2.77299/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::simple>         2737 ms         2737 ms            1       0.365373/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::simple>       2.64 ms         2.64 ms          255        379.344/s
bench_sphincs::keygen<16, 66, 22, 16, variant::robust>              10.8 ms         10.8 ms           65        92.7138/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::robust>          249 ms          249 ms            3        4.02112/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::robust>       15.5 ms         15.5 ms           46        64.6147/s
bench_sphincs::keygen<16, 66, 22, 16, variant::simple>              5.65 ms         5.64 ms          124        177.149/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::simple>          131 ms          131 ms            5        7.60953/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::simple>       8.28 ms         8.28 ms           90        120.828/s
bench_sphincs::keygen<24, 63, 7, 16, variant::robust>               1006 ms         1006 ms            1        0.99375/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::robust>         8757 ms         8756 ms            1       0.114201/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::robust>       7.83 ms         7.83 ms           85        127.773/s
bench_sphincs::keygen<24, 63, 7, 16, variant::simple>                526 ms          526 ms            1        1.90001/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::simple>         4702 ms         4702 ms            1       0.212686/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::simple>       3.96 ms         3.96 ms          180         252.54/s
bench_sphincs::keygen<24, 66, 22, 16, variant::robust>              15.7 ms         15.7 ms           44        63.5635/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::robust>          395 ms          395 ms            2        2.53044/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::robust>       22.2 ms         22.2 ms           30        45.0619/s
bench_sphincs::keygen<24, 66, 22, 16, variant::simple>              8.23 ms         8.23 ms           85        121.451/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::simple>          212 ms          212 ms            3        4.71934/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::simple>       11.9 ms         11.9 ms           59        83.9878/s
bench_sphincs::keygen<32, 64, 8, 16, variant::robust>                663 ms          663 ms            1        1.50882/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::robust>         7478 ms         7478 ms            1        0.13373/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::robust>       11.1 ms         11.1 ms           62        89.8732/s
bench_sphincs::keygen<32, 64, 8, 16, variant::simple>                347 ms          347 ms            2        2.88308/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::simple>         4098 ms         4098 ms            1       0.244038/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::simple>       5.98 ms         5.98 ms          119        167.177/s
bench_sphincs::keygen<32, 68, 17, 16, variant::robust>              41.4 ms         41.4 ms           17        24.1373/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::robust>          812 ms          812 ms            1        1.23187/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::robust>       22.9 ms         22.9 ms           30        43.6005/s
bench_sphincs::keygen<32, 68, 17, 16, variant::simple>              21.7 ms         21.7 ms           32        46.1087/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::simple>          434 ms          434 ms            2        2.30276/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::simple>       12.1 ms         12.1 ms           59         82.582/s
```

### On ARM Cortex-A72 [ compiled with Clang ]

```bash
2022-12-08T07:19:19+00:00
Running ./bench/a.out
Run on (16 X 166.66 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 48 KiB (x16)
  L2 Unified 2048 KiB (x4)
Load Average: 0.45, 0.42, 0.19
-------------------------------------------------------------------------------------------------------------------------
Benchmark                                                              Time             CPU   Iterations items_per_second
-------------------------------------------------------------------------------------------------------------------------
bench_sphincs::keygen<16, 63, 7, 16, variant::robust>                497 ms          497 ms            2        2.01115/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::robust>         3736 ms         3736 ms            1       0.267672/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::robust>       3.84 ms         3.84 ms          186        260.496/s
bench_sphincs::keygen<16, 63, 7, 16, variant::simple>                264 ms          264 ms            3         3.7854/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::simple>         2007 ms         2007 ms            1        0.49837/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::simple>       2.12 ms         2.12 ms          342        472.809/s
bench_sphincs::keygen<16, 66, 22, 16, variant::robust>              7.78 ms         7.78 ms           90        128.543/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::robust>          180 ms          180 ms            4        5.55778/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::robust>       10.8 ms         10.8 ms           61        92.2158/s
bench_sphincs::keygen<16, 66, 22, 16, variant::simple>              4.14 ms         4.14 ms          169        241.631/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::simple>         96.4 ms         96.4 ms            7        10.3727/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::simple>       5.63 ms         5.63 ms          125        177.624/s
bench_sphincs::keygen<24, 63, 7, 16, variant::robust>                728 ms          728 ms            1        1.37326/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::robust>         6340 ms         6339 ms            1       0.157744/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::robust>       5.53 ms         5.53 ms          132        180.678/s
bench_sphincs::keygen<24, 63, 7, 16, variant::simple>                385 ms          385 ms            2        2.59551/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::simple>         3449 ms         3449 ms            1       0.289948/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::simple>       2.83 ms         2.83 ms          242        352.911/s
bench_sphincs::keygen<24, 66, 22, 16, variant::robust>              11.4 ms         11.4 ms           61        87.8068/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::robust>          287 ms          287 ms            2         3.4806/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::robust>       16.8 ms         16.8 ms           43        59.6386/s
bench_sphincs::keygen<24, 66, 22, 16, variant::simple>              6.03 ms         6.03 ms          116        165.795/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::simple>          155 ms          155 ms            5        6.44817/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::simple>       8.26 ms         8.26 ms           85        121.002/s
bench_sphincs::keygen<32, 64, 8, 16, variant::robust>                480 ms          480 ms            2        2.08456/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::robust>         5444 ms         5444 ms            1        0.18369/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::robust>       8.36 ms         8.36 ms           85        119.586/s
bench_sphincs::keygen<32, 64, 8, 16, variant::simple>                254 ms          254 ms            3        3.94247/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::simple>         3011 ms         3010 ms            1       0.332173/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::simple>       4.29 ms         4.29 ms          166         232.98/s
bench_sphincs::keygen<32, 68, 17, 16, variant::robust>              30.0 ms         30.0 ms           23        33.3402/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::robust>          589 ms          589 ms            1        1.69912/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::robust>       16.6 ms         16.6 ms           43        60.3322/s
bench_sphincs::keygen<32, 68, 17, 16, variant::simple>              15.9 ms         15.9 ms           44        63.0379/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::simple>          318 ms          318 ms            2        3.14367/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::simple>       8.58 ms         8.58 ms           82        116.516/s
```

### On ARM Neoverse-V1 [ compiled with GCC ]

```bash
2022-12-08T07:25:49+00:00
Running ./bench/a.out
Run on (64 X 2100 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x64)
  L1 Instruction 64 KiB (x64)
  L2 Unified 1024 KiB (x64)
  L3 Unified 32768 KiB (x1)
Load Average: 0.17, 0.07, 0.02
-------------------------------------------------------------------------------------------------------------------------
Benchmark                                                              Time             CPU   Iterations items_per_second
-------------------------------------------------------------------------------------------------------------------------
bench_sphincs::keygen<16, 63, 7, 16, variant::robust>                429 ms          429 ms            2        2.33332/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::robust>         3229 ms         3229 ms            1       0.309693/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::robust>       3.50 ms         3.50 ms          219        285.916/s
bench_sphincs::keygen<16, 63, 7, 16, variant::simple>                222 ms          222 ms            3        4.49637/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::simple>         1687 ms         1687 ms            1       0.592607/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::simple>       1.59 ms         1.59 ms          430        627.559/s
bench_sphincs::keygen<16, 66, 22, 16, variant::robust>              6.70 ms         6.70 ms          104        149.321/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::robust>          155 ms          155 ms            5        6.44058/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::robust>       9.90 ms         9.90 ms           74        100.969/s
bench_sphincs::keygen<16, 66, 22, 16, variant::simple>              3.48 ms         3.48 ms          201        287.514/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::simple>         80.1 ms         80.1 ms            9        12.4882/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::simple>       4.66 ms         4.66 ms          142        214.575/s
bench_sphincs::keygen<24, 63, 7, 16, variant::robust>                625 ms          625 ms            1        1.60118/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::robust>         5503 ms         5503 ms            1       0.181731/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::robust>       4.59 ms         4.59 ms          146        217.892/s
bench_sphincs::keygen<24, 63, 7, 16, variant::simple>                323 ms          323 ms            2         3.0962/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::simple>         2911 ms         2911 ms            1       0.343579/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::simple>       2.48 ms         2.48 ms          290        403.089/s
bench_sphincs::keygen<24, 66, 22, 16, variant::robust>              9.76 ms         9.76 ms           72         102.41/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::robust>          248 ms          248 ms            3        4.02659/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::robust>       13.8 ms         13.8 ms           50        72.3611/s
bench_sphincs::keygen<24, 66, 22, 16, variant::simple>              5.05 ms         5.05 ms          139         198.04/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::simple>          132 ms          132 ms            5        7.56995/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::simple>       7.11 ms         7.11 ms           97        140.567/s
bench_sphincs::keygen<32, 64, 8, 16, variant::robust>                408 ms          408 ms            2        2.44808/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::robust>         4615 ms         4615 ms            1       0.216699/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::robust>       6.82 ms         6.82 ms          104        146.581/s
bench_sphincs::keygen<32, 64, 8, 16, variant::simple>                213 ms          213 ms            3        4.68975/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::simple>         2516 ms         2516 ms            1       0.397503/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::simple>       3.59 ms         3.59 ms          197        278.809/s
bench_sphincs::keygen<32, 68, 17, 16, variant::robust>              25.5 ms         25.5 ms           27        39.1657/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::robust>          505 ms          505 ms            1        1.98033/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::robust>       14.2 ms         14.2 ms           49        70.4305/s
bench_sphincs::keygen<32, 68, 17, 16, variant::simple>              13.3 ms         13.3 ms           53        75.0293/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::simple>          268 ms          268 ms            3        3.72861/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::simple>       7.20 ms         7.20 ms           96        138.893/s
```

### On ARM Neoverse-V1 [ compiled with Clang ]

```bash
2022-12-08T07:30:20+00:00
Running ./bench/a.out
Run on (64 X 2100 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x64)
  L1 Instruction 64 KiB (x64)
  L2 Unified 1024 KiB (x64)
  L3 Unified 32768 KiB (x1)
Load Average: 0.25, 0.30, 0.15
-------------------------------------------------------------------------------------------------------------------------
Benchmark                                                              Time             CPU   Iterations items_per_second
-------------------------------------------------------------------------------------------------------------------------
bench_sphincs::keygen<16, 63, 7, 16, variant::robust>                219 ms          219 ms            3        4.57051/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::robust>         1638 ms         1638 ms            1       0.610629/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::robust>       1.63 ms         1.63 ms          401        613.046/s
bench_sphincs::keygen<16, 63, 7, 16, variant::simple>                112 ms          112 ms            6        8.90044/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::simple>          857 ms          857 ms            1        1.16748/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::simple>      0.865 ms        0.865 ms          894       1.15575k/s
bench_sphincs::keygen<16, 66, 22, 16, variant::robust>              3.42 ms         3.42 ms          205        292.156/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::robust>         78.8 ms         78.8 ms            9        12.6943/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::robust>       4.73 ms         4.73 ms          145        211.375/s
bench_sphincs::keygen<16, 66, 22, 16, variant::simple>              1.76 ms         1.76 ms          398        568.252/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::simple>         41.1 ms         41.1 ms           17        24.3162/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::simple>       2.45 ms         2.45 ms          278        407.522/s
bench_sphincs::keygen<24, 63, 7, 16, variant::robust>                325 ms          325 ms            2        3.07811/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::robust>         2818 ms         2818 ms            1       0.354913/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::robust>       2.42 ms         2.42 ms          288        413.237/s
bench_sphincs::keygen<24, 63, 7, 16, variant::simple>                166 ms          166 ms            4        6.00926/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::simple>         1496 ms         1496 ms            1       0.668515/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::simple>       1.19 ms         1.19 ms          570        842.204/s
bench_sphincs::keygen<24, 66, 22, 16, variant::robust>              5.08 ms         5.08 ms          138        196.908/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::robust>          129 ms          129 ms            5        7.77763/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::robust>       7.16 ms         7.16 ms           99        139.722/s
bench_sphincs::keygen<24, 66, 22, 16, variant::simple>              2.60 ms         2.60 ms          269         384.12/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::simple>         67.3 ms         67.3 ms           10        14.8624/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::simple>       3.62 ms         3.62 ms          196        275.885/s
bench_sphincs::keygen<32, 64, 8, 16, variant::robust>                211 ms          211 ms            3        4.73802/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::robust>         2388 ms         2388 ms            1       0.418753/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::robust>       3.62 ms         3.62 ms          193        276.214/s
bench_sphincs::keygen<32, 64, 8, 16, variant::simple>                109 ms          109 ms            6        9.13816/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::simple>         1300 ms         1300 ms            1       0.769031/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::simple>       1.76 ms         1.76 ms          400        568.922/s
bench_sphincs::keygen<32, 68, 17, 16, variant::robust>              13.2 ms         13.2 ms           53        75.7917/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::robust>          260 ms          260 ms            3        3.83998/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::robust>       7.25 ms         7.25 ms           96        137.877/s
bench_sphincs::keygen<32, 68, 17, 16, variant::simple>              6.84 ms         6.84 ms          102        146.158/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::simple>          136 ms          136 ms            5        7.37617/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::simple>       3.49 ms         3.49 ms          191        286.591/s
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz [ compiled with GCC ]

```bash
2022-12-08T07:34:43+00:00
Running ./bench/a.out
Run on (4 X 2300.25 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.15, 0.03, 0.01
--------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                                              Time             CPU   Iterations average_cpu_cycles items_per_second
--------------------------------------------------------------------------------------------------------------------------------------------
bench_sphincs::keygen<16, 63, 7, 16, variant::robust>                612 ms          612 ms            1           1.40778G        1.63399/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::robust>         4639 ms         4639 ms            1           10.6706G       0.215558/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::robust>       4.78 ms         4.78 ms          148            10.996M        209.189/s
bench_sphincs::keygen<16, 63, 7, 16, variant::simple>                321 ms          321 ms            2           738.981M        3.11279/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::simple>         2438 ms         2438 ms            1           5.60734G       0.410208/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::simple>       2.37 ms         2.37 ms          313           5.46067M        421.212/s
bench_sphincs::keygen<16, 66, 22, 16, variant::robust>              9.56 ms         9.56 ms           73           21.9974M        104.559/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::robust>          220 ms          220 ms            3            506.64M        4.53979/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::robust>       14.1 ms         14.1 ms           51            32.384M        71.0268/s
bench_sphincs::keygen<16, 66, 22, 16, variant::simple>              5.03 ms         5.03 ms          139            11.566M        198.878/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::simple>          117 ms          117 ms            6           269.201M        8.54421/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::simple>       7.06 ms         7.06 ms          104           16.2499M        141.546/s
bench_sphincs::keygen<24, 63, 7, 16, variant::robust>                888 ms          888 ms            1           2.04335G        1.12561/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::robust>         7739 ms         7739 ms            1           17.7998G       0.129218/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::robust>       6.90 ms         6.90 ms          101           15.8733M        144.903/s
bench_sphincs::keygen<24, 63, 7, 16, variant::simple>                464 ms          464 ms            2           1067.18M        2.15531/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::simple>         4158 ms         4158 ms            1           9.56426G       0.240486/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::simple>       3.64 ms         3.64 ms          192           8.36833M        274.837/s
bench_sphincs::keygen<24, 66, 22, 16, variant::robust>              13.9 ms         13.9 ms           50           31.9581M        71.9746/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::robust>          385 ms          385 ms            2           886.537M        2.59439/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::robust>       19.7 ms         19.7 ms           36           45.2478M        50.8312/s
bench_sphincs::keygen<24, 66, 22, 16, variant::simple>              7.26 ms         7.26 ms           96           16.6948M        137.781/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::simple>          188 ms          188 ms            4           431.454M        5.33133/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::simple>       10.2 ms         10.2 ms           67           23.3489M        98.5089/s
bench_sphincs::keygen<32, 64, 8, 16, variant::robust>                607 ms          607 ms            1           1.39649G        1.64712/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::robust>         6683 ms         6683 ms            1           15.3708G        0.14964/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::robust>       9.74 ms         9.73 ms           70           22.3923M        102.723/s
bench_sphincs::keygen<32, 64, 8, 16, variant::simple>                305 ms          305 ms            2           700.768M         3.2823/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::simple>         3583 ms         3583 ms            1           8.24042G       0.279125/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::simple>       5.04 ms         5.04 ms          138           11.5869M        198.516/s
bench_sphincs::keygen<32, 68, 17, 16, variant::robust>              37.9 ms         37.9 ms           18           87.2275M         26.371/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::robust>          715 ms          715 ms            1           1.64516G        1.39812/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::robust>       19.8 ms         19.8 ms           35           45.5412M        50.5091/s
bench_sphincs::keygen<32, 68, 17, 16, variant::simple>              19.1 ms         19.1 ms           37           43.8764M        52.4224/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::simple>          380 ms          380 ms            2           873.303M        2.63396/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::simple>       10.0 ms         10.0 ms           70           23.0472M        99.8082/s
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz [ compiled with Clang ]

```bash
2022-12-08T07:39:20+00:00
Running ./bench/a.out
Run on (4 X 2300.25 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.33, 0.37, 0.18
--------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                                              Time             CPU   Iterations average_cpu_cycles items_per_second
--------------------------------------------------------------------------------------------------------------------------------------------
bench_sphincs::keygen<16, 63, 7, 16, variant::robust>                302 ms          302 ms            2           694.299M        3.31264/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::robust>         2261 ms         2261 ms            1           5.20139G       0.442203/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::robust>       2.39 ms         2.39 ms          287           5.49539M        418.556/s
bench_sphincs::keygen<16, 63, 7, 16, variant::simple>                159 ms          159 ms            4           366.439M        6.27696/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::simple>         1211 ms         1211 ms            1           2.78554G       0.825711/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::simple>       1.16 ms         1.16 ms          589           2.67407M        860.143/s
bench_sphincs::keygen<16, 66, 22, 16, variant::robust>              4.71 ms         4.71 ms          149           10.8237M        212.505/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::robust>          110 ms          110 ms            6           252.757M        9.10013/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::robust>       7.16 ms         7.16 ms          103           16.4642M        139.701/s
bench_sphincs::keygen<16, 66, 22, 16, variant::simple>              2.50 ms         2.50 ms          280           5.74299M        400.504/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::simple>         58.2 ms         58.2 ms           12           133.756M        17.1961/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::simple>       3.46 ms         3.46 ms          204           7.95742M        289.042/s
bench_sphincs::keygen<24, 63, 7, 16, variant::robust>                445 ms          445 ms            2           1022.61M        2.24936/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::robust>         3871 ms         3871 ms            1           8.90392G       0.258326/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::robust>       3.41 ms         3.41 ms          211            7.8527M        292.898/s
bench_sphincs::keygen<24, 63, 7, 16, variant::simple>                236 ms          236 ms            3           542.477M        4.23987/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::simple>         2110 ms         2110 ms            1           4.85302G       0.473948/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::simple>       1.72 ms         1.72 ms          398           3.95952M        580.864/s
bench_sphincs::keygen<24, 66, 22, 16, variant::robust>              6.95 ms         6.95 ms          101           15.9896M        143.847/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::robust>          176 ms          176 ms            4           404.608M        5.68521/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::robust>       10.1 ms         10.1 ms           71           23.1378M        99.4071/s
bench_sphincs::keygen<24, 66, 22, 16, variant::simple>              3.69 ms         3.69 ms          190           8.49073M        270.897/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::simple>         96.1 ms         96.1 ms            7            221.07M        10.4038/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::simple>       5.27 ms         5.27 ms          129            12.116M        189.831/s
bench_sphincs::keygen<32, 64, 8, 16, variant::robust>                290 ms          290 ms            2           667.092M        3.44782/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::robust>         3287 ms         3287 ms            1            7.5605G       0.304231/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::robust>       4.92 ms         4.92 ms          149           11.3189M        203.203/s
bench_sphincs::keygen<32, 64, 8, 16, variant::simple>                151 ms          151 ms            5           346.251M        6.64299/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::simple>         1849 ms         1849 ms            1           4.25333G       0.540779/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::simple>       2.46 ms         2.46 ms          288           5.66578M        405.966/s
bench_sphincs::keygen<32, 68, 17, 16, variant::robust>              18.1 ms         18.1 ms           39           41.6993M        55.1559/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::robust>          354 ms          354 ms            2           813.087M        2.82885/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::robust>       10.2 ms         10.2 ms           70            23.463M        98.0264/s
bench_sphincs::keygen<32, 68, 17, 16, variant::simple>              9.47 ms         9.47 ms           74           21.7869M        105.571/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::simple>          190 ms          190 ms            4           437.308M        5.25943/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::simple>       5.06 ms         5.06 ms          100           11.6456M         197.53/s
```
