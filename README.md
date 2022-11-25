# sphincs
SPHINCS+: Stateless Hash-based Digital Signature Algorithm

## Introduction

SPHINCS+ is one of those post-quantum digital signature algorithms ( DSA ), which is selected by NIST for standardization. Quoting directly from SPHINCS+ specification

> SPHINCS+ is probably the most conservative design of a post-quantum signature scheme, on the other hand, it is rather inefficient in terms of signature size and speed.

SPHINCS+ builds on SPHINCS with various improvements. I suggest you read section 1.1 of the specification. 

SPHINCS+ DSA offers following three APIs

- `keygen`: Computes random keypair where secret key is 4n -bytes and public key is 2n -bytes | n = security parameter ( in bytes )

>**Note** 

> SPHINCS+ secret key holds a copy of public key, which is used when signing messages.

- `sign`: Given M ( > 0 ) -bytes message, SPHINCS+ secret key ( of 4n -bytes ) is used for signing message, by default deterministically. Though one might specifically ask for randomized signing, which will produce random signatures for same message. 
- `verify`: Given M ( > 0 ) -bytes message and SPHINCS+ signature, it uses SPHINCS+ public key ( of 2n -bytes ) for verifying signature, returning boolean result. Truth value is returned in successful verification of signature.

Here I'm maintaining SPHINCS+ as zero-dependency, header-only and easy-to-use C++ library, which implements SPHINCS+-SHAKE key generation/ signing/ verification algorithms, for all parameter sets ( encompassing NIST security levels {1, 3, 5} ), as suggested in section 7.2 and table 3 of the specification. 

*Find the SPHINCS+ specification [here](https://sphincs.org/data/sphincs+-r3.1-specification.pdf), which was followed during this work.*

`sha3` is the only dependency of this project, which itself is a zero-dependency, header-only C++ library, implementing SHA3 specification of NIST ( i.e. FIPS PUB 202 ). This is done in order to modularize commonly seen symmetric key dependency in post-quantum cryptographic constructions.

> `sha3` is pinned to specific commit, using git submodule. See [usage](#usage) below in order to understand how to use SPHINCS+ in your project.

> Follow progress of NIST PQC standardization effort [here](https://csrc.nist.gov/projects/post-quantum-cryptography).

## Prerequisites

- A C++ compiler with C++20 standard library support such as `g++` or `clang++`

```bash
$ clang++ --version
Ubuntu clang version 14.0.0-1ubuntu1
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/bin

$ g++ --version
g++ (Ubuntu 11.2.0-19ubuntu1) 11.2.0
```

- System development utilities

```bash
$ make --version
GNU Make 4.3

$ cmake --version
cmake version 3.22.1

$ git --version
git version 2.34.1
```

- For benchmark you'll need to have `google-benchmark` installed. I found [this](https://github.com/google/benchmark/tree/2257fa4#installation) installation guide helpful.
- `sha3`: a zero-dependency, header-only C++ library, is the only dependency of this project, which is pinned to specific git commit, using git submodule. Import Sha3 after cloning SPHINCS+ repository.

```bash
git clone https://github.com/itzmeanjan/sphincs.git
cd sphincs
git submodule update --init

# now you can {test, benchmark, use} SPHINCS+
```

## Testing

For ensuring that SPHINCS+ implementation is functionally correct, you may issue

>**Note**

> This implementation of SPHINCS+ specification is not yet **tested** to be compatible and conformant with latest version of specification. Ensuring that using known answer tests (KATs) is work-in-progress.

```bash
make

[test] WOTS+ one-time signature
[test] Fixed Input-Length XMSS signature
[test] HyperTree signature
[test] FORS signature
[test] SPHINCS+ signature
```

## Benchmarking

For benchmarking SPHINCS+ implementation for different parameter sets ( as suggested in the specification's section 7.2 and table 3 ), you may issue


```bash
make benchmark
```

>**Warning**

> CPU frequency scaling based on demand must be disabled when collecting benchmark results, you may follow [this](https://github.com/google/benchmark/blob/2257fa4/docs/user_guide.md#disabling-cpu-frequency-scaling) guide for doing so.

### On Intel(R) Core(TM) i5-8279U CPU @ 2.40GHz [ compiled with Clang ]

```bash
2022-11-25T09:27:06+04:00
Running ./bench/a.out
Run on (8 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB
  L1 Instruction 32 KiB
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB
Load Average: 2.13, 2.30, 2.18
------------------------------------------------------------------------------------------------------------------------
Benchmark                                                              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------------------------------------------
bench_sphincs::keygen<16, 63, 7, 16, variant::robust>                222 ms          222 ms            3 items_per_second=4.50176/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::robust>         1677 ms         1672 ms            1 items_per_second=0.598001/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::robust>       1.64 ms         1.64 ms          407 items_per_second=609.5/s
bench_sphincs::keygen<16, 63, 7, 16, variant::simple>                115 ms          115 ms            6 items_per_second=8.68595/s
bench_sphincs::sign<16, 63, 7, 12, 14, 16, variant::simple>          867 ms          866 ms            1 items_per_second=1.1549/s
bench_sphincs::verify<16, 63, 7, 12, 14, 16, variant::simple>      0.931 ms        0.901 ms          832 items_per_second=1.1096k/s
bench_sphincs::keygen<16, 66, 22, 16, variant::robust>              3.51 ms         3.51 ms          200 items_per_second=284.961/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::robust>         79.9 ms         79.9 ms            9 items_per_second=12.5232/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::robust>       5.02 ms         5.02 ms          142 items_per_second=199.182/s
bench_sphincs::keygen<16, 66, 22, 16, variant::simple>              1.84 ms         1.84 ms          379 items_per_second=544.778/s
bench_sphincs::sign<16, 66, 22, 6, 33, 16, variant::simple>         42.1 ms         42.0 ms           17 items_per_second=23.8371/s
bench_sphincs::verify<16, 66, 22, 6, 33, 16, variant::simple>       2.51 ms         2.50 ms          281 items_per_second=399.405/s
bench_sphincs::keygen<24, 63, 7, 16, variant::robust>                325 ms          325 ms            2 items_per_second=3.08016/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::robust>         2946 ms         2931 ms            1 items_per_second=0.341142/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::robust>       2.62 ms         2.62 ms          268 items_per_second=381.546/s
bench_sphincs::keygen<24, 63, 7, 16, variant::simple>                167 ms          167 ms            4 items_per_second=5.97663/s
bench_sphincs::sign<24, 63, 7, 14, 17, 16, variant::simple>         1511 ms         1510 ms            1 items_per_second=0.662078/s
bench_sphincs::verify<24, 63, 7, 14, 17, 16, variant::simple>       1.20 ms         1.20 ms          556 items_per_second=834.249/s
bench_sphincs::keygen<24, 66, 22, 16, variant::robust>              5.07 ms         5.07 ms          135 items_per_second=197.35/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::robust>          131 ms          131 ms            5 items_per_second=7.66252/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::robust>       7.25 ms         7.25 ms           94 items_per_second=137.998/s
bench_sphincs::keygen<24, 66, 22, 16, variant::simple>              2.65 ms         2.65 ms          262 items_per_second=377.906/s
bench_sphincs::sign<24, 66, 22, 8, 33, 16, variant::simple>         67.8 ms         67.8 ms           10 items_per_second=14.7528/s
bench_sphincs::verify<24, 66, 22, 8, 33, 16, variant::simple>       3.64 ms         3.63 ms          198 items_per_second=275.118/s
bench_sphincs::keygen<32, 64, 8, 16, variant::robust>                212 ms          212 ms            3 items_per_second=4.72376/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::robust>         2613 ms         2597 ms            1 items_per_second=0.38507/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::robust>       3.71 ms         3.71 ms          194 items_per_second=269.51/s
bench_sphincs::keygen<32, 64, 8, 16, variant::simple>                110 ms          110 ms            6 items_per_second=9.0641/s
bench_sphincs::sign<32, 64, 8, 14, 22, 16, variant::simple>         1312 ms         1312 ms            1 items_per_second=0.762404/s
bench_sphincs::verify<32, 64, 8, 14, 22, 16, variant::simple>       1.85 ms         1.85 ms          373 items_per_second=541.151/s
bench_sphincs::keygen<32, 68, 17, 16, variant::robust>              13.3 ms         13.3 ms           52 items_per_second=75.0826/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::robust>          271 ms          270 ms            3 items_per_second=3.70055/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::robust>       7.52 ms         7.52 ms           89 items_per_second=133.047/s
bench_sphincs::keygen<32, 68, 17, 16, variant::simple>              7.06 ms         7.05 ms           99 items_per_second=141.776/s
bench_sphincs::sign<32, 68, 17, 9, 35, 16, variant::simple>          139 ms          139 ms            5 items_per_second=7.19876/s
bench_sphincs::verify<32, 68, 17, 9, 35, 16, variant::simple>       3.57 ms         3.57 ms          190 items_per_second=279.987/s
```
