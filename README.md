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
- `verify`: Given M ( > 0 ) -bytes message and SPHINCS+ signature, it uses SPHINCS+ public key ( of 2n -bytes ) for verifying signature, returning boolean result. Truth value is returned if signature is successfully verified.

Here I'm maintaining SPHINCS+ as zero-dependency, header-only and easy-to-use C++ library, which implements SPHINCS+-SHAKE key generation/ signing/ verification algorithms, for all parameter sets ( encompassing NIST security levels {1, 3, 5} ), as suggested in section 7.2 and table 3 of the specification. 

*Find the SPHINCS+ specification [here](https://sphincs.org/data/sphincs+-r3.1-specification.pdf), which was followed during this work.*

`sha3` is the only dependency of this project, which itself is a zero-dependency, header-only C++ library, implementing SHA3 specification of NIST ( i.e. [FIPS PUB 202](http://dx.doi.org/10.6028/NIST.FIPS.202) ). This is done in order to modularize commonly seen symmetric key dependency in post-quantum cryptographic constructions.

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

- System development utilities such as `make`, `cmake` & `git`

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

For ensuring that SPHINCS+ implementation is functionally correct and compliant with specification, you may issue

>**Note**

> This implementation of SPHINCS+ specification is **tested** to be compatible and conformant with latest version of the specification. That's ensured by generating known answer tests ( KATs ) following https://gist.github.com/itzmeanjan/d483872509b8a1a7c4d6614ec9d43e6c and testing this implementation using those test vectors.

```bash
make

[test] WOTS+ one-time signature
[test] Fixed Input-Length XMSS signature
[test] HyperTree signature
[test] FORS signature
[test] SPHINCS+ signature
bash test_kat.sh
clang++ -std=c++20 -Wall -Wextra -pedantic -O3 -march=native -I ./include -I ./sha3/include -fPIC --shared wrapper/sphincs+-shake.cpp -o wrapper/libsphincs+-shake.so
~/Documents/work/sphincs/wrapper/python ~/Documents/work/sphincs
=========================================================================================== test session starts ============================================================================================
platform darwin -- Python 3.10.8, pytest-7.1.3, pluggy-1.0.0 -- /usr/local/opt/python@3.10/bin/python3.10
cachedir: .pytest_cache
benchmark: 3.4.1 (defaults: timer=time.perf_counter disable_gc=False min_rounds=5 min_time=0.000005 max_time=1.0 calibration_precision=10 warmup=False warmup_iterations=100000)
rootdir: /Users/anjan/Documents/work/sphincs/wrapper/python
plugins: benchmark-3.4.1
collected 12 items

test_sphincs_shake.py::test_sphincs_shake_128s_robust PASSED                                                                                                                                         [  8%]
test_sphincs_shake.py::test_sphincs_shake_128s_simple PASSED                                                                                                                                         [ 16%]
test_sphincs_shake.py::test_sphincs_shake_128f_robust PASSED                                                                                                                                         [ 25%]
test_sphincs_shake.py::test_sphincs_shake_128f_simple PASSED                                                                                                                                         [ 33%]
test_sphincs_shake.py::test_sphincs_shake_192s_robust PASSED                                                                                                                                         [ 41%]
test_sphincs_shake.py::test_sphincs_shake_192s_simple PASSED                                                                                                                                         [ 50%]
test_sphincs_shake.py::test_sphincs_shake_192f_robust PASSED                                                                                                                                         [ 58%]
test_sphincs_shake.py::test_sphincs_shake_192f_simple PASSED                                                                                                                                         [ 66%]
test_sphincs_shake.py::test_sphincs_shake_256s_robust PASSED                                                                                                                                         [ 75%]
test_sphincs_shake.py::test_sphincs_shake_256s_simple PASSED                                                                                                                                         [ 83%]
test_sphincs_shake.py::test_sphincs_shake_256f_robust PASSED                                                                                                                                         [ 91%]
test_sphincs_shake.py::test_sphincs_shake_256f_simple PASSED                                                                                                                                         [100%]

===================================================================================== 12 passed in 1275.34s (0:21:15) ======================================================================================
~/Documents/work/sphincs
```

## Benchmarking

For benchmarking SPHINCS+ implementation for different parameter sets ( as suggested in the specification's section 7.2 and table 3 ), you may issue


```bash
make benchmark
```

>**Warning**

> CPU frequency scaling based on demand **must** be disabled when collecting benchmark results, you may follow [this](https://github.com/google/benchmark/blob/2257fa4/docs/user_guide.md#disabling-cpu-frequency-scaling) guide for doing so.

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
- Finally compile your program, while letting your compiler know where it can find SPHINCS+ and Sha3 headers.

> **Note**

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

> **Note**

> Utility routines live under namespace `sphincs_utils::`, in header [utils.hpp](./include/utils.hpp). You won't probably need to import it seperately, it becomes available with inclusion of [sphincs.hpp](./include/sphincs.hpp).

- Generate new keypair, using system randomness
- Sign random 32 -bytes message, using secret key
- Verify signature using message and public key

for SPHINCS+-SHAKE-128s-robust parameter set. It can be adapted for other parameter sets fairly easily. 

```bash
pushd example; g++ -std=c++20 -Wall -O3 -I ../include -I ../sha3/include sphincs+-shake.cpp; popd

SPHINCS+-SHAKE-128s-robust @ NIST Security Level 1
Secret Key   : b5e851cfdd27f38dcfc35b77ba6154c899cf7731f6dab77cc11c61ac9da6f93852a155e0e91a7f34eb2fbb1d207378ba5e89251f7580d30042460babe857d636
Public Key   : 52a155e0e91a7f34eb2fbb1d207378ba5e89251f7580d30042460babe857d636
Message      : eaaea8b02673a88137c9a4f600c270368da39871b2ff0df3a3cfc096e2ac75a1
Signature    : d913c137da653c0be702608be80388d30f3994b33ee1611546079ad09bb87e0d3aadc441ffda9b65342c520e52ebaca34638f31258dff122edc0e45efd609cdaed0e87acf9b680a98c1c06f7e1333a8e15614f89f888a02f7e863e7f20cce767de302fde8a085c8fde36634326f9dfd45acd4739ac6bc205dfcd823d25459e06f77f2e1f76fd69971d21096a64fb81c39d7d57ea57e242098a404bb2a7bca9ad4e39736bac57d767232be9b789e157e802a7284dfb4e9a580ccad24861f4ba2170b3cdb0bd764d984b952d33c477ec0f851ddd1ee32eb61870c20d432c836907c8aa8ea53fc067b325fabe71ca2ec3076ad2e3d4afaf599b7e279e6e147d5605fafbfa62ea784c980c7ac684ca680797dd1e0cb35522ca2d635132ba4f35ca31053b98614793f8c843768b228f16106a5acbc60a6c41dd2c2550b0a33b8f2e5f5534b30818de5c2e620fce0d5209907e89ee88365a484f7182530357dd5a353c74026878d2851f6e04ef60b133f57dff02c29d58ed0d2adf414a0c1f2f0dd0ddade785e774dda673c8f716878bcbc8181c8719542456a5f5f0482413e852f14eb1c92eb860ddfcd31d2c70c04cd25c269d346097bd3fddbe7eeaa20da735dcadfb57b68ec8097abc10776260af7afa5d84c4fd7ac6405c95ff85e0dde7d3d0f0149d99d15e3d9a7914d54612f44ad4388937c5a4bbca2ab69ede43b342e4e206d413a2ebe4e8303652d63fcddc39d9472a3ffbcfebe9e750d718b9b48eb9f7949849ac1ac1ffc53739aef4d708fde7d568d485cd2facfc0117673c608f4d986e07da55c88658b309a7490bdaa6cc07898e742268ef2503feb204fc92d085b4379575b45d4892afe89d45e1fda1ba2f5989cf723287efc1ba53d40d5cb645b98787fed5db6a992858822d4d6989544b4134c09a8838676d28b1ea5ad897367f6b7355e49654767b1193c88eb878b73c6bc020554b46fb5429b9f11aa6baca077c015e13b725066b661d71de85082fcf8aff269295ca75816be1dd7870772e443a198c0ee9cccf338b75e5dd073fc013e73c4305636018cff5dea247c947ec9f7e71ac20e2c3870c9a69b5751de4b655e00e5dd6a25f7ec7dd164bdf11d934d28eb59c6f280e5b8f5842b6aae91e8d3c56baec7f309cb8f4b0d6b65afd229a4c16f532d8dcf5612d4d66986f4c816968556b4675a47130fa59a20ae42f14d32238781b76d980b36f1a3d7e6eea97d36fd06361ea1811865bfc4161001dea4f3f7752d9c69455e5aab2c6815a0ec580c44d684130755ec2c375548b5a52e29cff8a7c4e02c18c89f84445a968ac6a6450ab8cc7c29ba1439567ab7ceca11b9a78d728bc7afef6cf5ca006617106e478c0fa9dffc12dd0efa293e256cd8d27db30a629d00c52fcddcc3038e29d22fd264a4a4d996e0b2e933b69becf299c7df231ff96d12d1d63f8e86b1abf5e2b089ac5de9b38b845d187cd1c5f5fb636eb4569d2915bf488672e47abb80726f81e32f86b76ce015c11c8dcbf84ad35c9c64c8764cbfc8fbf5c446976191504ad9b665e15485a8bf579ca12666b1905e028e6593da598aca9e3aba2afe211e2fd0690efa87d7cc690cf281b6a239226f22ea3a2702abc79f6274cd16ef6413564afc30bbb2d003c15677ad4fc34744e6897077c34940ff663f09828e3711344b24682ff56ef269255ccbf4f46860b6dc6084372e3d4622a3cd861507c234794c2783ba4176c4715cd366bfc02feb12a152796d88d55019b78b0964c214b00e5df86d84f1b45666eb8adffabfd133b5946337437e151e7c3ba195bd798a5ff5a2233a00f31e606c6c5e5bcd87919778ef482db524427afdaf1bba3653246bb0f32d525e9b8023ccdb8ee557cf72a9f546334c9cc84032d23076e5a6b80789af3b6d182e6d4af2afeae3b7af128aacb48aa1aedce9c7a4d05333ed634b437e3c76d8a2d47f96b048f8f6e2122d9c493263d900c6ed00318f489a277e880e7856684fe6b7141b85a58d7594b541f06cfcfc43fa85165c936cd2a0e60ae67c1bc2737039c5c111c897a2f97539c6563a4f75b1cfae1b267a88816ae235557a6e417671da3800c9c9a7d6cf8c5bd817edb1749c41e091bb10d2b5f4d86be7fe214395ae6a8d61ad40cdadaed1c3fe6d212d82d47387b134f13fce744054db62a9d2239fa73d91ceae09b2b1c4eda15d93fa642f29eb119997b9f4f51ee2be57e2b267baf7b562a029e6497c67174cde91f03d5034dff7e9020ed98dcfe4982323243a29f7497d779cf373be01bfa1a96aefa096fef4be3e286c1dc467d15fd577bf70c8c315e3f5df1cf74c9dcb6ef39b4885a83e192b6377a3aa35ac418f85df16d6d04a4f0c49ac49959bc005de3d68ce4d545bb1a52a43355a01fe98b6e548cf0aef91be3cc8e0792629e3049b8e4613cb601bbbcc2fefdae230cc3459fd5947aae5d1a579e6e8c62d8a593ecd8aa4710da8aa14fc31ef081e140bc4c8dde0315cb8b7362f315dd70389f8ed9f37cf7609f73068cb378c6c1e65ab2e6b3a0dc4fa302f1d9adfe184e5d0ae5c2f2ff04321602443e5ea3152d2f2124831f2f5b5ea7eac481bb853a10d86316f375604a0ea8d79732fff9105d600decc539d54fa7fb5d0e8066d3558b288c58c36d2e3df047060e073c27a795c27197db9f5c7d453586dc7faa9d17de8155d5ec12f97297ee9bcf4d2433bfd081e586441d15134990e58f3abe33d376a8d63ba66bb8d4f6e1287be05e9110f8a1667448e79ee7cf1d461b4ded3b737e1ecf28711404b7771c2b4619832948908e04b2c71f386929a3ce9cea24e8fd379d4a3bef8c99f8bc882361cdfe24686894c1a57e569f64621d3a3450aa9c16ef7ea05ddc439d0e64a6cdfd2af36a2f57bfe12ddaa6ee87be4ded1900481a240f53b6524491fcef1a24dd9e9440f8bc45d820d0389881a07317fb92dd26c89997b908ffc3e0f6f57088b0d331ec7e93145e604e4cacebc0c637a8e065da1ef944990af4f1366a8e6bcaba333bd882af2580d372ba2ad395638c7e644045543e5a309fbe9bbd0de1e02cc06901e583ec2e9ac26f97db2a121b635e39d757f7e6f3b31c7f780a0b917049fdc825c4b2ae8f4925c12cb53153db881d42ab0c285df51533e6f09e6db8511891ae3c83b9c95a078d3b6c96452a4a6c348d66999f80ec177e705049180b25cc6ee12110e101787989a4c74522cd376cb6070b3f760724314822114b5fd31b8ed24a14c81631e142792a0a0d243cbeb0bb7aa1f402dc73f455f640f3a47c5b79cc30728cd44fd8163cf54aa32938ac5155bd157877e580723bb2c99997cc307a936d77fd1cb8a11dc69061b500da2f0a9cdd3d1cc05d848b323da20a569e1a92532f082b75a980a058ffef56f9fe7cf6637081e64f3f85f025183240b3f6194dfcff5461f6a60a93e8469d7afe921f0998528c44035e669a2ae3d4492297124789aa3400adc4ca08ab0fbd562ae009e873c164b0a04e5196d1ab574ca1a278dc038123da56428ce0ec32e58f5a08b90a9680f805ee36e8759fa35642f5f87ba3e08ed78e4ca506ca8412858d7d1f41eb3b05b6afde877a0904ab0168ff379c02587726485dfced4352a74936cff728e9fb950d07d97185bec5f014ac5ff7bc7dfd4a666b912b012534959de0ce9dc91e83fa47ff4f6c565040ddafc2640ea86a82ed9ca3fd110635e949512fa4ebf3af5e9ae3167cc5aea55468d58c131672d185e77f660e63a78b7c740cccc6ea00a22974b2ee03ab69909cda1b76082c56fc59a5dcdad092901ca8dca6c3771636e48cbd06752c6ee6818c42e603e2585b47926001c79b3a8da4d3d8bf9b5ff15d7d121b924ca10fac7ef4e029fc1e4482151b5153e628b3f7d94081a3a7d47f950c92799164920ebb9012574ebf5b20772579c68bd0ed093c10d1f0f7f3eec6c044faf175f2e5f5b6a0ccaaf4c1e34064231a23882f3cc94ec506345bb3c27545590f0dd46578272cca2d9afe5458b43ba138010b3a1d0dfe8142ca440956677992dcc8e59eadce08998c7b14bbbdfb14d2b84a612d22256f1eeedeeaaf4b5aaf0ecb73249883b283c8f6e2c8f44cb88de1e5566fedbdfdd9d6fb5ac67c8ebcabd65432d0becf7fb7ea60ab9f93f31b316f6963479f74e57fd6dda4f986d327d129bb16e611cc2c76f6cfd25dedc5b20b9c36750cc50c37fdd72d7b62f5f3b9b2dcdb49b4dada8b0ead77384fa2c50b41ea80886ec44674e44275e2a88fd18107dd668118a544a3c145fecfbdf6dc14678402146cb936e78a5458b3e5214c2c481b7e8ac9c2b7f80f61a3d8065bf6f9c6d369c7a33214c411cd7daad7b7954dfafb606dc6b9d80d821e74122489e15da2329bb2940d1b099f75a600b361c683fc5f1b7768fbbec82bdcfc9792ae2d2bed900fbf9a5a47c20db4c222a29fe9d2d0435b29640ab98818fb09be4a375f8927cc10985e7626fe8e29176734a0110a75f417e4568100a201c0241ee25402c5004f21f12103b0295af64b13301ed756acc4954373744b3fb657c91ad99e8054e9dba40488cbc9d128ac8a0a3826b7e8782edc8101c5e24c8ffb83207bcca3a7e17a3675388faa07965033e08f029ffff5416fc6fa44666878b9c94dd8c95900dcf44ac640405cfdd8afb623ab87bdd77b4c08f5d81b6e08268a82887f1737c7221ff87c5247776f21e5e2252b47d4ed732ece6f0fd96e5314363b125daea66dc430a50956d7380eb7aad3aae6eea7a4642a63fbde472f74f70dd2a38b50b44030723d41d3a097718a3a11c1900393a3b775e81a10024cd3dd45d840a22db6ff71cd78cda138b05444fa8e77193cc5c6a806ab195f6bd41aede22bdb4772e439af6081522ea05e4d79291f5a6ee301ee4da3d02b3d8d863ee3c449ef928ed53c149700b51f2d4074bd0baedb92d2dc58a2d3f1d616532e5467a74e52f1d0f0b8d7eb5cd7aa2023e14ceabb735a5a48eef29e4a1de81c7a9a8e2fedf756002d59d8abb309fdd3a21249098a6adaa218a52c59c87401c65816e307e1894c30bca0de5add482c7bc8cf5a25bd1665e1026b40192e813f0a49cf8298f0852efa0d290650999c3affa76ae8b16396fc53659f7bd8430f0d98317f03d984f871bbacc17905026223abe28032c32be8c2437575dfcafd22d06a625a3d3220a52204620e053665112141d67cd5a9c0701dbceb315cb6e0978fabbdd0821f30540fed0f201980d8920eae0bce1c46465cbf98a4d8b2583330773c3d00ba72f3589efb8eeedee69cc95d32f3fbf360d66dc77ed9b2900f0fb83c5bd69c3dc2c8c57f2a54d99869afef3eacdacfe1adaa1bcbc0cb12d64bfda011bf1e142fac0625a97159fd1ff2cddedbabb57d77033f6a5134c20651a1a8befb30afed77327cb455f0666f6796ccf0d23fa85e579d4d1683cbb60d5cb41a17efb9dc11947cd06ebbe3314d9c3bfabbc915f6f8a54cfeba7fb92caba0e6c2085f1750116e9a3d49bae699ba5af36a55f63e015155d61d3bc893c0dae8879035daba3fb706b93382ce104278ae3d68eaad6c29b17fe2ef7a2e24f0d2483827e4aa1416ca3d0c8c54b329e6e30ca319711ca73c5dfd6e9b0c5b6a03b022543dab3b40bb4a1a23681d161c573cb349feac80130beee40432c1b6b3ed8076ced3cb93ab21f942b4d5f64a778395efc5c0bf07bcb45997358a17aa9fd8965c4e5282376983e5ae5c05913b1e4d87bf8fdbed560d1f2dcb9e4761268f42a47d7b72f51d7d0f74aedd1f04cfe217d8d1d5a516667986eee8cd46d5016b3f6a720327524db4340c9f0993a71e5e047aa7f94ea115d972cb80792050bba898e549e9d0593a18e9f14c0c7bcbf52b3517dff4b0d5a6a0a53fbd40916a0bf41c704ceff339ccf7bace0845d3ab8ca25e0154958355b8a3785898b18055f40650eae07df87e1d45943a944e23030eef10a467fcc04d3f24e2907449e59b969ba0b06b574bd8a2acdb26f83e80ae0fbf806b074e376e1c8c6c1308b85d6fc5cee92541f87cd908ac87d82f413341dcbe268036c3dddbced97faafaa3773a4d0bf2ae6c955971b845fdaac655c01eb65fa1fae32edca3494eacb8de92d5f71272e6fe0ed3ca5794590034b6faa8cd9ba44b7b8be581b1bbd68f44acc30715c5b69aa84227d538a6934c4059580c3587993957b94df335479c85e6a8eac328ada652171be65a6d0188adac80b2b1f53a19cbdb1f10c8c6b903587c29955c06c51253f78dfd50d83774a9c4dcd78f8a4d599eb221df358263028d1270944c084434c8d1923fa5f4a0a1edf59a38ff298b01961fe981b5270e3e24234e816f6610aec829b1996bb9635069b495e77d8bcc8f72afb5be5debc8c4c818214c0190483d7c2296699acf9238ed2ae353a35307d3a609702e5c666dd92c4bafaaba44082209b41b6baed7ad194841cec368e0940c3b4be0366ea49bd965a9217f8ebf30c8178bb3c04de69b8b19cf6e31a77364869991465ad1a0bb76d3ee48dceb1c950a3520ce7b75360e9e224cc63c4cd50f35c53cda085b3e64bd65409a7295fca5b757b03504ecf8f0742e831438303c31e138d3b16987937bdc54031c6123a5b1ebe74a2cd6af7e076fbb243f051d18e252fead8afa8bed80aea319023fd1879754d98780a5a25a7fced59438f788bfa66dbc04bf95f16bb45828f56e83c39a485caa32b8a9b0a8d15b9098ce251edb4ce5ec7a357c47ea98aa67637640d6fedd6273172e746dda9e9c9d089f77baebd578a87c785a2d2aafd18e4e36337f3aa7040b36d8022baaf10eb4633f24356ef045600adb8c213879ddde562c9b3cf7c7821f1bad3eb8f1a65a40e080dfa2335bdc4cba87aaa7a2789a5dd40e3a45724d83c9ed497ee465db2eb779e29aa1bcbfe4b8cd4a42050813fcb6075873556c919cf3a93a004ca3e9cd95dc864525f50eb749ffa408b959bd74d5514807d2458067a110e00c84f82fd2e1054be6d3f2d2a1201be06c8ea93dd0167e3700d7e55ec30f788a539e64f2a487e09d47c662ce9bda287d758b18020e5d1ea93f4667f2f6e177404c656d9f4b36998306a7cd6c9aaa446624024e2f615b32f0be31bef0068c70a65e3514fb3242aaf69731d46c69a9e1d8c1e60d82cb7c3d6c08ee10b976acbff0a551682935f2b5047fcaaf17036bf4ad13fe6f0a65bb0e6e8ef2e5a13369edf98ae917828e296dcb439d6736a8b1a7002165d89d5d9cba524289b274f52c8f591bfd201fb0c0ba42030171d92238ff6b8e18a65f2375606d3be644bf47a7548134d907dd0c9a9e5a535ad83d7de88041fdb90f305f588e666194a135691a722b88ecc9db393929e416a655053627807e740418ff5667f3143fb61f189adc868b4342b200541a453748727bdafc7210d5bb57b6e05e56141990fa2b6de004e4a3616092a4ef4a9ca4a9589d8b386bea218243140ff66ef553fe408a288d0ff5cda8d1de4c125f821fb4e8c6512cd8b7322f9f66e68fa7863f0bb49747fe12d0ea8ff472b36a15f48a2789bf9973670308871892c92976f6b46b6d927cfd0f69d6540ebad0c5d13f0667ddeb69d68e429b1344a9fcc88bc568ad3ba6ec848a190343900132613e5eb09855df6abb7cd6ea1920f259a662198fffed7a4b5bc76b8ae7a12151e42ccf8c088da8ab079ffa015c427ac1032ffbcc411d4aa504e71ef9c506f93fbe86874b882e4ea3fe29cde0b62bddabb14dcbf2f42d881329c909014ff6d39abad1c8022cb108fe19c42ab396b26b20286b7bd4938704b88caae4cfdd4b8ecc6712fc0dbfeb21f1a2cfe2c0fe7b0a92c0bcd9102231122892f154e347ba747ec64a1fde5c9595144b7eb720459531b7993868426614bf8b0b222aa70e04153ee4f4371d98c93bc141e0f15a4de42c35e8f79cc7cc5dc90e5a0f2527cb56f6b8652d1f08e9004faf13e03ae7cc0ac8d20466998a71d7f685f88904d5a578b88aeb6c092b65c9ba431d2418f3ad9549eb45672be395879f23a21543e1f3c61ad8bd10046c999263e008b6a651bf04f551362578522001ea07bd359fb8c4a00dd01bca3c1f67f79f88d904315b9959e80892879c28b58d0b622ad755d2ebdcbeb26b55ae4035c81ed0de7100ae6f2574a6afcaa4ddf9952a592d3b00bf6fb194ca9a0ac9667bfaf4cdd5d1bf3c27f7914e925c3d948b79eadea8e381fc0574788168aae5b7279c9c187d94f3b6209aba1205064f81edc7f9a3aa6d1c09294e975b9d45818f9add7fb2de8fdaf36fd9856b340be35c033f57ea984e703ba2d64d963179bb823cf2c987c7b7da7d5c43a0c7e598c5d839309f6df6bd2be3c8c20b8bea70f7515a5867dbab1723f2b4df2d0d98c493799df993511acada140c879fce7b52e9a7453db97da09797b072b25ded563aa786e32ead75dbd84b8923ded4330b481894cd8fb00af8725052f6a61587aa0de1602fc8781fbdf042285b5799484b4ebf144a3c0cd7c91efec0053c47744b4f5a659c17f6ef87b13170c7ec025ac99b3df5c5e2d9ce24befc671120fe60e282b471fce3a9a7285b4f1419e9347324ef2e9668158b51c7d35b36555e752afcaf33345100a43c485a27871a4c3985f52013388a504538c64952f43019d7ee8db7379fcaef5a5cb55f002327e0ba5d6d287285a60c84f57f065701d7ba58d0dfd740124ad0d5b61155132b47735e216a6764ebfb495c01c039331698ddd2c06673fb316b17cfabc5cb3657509b8d2495a6f17b4a46e813ace9c5cece8356fd9a9bb23cad0b4a62f70c1c49e743a6d914d5d44fe2cd8b86b541340d4914e6d51d97c7963114fc8f9d96584b94dd78303cda83086cad1b09a1f17d8a932c04a1a64a279ee87a01f8926ff32e70cc50ae775fed63d86ca14556395dfc320618b49d2a007f62e12f3d3ce67bd5c02afe80d9db2c306a7f05bfb655cf7dba203b0d29c7437ab198a65f0b36a7248c85f238abaf7cd1e5a23e708bd8c49b9bcc5ca02543f185d7f1b03fc0374ccb9956ef9bb15cdfeacee1e6c97f46c6b2226e4d7fa49cf6a7f69bc0ddb6698e8485ffedd6d268334648498d4b23ae7770d87ee3508b4b13bd304eb6063159493cd0ac380aecd4166d1dad8678587220f1c486ebff45357843c240dc299255c230015296aa1fb1fbdf51f09130a114b61d94c135f5de216b748f390b48952faf0faed51b76b1c7f985d7f02907d286c628efa3c14d86fa733bea13b8ef1c0a6e79d8749bf5f6b1fe79380ae278c56ccaef78e6a3ea5fa6a3bd644dc1a0205db456a685d4e05867f7a6115da1ba8d89b935952ca7377cbc635bc41236c4c99569d49019c76281e725500cd722010c48c917bb8c3b1a48a9a1065115985b5618b70ab863a7f311246c01d6640a25bc348d83eb1c305768249f00b7fc94163f44bc27ce74aed912c8aa69af494b94ad4e1ce4461c6fc95303d15dca537cfc5297fb6c279f6c9243fc4f9a1c406c5776554a5d706c7b888e7d3c4dc39dd00600de5de59a8b7e1b4ad4a75d80d6a4299474025a5d7b956f45b793b2e6ad2e3295787e89ca9d953e07bb9f818ab2b9d091302e0b22c8fa1b1e58ce9b412d155977eedb73c6c5808f2bcd3989f695c5bc9f82a618826a633cb56281ebe0115155250da9069648803303d6a89d0601a38f3146b3166b18ff1fdbe492df958cb9a4110b491b27ff80ce5322e9ec592cc89a36e59d7846e0231514961b55daa7f9237f8b57b4a7bdb66a531f3ca23824a5cfbcd047f7eb8e7ff18f5c2d39291709a8b5c031341b939ab369ced23e83e5c925f8c5f3c89f48df8eaedb44eb136266b9cebc97f1223313ccdd684f93ab79696464d6b694a0cb4baeedca0e9fc1ba67037fa078fee599647fae4ff47a4ec6870314cc1110aba58c9f43d030f6da13d842748faa4bfb4cbadea2563d7cae7d2486e57a87d92a57781d708fcbe1b9a13c47d9e90dd0ba8793225435aef7a20db4535edf8c458dbdbfec78975b10380f830ec18c81bfd403576ebf7ccccf281c3d39db2f5869c526e64d391139f24495c6a3168b9dab945eb8f30020bf268829be17956d9e7356f7e3889bcd7bcbdd00c38ae3cc7d36168c67aa407691a1e8adc7043a9129aacc9657f09d7b3a3e83586298d7cf6d1e1e93af7197c0f33ac7c7301572c3415bfe9adb8847630c3c025448351e137361308bc3be47ac22ae4b5101cb066f51eacf701bdaae9c19514f5debefb9e10bd384341818be97abb734aed61ef8ba4978b48ec878d6b425d8732e648356751e12642d47edb6b60c276ef83c198dc3ad2f38db8680f61ca6b2ea50520b3ec82b0f85d9f08d45e51575b66ae41baa0c4efbd2dceb3e732918e3b20cac3c2c5a1099a657685c3d6bb8ea1f65a1d3ad87ba914ad3c4c277f3b5ba9f298a53bbdffbc3f3fbb161a563110b27e2ecb0931e5a40aaaedabefa09329740d3b2caeac8fb6ec043a1764b49761c4f3b50711dd1fce3aa0f54c0b56cea63edcf0bf1febe2426e693db22645c5002b3a0976b964309adfbb78a645e0e764602b904536159194a78b5f31b17cbcf2b9cbc406a16f06467135fceb1796c057d8fb0d88d3ddaa26a2c2484307108e36e38c1d3586b7b8ce9c6d5e0c35fbd4713aabdb7dbdfb5516f10131872d8a171e656daad490c7fb58d44d433c85fc33d55f9dd91e566538768b33ee162dfdf1104aaa4099e482182cb3dc9532211dee983466e98d68ba79ded8e88a9131dcffe19e8a9ea3569366c98d0e4fe57240b3dabfd144ae46920504d6829f7d1ae258805c28731765458f349f05afd227f801453b21d1df5198bba992460e2116f10c2940cadcd16fffe1a9346fcbdbe55a96acf72ee422c4f3dcf70eaf71fa2abdab2a3c82d6ca376d4b1230dc81d698f1feadb386d76fa84f416cc563cd64e41ae6805b19a9560124b8b2e497a128575a4963c5b28c64e020907ddc46894827ec32761fd6f5d370b486b6390179c1d6ef5c6c54fb2f9695f79d37a08a4a255aae699c555044568a31258b87ca6f33f1eb3c52d5dd8271513a51273232fdc6fe42df0e5cac015504ff21e95e5db5986c4d5e8fff2ac79ee9c0c9b6f22b04023c8
Verified      : true
```
