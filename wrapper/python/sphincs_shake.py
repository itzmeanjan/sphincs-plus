#!/usr/bin/python3

"""
  Before using `sphincs_shake` library module, make sure you've run
  `make lib` and generated shared library object, which is loaded
  here; then function calls are forwarded to respective C++ implementation.

  Author: Anjan Roy <hello@itzmeanjan.in>
  
  Project: https://github.com/itzmeanjan/sphincs.git
"""

from ctypes import c_size_t, CDLL, c_char_p, create_string_buffer, c_bool
from posixpath import exists, abspath
from typing import Tuple

SO_PATH: str = abspath("../libsphincs+-shake.so")
assert exists(SO_PATH), "Use `make lib` to generate shared library object !"

SO_LIB: CDLL = CDLL(SO_PATH)


def sphincs_shake_128s_robust_keygen(
    sk_seed: bytes, sk_prf: bytes, pk_seed: bytes
) -> Tuple[bytes, bytes]:
    """
    Given 16 -bytes secret key seed, 16 -bytes secret key PRF and 16 -bytes
    public key seed, this routine compute 64 -bytes secret key and 32 -bytes
    public key, using SPHINCS+-SHAKE-128s-robust keygen algorithm
    """
    assert (
        len(sk_seed) == 16
    ), "[SPHINCS+-SHAKE-128s-robust] Expected secret key seed to be 16 -bytes"
    assert (
        len(sk_prf) == 16
    ), "[SPHINCS+-SHAKE-128s-robust] Expected secret key PRF to be 16 -bytes"
    assert (
        len(pk_seed) == 16
    ), "[SPHINCS+-SHAKE-128s-robust] Expected public key seed to be 16 -bytes"

    skey = create_string_buffer(64)
    pkey = create_string_buffer(32)

    args = [c_char_p, c_char_p, c_char_p, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_128s_robust_keygen.argtypes = args

    SO_LIB.sphincs_shake_128s_robust_keygen(sk_seed, sk_prf, pk_seed, skey, pkey)

    return skey.raw, pkey.raw


def sphincs_shake_128s_simple_keygen(
    sk_seed: bytes, sk_prf: bytes, pk_seed: bytes
) -> Tuple[bytes, bytes]:
    """
    Given 16 -bytes secret key seed, 16 -bytes secret key PRF and 16 -bytes
    public key seed, this routine compute 64 -bytes secret key and 32 -bytes
    public key, using SPHINCS+-SHAKE-128s-simple keygen algorithm
    """
    assert (
        len(sk_seed) == 16
    ), "[SPHINCS+-SHAKE-128s-simple] Expected secret key seed to be 16 -bytes"
    assert (
        len(sk_prf) == 16
    ), "[SPHINCS+-SHAKE-128s-simple] Expected secret key PRF to be 16 -bytes"
    assert (
        len(pk_seed) == 16
    ), "[SPHINCS+-SHAKE-128s-simple] Expected public key seed to be 16 -bytes"

    skey = create_string_buffer(64)
    pkey = create_string_buffer(32)

    args = [c_char_p, c_char_p, c_char_p, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_128s_simple_keygen.argtypes = args

    SO_LIB.sphincs_shake_128s_simple_keygen(sk_seed, sk_prf, pk_seed, skey, pkey)

    return skey.raw, pkey.raw


def sphincs_shake_128f_robust_keygen(
    sk_seed: bytes, sk_prf: bytes, pk_seed: bytes
) -> Tuple[bytes, bytes]:
    """
    Given 16 -bytes secret key seed, 16 -bytes secret key PRF and 16 -bytes
    public key seed, this routine compute 64 -bytes secret key and 32 -bytes
    public key, using SPHINCS+-SHAKE-128f-robust keygen algorithm
    """
    assert (
        len(sk_seed) == 16
    ), "[SPHINCS+-SHAKE-128f-robust] Expected secret key seed to be 16 -bytes"
    assert (
        len(sk_prf) == 16
    ), "[SPHINCS+-SHAKE-128f-robust] Expected secret key PRF to be 16 -bytes"
    assert (
        len(pk_seed) == 16
    ), "[SPHINCS+-SHAKE-128f-robust] Expected public key seed to be 16 -bytes"

    skey = create_string_buffer(64)
    pkey = create_string_buffer(32)

    args = [c_char_p, c_char_p, c_char_p, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_128f_robust_keygen.argtypes = args

    SO_LIB.sphincs_shake_128f_robust_keygen(sk_seed, sk_prf, pk_seed, skey, pkey)

    return skey.raw, pkey.raw


def sphincs_shake_128f_simple_keygen(
    sk_seed: bytes, sk_prf: bytes, pk_seed: bytes
) -> Tuple[bytes, bytes]:
    """
    Given 16 -bytes secret key seed, 16 -bytes secret key PRF and 16 -bytes
    public key seed, this routine compute 64 -bytes secret key and 32 -bytes
    public key, using SPHINCS+-SHAKE-128f-simple keygen algorithm
    """
    assert (
        len(sk_seed) == 16
    ), "[SPHINCS+-SHAKE-128f-simple] Expected secret key seed to be 16 -bytes"
    assert (
        len(sk_prf) == 16
    ), "[SPHINCS+-SHAKE-128f-simple] Expected secret key PRF to be 16 -bytes"
    assert (
        len(pk_seed) == 16
    ), "[SPHINCS+-SHAKE-128f-simple] Expected public key seed to be 16 -bytes"

    skey = create_string_buffer(64)
    pkey = create_string_buffer(32)

    args = [c_char_p, c_char_p, c_char_p, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_128f_simple_keygen.argtypes = args

    SO_LIB.sphincs_shake_128f_simple_keygen(sk_seed, sk_prf, pk_seed, skey, pkey)

    return skey.raw, pkey.raw


def sphincs_shake_192s_robust_keygen(
    sk_seed: bytes, sk_prf: bytes, pk_seed: bytes
) -> Tuple[bytes, bytes]:
    """
    Given 24 -bytes secret key seed, 24 -bytes secret key PRF and 24 -bytes
    public key seed, this routine compute 96 -bytes secret key and 48 -bytes
    public key, using SPHINCS+-SHAKE-192s-robust keygen algorithm
    """
    assert (
        len(sk_seed) == 24
    ), "[SPHINCS+-SHAKE-192s-robust] Expected secret key seed to be 24 -bytes"
    assert (
        len(sk_prf) == 24
    ), "[SPHINCS+-SHAKE-192s-robust] Expected secret key PRF to be 24 -bytes"
    assert (
        len(pk_seed) == 24
    ), "[SPHINCS+-SHAKE-192s-robust] Expected public key seed to be 24 -bytes"

    skey = create_string_buffer(96)
    pkey = create_string_buffer(48)

    args = [c_char_p, c_char_p, c_char_p, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_192s_robust_keygen.argtypes = args

    SO_LIB.sphincs_shake_192s_robust_keygen(sk_seed, sk_prf, pk_seed, skey, pkey)

    return skey.raw, pkey.raw


def sphincs_shake_192s_simple_keygen(
    sk_seed: bytes, sk_prf: bytes, pk_seed: bytes
) -> Tuple[bytes, bytes]:
    """
    Given 24 -bytes secret key seed, 24 -bytes secret key PRF and 24 -bytes
    public key seed, this routine compute 96 -bytes secret key and 48 -bytes
    public key, using SPHINCS+-SHAKE-192s-simple keygen algorithm
    """
    assert (
        len(sk_seed) == 24
    ), "[SPHINCS+-SHAKE-192s-simple] Expected secret key seed to be 24 -bytes"
    assert (
        len(sk_prf) == 24
    ), "[SPHINCS+-SHAKE-192s-simple] Expected secret key PRF to be 24 -bytes"
    assert (
        len(pk_seed) == 24
    ), "[SPHINCS+-SHAKE-192s-simple] Expected public key seed to be 24 -bytes"

    skey = create_string_buffer(96)
    pkey = create_string_buffer(48)

    args = [c_char_p, c_char_p, c_char_p, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_192s_simple_keygen.argtypes = args

    SO_LIB.sphincs_shake_192s_simple_keygen(sk_seed, sk_prf, pk_seed, skey, pkey)

    return skey.raw, pkey.raw


def sphincs_shake_192f_robust_keygen(
    sk_seed: bytes, sk_prf: bytes, pk_seed: bytes
) -> Tuple[bytes, bytes]:
    """
    Given 24 -bytes secret key seed, 24 -bytes secret key PRF and 24 -bytes
    public key seed, this routine compute 96 -bytes secret key and 48 -bytes
    public key, using SPHINCS+-SHAKE-192f-robust keygen algorithm
    """
    assert (
        len(sk_seed) == 24
    ), "[SPHINCS+-SHAKE-192f-robust] Expected secret key seed to be 24 -bytes"
    assert (
        len(sk_prf) == 24
    ), "[SPHINCS+-SHAKE-192f-robust] Expected secret key PRF to be 24 -bytes"
    assert (
        len(pk_seed) == 24
    ), "[SPHINCS+-SHAKE-192f-robust] Expected public key seed to be 24 -bytes"

    skey = create_string_buffer(96)
    pkey = create_string_buffer(48)

    args = [c_char_p, c_char_p, c_char_p, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_192f_robust_keygen.argtypes = args

    SO_LIB.sphincs_shake_192f_robust_keygen(sk_seed, sk_prf, pk_seed, skey, pkey)

    return skey.raw, pkey.raw


def sphincs_shake_192f_simple_keygen(
    sk_seed: bytes, sk_prf: bytes, pk_seed: bytes
) -> Tuple[bytes, bytes]:
    """
    Given 24 -bytes secret key seed, 24 -bytes secret key PRF and 24 -bytes
    public key seed, this routine compute 96 -bytes secret key and 48 -bytes
    public key, using SPHINCS+-SHAKE-192f-simple keygen algorithm
    """
    assert (
        len(sk_seed) == 24
    ), "[SPHINCS+-SHAKE-192f-simple] Expected secret key seed to be 24 -bytes"
    assert (
        len(sk_prf) == 24
    ), "[SPHINCS+-SHAKE-192f-simple] Expected secret key PRF to be 24 -bytes"
    assert (
        len(pk_seed) == 24
    ), "[SPHINCS+-SHAKE-192f-simple] Expected public key seed to be 24 -bytes"

    skey = create_string_buffer(96)
    pkey = create_string_buffer(48)

    args = [c_char_p, c_char_p, c_char_p, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_192f_simple_keygen.argtypes = args

    SO_LIB.sphincs_shake_192f_simple_keygen(sk_seed, sk_prf, pk_seed, skey, pkey)

    return skey.raw, pkey.raw


def sphincs_shake_256s_robust_keygen(
    sk_seed: bytes, sk_prf: bytes, pk_seed: bytes
) -> Tuple[bytes, bytes]:
    """
    Given 32 -bytes secret key seed, 32 -bytes secret key PRF and 32 -bytes
    public key seed, this routine compute 128 -bytes secret key and 64 -bytes
    public key, using SPHINCS+-SHAKE-256s-robust keygen algorithm
    """
    assert (
        len(sk_seed) == 32
    ), "[SPHINCS+-SHAKE-256s-robust] Expected secret key seed to be 32 -bytes"
    assert (
        len(sk_prf) == 32
    ), "[SPHINCS+-SHAKE-256s-robust] Expected secret key PRF to be 32 -bytes"
    assert (
        len(pk_seed) == 32
    ), "[SPHINCS+-SHAKE-256s-robust] Expected public key seed to be 32 -bytes"

    skey = create_string_buffer(128)
    pkey = create_string_buffer(64)

    args = [c_char_p, c_char_p, c_char_p, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_256s_robust_keygen.argtypes = args

    SO_LIB.sphincs_shake_256s_robust_keygen(sk_seed, sk_prf, pk_seed, skey, pkey)

    return skey.raw, pkey.raw


def sphincs_shake_256s_simple_keygen(
    sk_seed: bytes, sk_prf: bytes, pk_seed: bytes
) -> Tuple[bytes, bytes]:
    """
    Given 32 -bytes secret key seed, 32 -bytes secret key PRF and 32 -bytes
    public key seed, this routine compute 128 -bytes secret key and 64 -bytes
    public key, using SPHINCS+-SHAKE-256s-simple keygen algorithm
    """
    assert (
        len(sk_seed) == 32
    ), "[SPHINCS+-SHAKE-256s-simple] Expected secret key seed to be 32 -bytes"
    assert (
        len(sk_prf) == 32
    ), "[SPHINCS+-SHAKE-256s-simple] Expected secret key PRF to be 32 -bytes"
    assert (
        len(pk_seed) == 32
    ), "[SPHINCS+-SHAKE-256s-simple] Expected public key seed to be 32 -bytes"

    skey = create_string_buffer(128)
    pkey = create_string_buffer(64)

    args = [c_char_p, c_char_p, c_char_p, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_256s_simple_keygen.argtypes = args

    SO_LIB.sphincs_shake_256s_simple_keygen(sk_seed, sk_prf, pk_seed, skey, pkey)

    return skey.raw, pkey.raw


def sphincs_shake_256f_robust_keygen(
    sk_seed: bytes, sk_prf: bytes, pk_seed: bytes
) -> Tuple[bytes, bytes]:
    """
    Given 32 -bytes secret key seed, 32 -bytes secret key PRF and 32 -bytes
    public key seed, this routine compute 128 -bytes secret key and 64 -bytes
    public key, using SPHINCS+-SHAKE-256f-robust keygen algorithm
    """
    assert (
        len(sk_seed) == 32
    ), "[SPHINCS+-SHAKE-256f-robust] Expected secret key seed to be 32 -bytes"
    assert (
        len(sk_prf) == 32
    ), "[SPHINCS+-SHAKE-256f-robust] Expected secret key PRF to be 32 -bytes"
    assert (
        len(pk_seed) == 32
    ), "[SPHINCS+-SHAKE-256f-robust] Expected public key seed to be 32 -bytes"

    skey = create_string_buffer(128)
    pkey = create_string_buffer(64)

    args = [c_char_p, c_char_p, c_char_p, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_256f_robust_keygen.argtypes = args

    SO_LIB.sphincs_shake_256f_robust_keygen(sk_seed, sk_prf, pk_seed, skey, pkey)

    return skey.raw, pkey.raw


def sphincs_shake_256f_simple_keygen(
    sk_seed: bytes, sk_prf: bytes, pk_seed: bytes
) -> Tuple[bytes, bytes]:
    """
    Given 32 -bytes secret key seed, 32 -bytes secret key PRF and 32 -bytes
    public key seed, this routine compute 128 -bytes secret key and 64 -bytes
    public key, using SPHINCS+-SHAKE-256f-simple keygen algorithm
    """
    assert (
        len(sk_seed) == 32
    ), "[SPHINCS+-SHAKE-256f-simple] Expected secret key seed to be 32 -bytes"
    assert (
        len(sk_prf) == 32
    ), "[SPHINCS+-SHAKE-256f-simple] Expected secret key PRF to be 32 -bytes"
    assert (
        len(pk_seed) == 32
    ), "[SPHINCS+-SHAKE-256f-simple] Expected public key seed to be 32 -bytes"

    skey = create_string_buffer(128)
    pkey = create_string_buffer(64)

    args = [c_char_p, c_char_p, c_char_p, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_256f_simple_keygen.argtypes = args

    SO_LIB.sphincs_shake_256f_simple_keygen(sk_seed, sk_prf, pk_seed, skey, pkey)

    return skey.raw, pkey.raw


def sphincs_shake_128s_robust_sign(
    msg: bytes, mlen: int, skey: bytes, rbytes: bytes
) -> bytes:
    """
    Given mlen -bytes message, 64 -bytes secret key and 16 -bytes signature
    randomization seed, this routine computes SPHINCS+ signature using
    SPHINCS+-SHAKE-128s-robust signing algorithm
    """
    sig = create_string_buffer(7856)

    args = [c_char_p, c_size_t, c_char_p, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_128s_robust_sign.argtypes = args

    SO_LIB.sphincs_shake_128s_robust_sign(msg, mlen, skey, rbytes, sig)

    return sig.raw


def sphincs_shake_128s_simple_sign(
    msg: bytes, mlen: int, skey: bytes, rbytes: bytes
) -> bytes:
    """
    Given mlen -bytes message, 64 -bytes secret key and 16 -bytes signature
    randomization seed, this routine computes SPHINCS+ signature using
    SPHINCS+-SHAKE-128s-simple signing algorithm
    """
    sig = create_string_buffer(7856)

    args = [c_char_p, c_size_t, c_char_p, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_128s_simple_sign.argtypes = args

    SO_LIB.sphincs_shake_128s_simple_sign(msg, mlen, skey, rbytes, sig)

    return sig.raw


def sphincs_shake_128f_robust_sign(
    msg: bytes, mlen: int, skey: bytes, rbytes: bytes
) -> bytes:
    """
    Given mlen -bytes message, 64 -bytes secret key and 16 -bytes signature
    randomization seed, this routine computes SPHINCS+ signature using
    SPHINCS+-SHAKE-128f-robust signing algorithm
    """
    sig = create_string_buffer(17088)

    args = [c_char_p, c_size_t, c_char_p, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_128f_robust_sign.argtypes = args

    SO_LIB.sphincs_shake_128f_robust_sign(msg, mlen, skey, rbytes, sig)

    return sig.raw


def sphincs_shake_128f_simple_sign(
    msg: bytes, mlen: int, skey: bytes, rbytes: bytes
) -> bytes:
    """
    Given mlen -bytes message, 64 -bytes secret key and 16 -bytes signature
    randomization seed, this routine computes SPHINCS+ signature using
    SPHINCS+-SHAKE-128f-simple signing algorithm
    """
    sig = create_string_buffer(17088)

    args = [c_char_p, c_size_t, c_char_p, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_128f_simple_sign.argtypes = args

    SO_LIB.sphincs_shake_128f_simple_sign(msg, mlen, skey, rbytes, sig)

    return sig.raw


def sphincs_shake_192s_robust_sign(
    msg: bytes, mlen: int, skey: bytes, rbytes: bytes
) -> bytes:
    """
    Given mlen -bytes message, 96 -bytes secret key and 24 -bytes signature
    randomization seed, this routine computes SPHINCS+ signature using
    SPHINCS+-SHAKE-192s-robust signing algorithm
    """
    sig = create_string_buffer(16224)

    args = [c_char_p, c_size_t, c_char_p, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_192s_robust_sign.argtypes = args

    SO_LIB.sphincs_shake_192s_robust_sign(msg, mlen, skey, rbytes, sig)

    return sig.raw


def sphincs_shake_192s_simple_sign(
    msg: bytes, mlen: int, skey: bytes, rbytes: bytes
) -> bytes:
    """
    Given mlen -bytes message, 96 -bytes secret key and 24 -bytes signature
    randomization seed, this routine computes SPHINCS+ signature using
    SPHINCS+-SHAKE-192s-simple signing algorithm
    """
    sig = create_string_buffer(16224)

    args = [c_char_p, c_size_t, c_char_p, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_192s_simple_sign.argtypes = args

    SO_LIB.sphincs_shake_192s_simple_sign(msg, mlen, skey, rbytes, sig)

    return sig.raw


def sphincs_shake_192f_robust_sign(
    msg: bytes, mlen: int, skey: bytes, rbytes: bytes
) -> bytes:
    """
    Given mlen -bytes message, 96 -bytes secret key and 24 -bytes signature
    randomization seed, this routine computes SPHINCS+ signature using
    SPHINCS+-SHAKE-192f-robust signing algorithm
    """
    sig = create_string_buffer(35664)

    args = [c_char_p, c_size_t, c_char_p, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_192f_robust_sign.argtypes = args

    SO_LIB.sphincs_shake_192f_robust_sign(msg, mlen, skey, rbytes, sig)

    return sig.raw


def sphincs_shake_192f_simple_sign(
    msg: bytes, mlen: int, skey: bytes, rbytes: bytes
) -> bytes:
    """
    Given mlen -bytes message, 96 -bytes secret key and 24 -bytes signature
    randomization seed, this routine computes SPHINCS+ signature using
    SPHINCS+-SHAKE-192f-simple signing algorithm
    """
    sig = create_string_buffer(35664)

    args = [c_char_p, c_size_t, c_char_p, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_192f_simple_sign.argtypes = args

    SO_LIB.sphincs_shake_192f_simple_sign(msg, mlen, skey, rbytes, sig)

    return sig.raw


def sphincs_shake_256s_robust_sign(
    msg: bytes, mlen: int, skey: bytes, rbytes: bytes
) -> bytes:
    """
    Given mlen -bytes message, 128 -bytes secret key and 32 -bytes signature
    randomization seed, this routine computes SPHINCS+ signature using
    SPHINCS+-SHAKE-256s-robust signing algorithm
    """
    sig = create_string_buffer(29792)

    args = [c_char_p, c_size_t, c_char_p, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_256s_robust_sign.argtypes = args

    SO_LIB.sphincs_shake_256s_robust_sign(msg, mlen, skey, rbytes, sig)

    return sig.raw


def sphincs_shake_256s_simple_sign(
    msg: bytes, mlen: int, skey: bytes, rbytes: bytes
) -> bytes:
    """
    Given mlen -bytes message, 128 -bytes secret key and 32 -bytes signature
    randomization seed, this routine computes SPHINCS+ signature using
    SPHINCS+-SHAKE-256s-simple signing algorithm
    """
    sig = create_string_buffer(29792)

    args = [c_char_p, c_size_t, c_char_p, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_256s_simple_sign.argtypes = args

    SO_LIB.sphincs_shake_256s_simple_sign(msg, mlen, skey, rbytes, sig)

    return sig.raw


def sphincs_shake_256f_robust_sign(
    msg: bytes, mlen: int, skey: bytes, rbytes: bytes
) -> bytes:
    """
    Given mlen -bytes message, 128 -bytes secret key and 32 -bytes signature
    randomization seed, this routine computes SPHINCS+ signature using
    SPHINCS+-SHAKE-256f-robust signing algorithm
    """
    sig = create_string_buffer(49856)

    args = [c_char_p, c_size_t, c_char_p, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_256f_robust_sign.argtypes = args

    SO_LIB.sphincs_shake_256f_robust_sign(msg, mlen, skey, rbytes, sig)

    return sig.raw


def sphincs_shake_256f_simple_sign(
    msg: bytes, mlen: int, skey: bytes, rbytes: bytes
) -> bytes:
    """
    Given mlen -bytes message, 128 -bytes secret key and 32 -bytes signature
    randomization seed, this routine computes SPHINCS+ signature using
    SPHINCS+-SHAKE-256f-simple signing algorithm
    """
    sig = create_string_buffer(49856)

    args = [c_char_p, c_size_t, c_char_p, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_256f_simple_sign.argtypes = args

    SO_LIB.sphincs_shake_256f_simple_sign(msg, mlen, skey, rbytes, sig)

    return sig.raw


def sphincs_shake_128s_robust_verify(
    msg: bytes, mlen: int, sig: bytes, pkey: bytes
) -> bool:
    """
    Given mlen -bytes message, SPHINCS+ signature and 32 -bytes public key,
    this routine verifies signature using SPHINCS+-SHAKE-128s-robust algorithm
    and returns boolean truth value if signature verification succeeds
    """
    args = [c_char_p, c_size_t, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_128s_robust_verify.argtypes = args
    SO_LIB.sphincs_shake_128s_robust_verify.restype = c_bool

    verified = SO_LIB.sphincs_shake_128s_robust_verify(msg, mlen, sig, pkey)
    return verified


def sphincs_shake_128s_simple_verify(
    msg: bytes, mlen: int, sig: bytes, pkey: bytes
) -> bool:
    """
    Given mlen -bytes message, SPHINCS+ signature and 32 -bytes public key,
    this routine verifies signature using SPHINCS+-SHAKE-128s-simple algorithm
    and returns boolean truth value if signature verification succeeds
    """
    args = [c_char_p, c_size_t, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_128s_simple_verify.argtypes = args
    SO_LIB.sphincs_shake_128s_simple_verify.restype = c_bool

    verified = SO_LIB.sphincs_shake_128s_simple_verify(msg, mlen, sig, pkey)
    return verified


def sphincs_shake_128f_robust_verify(
    msg: bytes, mlen: int, sig: bytes, pkey: bytes
) -> bool:
    """
    Given mlen -bytes message, SPHINCS+ signature and 32 -bytes public key,
    this routine verifies signature using SPHINCS+-SHAKE-128f-robust algorithm
    and returns boolean truth value if signature verification succeeds
    """
    args = [c_char_p, c_size_t, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_128f_robust_verify.argtypes = args
    SO_LIB.sphincs_shake_128f_robust_verify.restype = c_bool

    verified = SO_LIB.sphincs_shake_128f_robust_verify(msg, mlen, sig, pkey)
    return verified


def sphincs_shake_128f_simple_verify(
    msg: bytes, mlen: int, sig: bytes, pkey: bytes
) -> bool:
    """
    Given mlen -bytes message, SPHINCS+ signature and 32 -bytes public key,
    this routine verifies signature using SPHINCS+-SHAKE-128f-simple algorithm
    and returns boolean truth value if signature verification succeeds
    """
    args = [c_char_p, c_size_t, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_128f_simple_verify.argtypes = args
    SO_LIB.sphincs_shake_128f_simple_verify.restype = c_bool

    verified = SO_LIB.sphincs_shake_128f_simple_verify(msg, mlen, sig, pkey)
    return verified


def sphincs_shake_192s_robust_verify(
    msg: bytes, mlen: int, sig: bytes, pkey: bytes
) -> bool:
    """
    Given mlen -bytes message, SPHINCS+ signature and 48 -bytes public key,
    this routine verifies signature using SPHINCS+-SHAKE-192s-robust algorithm
    and returns boolean truth value if signature verification succeeds
    """
    args = [c_char_p, c_size_t, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_192s_robust_verify.argtypes = args
    SO_LIB.sphincs_shake_192s_robust_verify.restype = c_bool

    verified = SO_LIB.sphincs_shake_192s_robust_verify(msg, mlen, sig, pkey)
    return verified


def sphincs_shake_192s_simple_verify(
    msg: bytes, mlen: int, sig: bytes, pkey: bytes
) -> bool:
    """
    Given mlen -bytes message, SPHINCS+ signature and 48 -bytes public key,
    this routine verifies signature using SPHINCS+-SHAKE-192s-simple algorithm
    and returns boolean truth value if signature verification succeeds
    """
    args = [c_char_p, c_size_t, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_192s_simple_verify.argtypes = args
    SO_LIB.sphincs_shake_192s_simple_verify.restype = c_bool

    verified = SO_LIB.sphincs_shake_192s_simple_verify(msg, mlen, sig, pkey)
    return verified


def sphincs_shake_192f_robust_verify(
    msg: bytes, mlen: int, sig: bytes, pkey: bytes
) -> bool:
    """
    Given mlen -bytes message, SPHINCS+ signature and 48 -bytes public key,
    this routine verifies signature using SPHINCS+-SHAKE-192f-robust algorithm
    and returns boolean truth value if signature verification succeeds
    """
    args = [c_char_p, c_size_t, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_192f_robust_verify.argtypes = args
    SO_LIB.sphincs_shake_192f_robust_verify.restype = c_bool

    verified = SO_LIB.sphincs_shake_192f_robust_verify(msg, mlen, sig, pkey)
    return verified


def sphincs_shake_192f_simple_verify(
    msg: bytes, mlen: int, sig: bytes, pkey: bytes
) -> bool:
    """
    Given mlen -bytes message, SPHINCS+ signature and 48 -bytes public key,
    this routine verifies signature using SPHINCS+-SHAKE-192f-simple algorithm
    and returns boolean truth value if signature verification succeeds
    """
    args = [c_char_p, c_size_t, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_192f_simple_verify.argtypes = args
    SO_LIB.sphincs_shake_192f_simple_verify.restype = c_bool

    verified = SO_LIB.sphincs_shake_192f_simple_verify(msg, mlen, sig, pkey)
    return verified


def sphincs_shake_256s_robust_verify(
    msg: bytes, mlen: int, sig: bytes, pkey: bytes
) -> bool:
    """
    Given mlen -bytes message, SPHINCS+ signature and 64 -bytes public key,
    this routine verifies signature using SPHINCS+-SHAKE-256s-robust algorithm
    and returns boolean truth value if signature verification succeeds
    """
    args = [c_char_p, c_size_t, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_256s_robust_verify.argtypes = args
    SO_LIB.sphincs_shake_256s_robust_verify.restype = c_bool

    verified = SO_LIB.sphincs_shake_256s_robust_verify(msg, mlen, sig, pkey)
    return verified


def sphincs_shake_256s_simple_verify(
    msg: bytes, mlen: int, sig: bytes, pkey: bytes
) -> bool:
    """
    Given mlen -bytes message, SPHINCS+ signature and 64 -bytes public key,
    this routine verifies signature using SPHINCS+-SHAKE-256s-simple algorithm
    and returns boolean truth value if signature verification succeeds
    """
    args = [c_char_p, c_size_t, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_256s_simple_verify.argtypes = args
    SO_LIB.sphincs_shake_256s_simple_verify.restype = c_bool

    verified = SO_LIB.sphincs_shake_256s_simple_verify(msg, mlen, sig, pkey)
    return verified


def sphincs_shake_256f_robust_verify(
    msg: bytes, mlen: int, sig: bytes, pkey: bytes
) -> bool:
    """
    Given mlen -bytes message, SPHINCS+ signature and 64 -bytes public key,
    this routine verifies signature using SPHINCS+-SHAKE-256f-robust algorithm
    and returns boolean truth value if signature verification succeeds
    """
    args = [c_char_p, c_size_t, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_256f_robust_verify.argtypes = args
    SO_LIB.sphincs_shake_256f_robust_verify.restype = c_bool

    verified = SO_LIB.sphincs_shake_256f_robust_verify(msg, mlen, sig, pkey)
    return verified


def sphincs_shake_256f_simple_verify(
    msg: bytes, mlen: int, sig: bytes, pkey: bytes
) -> bool:
    """
    Given mlen -bytes message, SPHINCS+ signature and 64 -bytes public key,
    this routine verifies signature using SPHINCS+-SHAKE-256f-simple algorithm
    and returns boolean truth value if signature verification succeeds
    """
    args = [c_char_p, c_size_t, c_char_p, c_char_p]
    SO_LIB.sphincs_shake_256f_simple_verify.argtypes = args
    SO_LIB.sphincs_shake_256f_simple_verify.restype = c_bool

    verified = SO_LIB.sphincs_shake_256f_simple_verify(msg, mlen, sig, pkey)
    return verified


if __name__ == "__main__":
    print("Use `sphincs-shake` as library module")
