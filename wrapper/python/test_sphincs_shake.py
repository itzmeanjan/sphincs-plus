#!/usr/bin/python3

import sphincs_shake as spx


def extract_out_value(key_val: str) -> str:
    return [i.strip() for i in key_val.split("=")][-1]


def hex_to_bytes(hs: str) -> bytes:
    return bytes(
        [int(f"0x{hs[i * 2: (i+1) * 2]}", base=16) for i in range(len(hs) >> 1)]
    )


def test_sphincs_shake_128s_robust():
    """
    Ensures functional correctness and compatibility of SPHINCS+-SHAKE-128s-robust DSA implementation,
    against original reference implementation https://github.com/sphincs/sphincsplus

    Known Answer Tests generated using https://gist.github.com/itzmeanjan/d483872509b8a1a7c4d6614ec9d43e6c
    """

    alg = "SPHINCS+-SHAKE-128s-robust"

    with open("sphincs-shake-128s-robust.kat", "r") as fd:
        while True:
            sk_seed = fd.readline()
            if not sk_seed:
                # no more KAT
                break

            sk_prf = fd.readline()
            pk_seed = fd.readline()
            pk_root = fd.readline()
            mlen = fd.readline()
            msg = fd.readline()
            opt = fd.readline()
            sig = fd.readline()

            # extract out required fields
            sk_seed = extract_out_value(sk_seed)
            sk_prf = extract_out_value(sk_prf)
            pk_seed = extract_out_value(pk_seed)
            pk_root = extract_out_value(pk_root)
            mlen = extract_out_value(mlen)
            msg = extract_out_value(msg)
            opt = extract_out_value(opt)
            sig = extract_out_value(sig)

            # convert input hex strings to bytes object
            sk_seed = hex_to_bytes(sk_seed)
            sk_prf = hex_to_bytes(sk_prf)
            pk_seed = hex_to_bytes(pk_seed)
            pk_root = hex_to_bytes(pk_root)
            mlen = int(mlen)
            msg = hex_to_bytes(msg)
            opt = hex_to_bytes(opt)
            sig = hex_to_bytes(sig)

            pkey = pk_seed + pk_root
            skey = sk_seed + sk_prf + pkey

            keys = spx.sphincs_shake_128s_robust_keygen(sk_seed, sk_prf, pk_seed)
            sig_ = spx.sphincs_shake_128s_robust_sign(msg, mlen, keys[0], opt)
            verified = spx.sphincs_shake_128s_robust_verify(msg, mlen, sig_, keys[1])

            assert verified, f"[{alg}] Failed to verify signature"
            assert pkey == keys[1], f"[{alg}] Public key doesn't match"
            assert skey == keys[0], f"[{alg}] Secret key doesn't match"
            assert sig_ == sig, f"[{alg}] Signature doesn't match"

            fd.readline()


def test_sphincs_shake_128s_simple():
    """
    Ensures functional correctness and compatibility of SPHINCS+-SHAKE-128s-simple DSA implementation,
    against original reference implementation https://github.com/sphincs/sphincsplus

    Known Answer Tests generated using https://gist.github.com/itzmeanjan/d483872509b8a1a7c4d6614ec9d43e6c
    """

    alg = "SPHINCS+-SHAKE-128s-simple"

    with open("sphincs-shake-128s-simple.kat", "r") as fd:
        while True:
            sk_seed = fd.readline()
            if not sk_seed:
                # no more KAT
                break

            sk_prf = fd.readline()
            pk_seed = fd.readline()
            pk_root = fd.readline()
            mlen = fd.readline()
            msg = fd.readline()
            opt = fd.readline()
            sig = fd.readline()

            # extract out required fields
            sk_seed = extract_out_value(sk_seed)
            sk_prf = extract_out_value(sk_prf)
            pk_seed = extract_out_value(pk_seed)
            pk_root = extract_out_value(pk_root)
            mlen = extract_out_value(mlen)
            msg = extract_out_value(msg)
            opt = extract_out_value(opt)
            sig = extract_out_value(sig)

            # convert input hex strings to bytes object
            sk_seed = hex_to_bytes(sk_seed)
            sk_prf = hex_to_bytes(sk_prf)
            pk_seed = hex_to_bytes(pk_seed)
            pk_root = hex_to_bytes(pk_root)
            mlen = int(mlen)
            msg = hex_to_bytes(msg)
            opt = hex_to_bytes(opt)
            sig = hex_to_bytes(sig)

            pkey = pk_seed + pk_root
            skey = sk_seed + sk_prf + pkey

            keys = spx.sphincs_shake_128s_simple_keygen(sk_seed, sk_prf, pk_seed)
            sig_ = spx.sphincs_shake_128s_simple_sign(msg, mlen, keys[0], opt)
            verified = spx.sphincs_shake_128s_simple_verify(msg, mlen, sig_, keys[1])

            assert verified, f"[{alg}] Failed to verify signature"
            assert pkey == keys[1], f"[{alg}] Public key doesn't match"
            assert skey == keys[0], f"[{alg}] Secret key doesn't match"
            assert sig_ == sig, f"[{alg}] Signature doesn't match"

            fd.readline()


def test_sphincs_shake_128f_robust():
    """
    Ensures functional correctness and compatibility of SPHINCS+-SHAKE-128f-robust DSA implementation,
    against original reference implementation https://github.com/sphincs/sphincsplus

    Known Answer Tests generated using https://gist.github.com/itzmeanjan/d483872509b8a1a7c4d6614ec9d43e6c
    """

    alg = "SPHINCS+-SHAKE-128f-robust"

    with open("sphincs-shake-128f-robust.kat", "r") as fd:
        while True:
            sk_seed = fd.readline()
            if not sk_seed:
                # no more KAT
                break

            sk_prf = fd.readline()
            pk_seed = fd.readline()
            pk_root = fd.readline()
            mlen = fd.readline()
            msg = fd.readline()
            opt = fd.readline()
            sig = fd.readline()

            # extract out required fields
            sk_seed = extract_out_value(sk_seed)
            sk_prf = extract_out_value(sk_prf)
            pk_seed = extract_out_value(pk_seed)
            pk_root = extract_out_value(pk_root)
            mlen = extract_out_value(mlen)
            msg = extract_out_value(msg)
            opt = extract_out_value(opt)
            sig = extract_out_value(sig)

            # convert input hex strings to bytes object
            sk_seed = hex_to_bytes(sk_seed)
            sk_prf = hex_to_bytes(sk_prf)
            pk_seed = hex_to_bytes(pk_seed)
            pk_root = hex_to_bytes(pk_root)
            mlen = int(mlen)
            msg = hex_to_bytes(msg)
            opt = hex_to_bytes(opt)
            sig = hex_to_bytes(sig)

            pkey = pk_seed + pk_root
            skey = sk_seed + sk_prf + pkey

            keys = spx.sphincs_shake_128f_robust_keygen(sk_seed, sk_prf, pk_seed)
            sig_ = spx.sphincs_shake_128f_robust_sign(msg, mlen, keys[0], opt)
            verified = spx.sphincs_shake_128f_robust_verify(msg, mlen, sig_, keys[1])

            assert verified, f"[{alg}] Failed to verify signature"
            assert pkey == keys[1], f"[{alg}] Public key doesn't match"
            assert skey == keys[0], f"[{alg}] Secret key doesn't match"
            assert sig_ == sig, f"[{alg}] Signature doesn't match"

            fd.readline()


def test_sphincs_shake_128f_simple():
    """
    Ensures functional correctness and compatibility of SPHINCS+-SHAKE-128f-simple DSA implementation,
    against original reference implementation https://github.com/sphincs/sphincsplus

    Known Answer Tests generated using https://gist.github.com/itzmeanjan/d483872509b8a1a7c4d6614ec9d43e6c
    """

    alg = "SPHINCS+-SHAKE-128f-simple"

    with open("sphincs-shake-128f-simple.kat", "r") as fd:
        while True:
            sk_seed = fd.readline()
            if not sk_seed:
                # no more KAT
                break

            sk_prf = fd.readline()
            pk_seed = fd.readline()
            pk_root = fd.readline()
            mlen = fd.readline()
            msg = fd.readline()
            opt = fd.readline()
            sig = fd.readline()

            # extract out required fields
            sk_seed = extract_out_value(sk_seed)
            sk_prf = extract_out_value(sk_prf)
            pk_seed = extract_out_value(pk_seed)
            pk_root = extract_out_value(pk_root)
            mlen = extract_out_value(mlen)
            msg = extract_out_value(msg)
            opt = extract_out_value(opt)
            sig = extract_out_value(sig)

            # convert input hex strings to bytes object
            sk_seed = hex_to_bytes(sk_seed)
            sk_prf = hex_to_bytes(sk_prf)
            pk_seed = hex_to_bytes(pk_seed)
            pk_root = hex_to_bytes(pk_root)
            mlen = int(mlen)
            msg = hex_to_bytes(msg)
            opt = hex_to_bytes(opt)
            sig = hex_to_bytes(sig)

            pkey = pk_seed + pk_root
            skey = sk_seed + sk_prf + pkey

            keys = spx.sphincs_shake_128f_simple_keygen(sk_seed, sk_prf, pk_seed)
            sig_ = spx.sphincs_shake_128f_simple_sign(msg, mlen, keys[0], opt)
            verified = spx.sphincs_shake_128f_simple_verify(msg, mlen, sig_, keys[1])

            assert verified, f"[{alg}] Failed to verify signature"
            assert pkey == keys[1], f"[{alg}] Public key doesn't match"
            assert skey == keys[0], f"[{alg}] Secret key doesn't match"
            assert sig_ == sig, f"[{alg}] Signature doesn't match"

            fd.readline()


def test_sphincs_shake_192s_robust():
    """
    Ensures functional correctness and compatibility of SPHINCS+-SHAKE-192s-robust DSA implementation,
    against original reference implementation https://github.com/sphincs/sphincsplus

    Known Answer Tests generated using https://gist.github.com/itzmeanjan/d483872509b8a1a7c4d6614ec9d43e6c
    """

    alg = "SPHINCS+-SHAKE-192s-robust"

    with open("sphincs-shake-192s-robust.kat", "r") as fd:
        while True:
            sk_seed = fd.readline()
            if not sk_seed:
                # no more KAT
                break

            sk_prf = fd.readline()
            pk_seed = fd.readline()
            pk_root = fd.readline()
            mlen = fd.readline()
            msg = fd.readline()
            opt = fd.readline()
            sig = fd.readline()

            # extract out required fields
            sk_seed = extract_out_value(sk_seed)
            sk_prf = extract_out_value(sk_prf)
            pk_seed = extract_out_value(pk_seed)
            pk_root = extract_out_value(pk_root)
            mlen = extract_out_value(mlen)
            msg = extract_out_value(msg)
            opt = extract_out_value(opt)
            sig = extract_out_value(sig)

            # convert input hex strings to bytes object
            sk_seed = hex_to_bytes(sk_seed)
            sk_prf = hex_to_bytes(sk_prf)
            pk_seed = hex_to_bytes(pk_seed)
            pk_root = hex_to_bytes(pk_root)
            mlen = int(mlen)
            msg = hex_to_bytes(msg)
            opt = hex_to_bytes(opt)
            sig = hex_to_bytes(sig)

            pkey = pk_seed + pk_root
            skey = sk_seed + sk_prf + pkey

            keys = spx.sphincs_shake_192s_robust_keygen(sk_seed, sk_prf, pk_seed)
            sig_ = spx.sphincs_shake_192s_robust_sign(msg, mlen, keys[0], opt)
            verified = spx.sphincs_shake_192s_robust_verify(msg, mlen, sig_, keys[1])

            assert verified, f"[{alg}] Failed to verify signature"
            assert pkey == keys[1], f"[{alg}] Public key doesn't match"
            assert skey == keys[0], f"[{alg}] Secret key doesn't match"
            assert sig_ == sig, f"[{alg}] Signature doesn't match"

            fd.readline()


def test_sphincs_shake_192s_simple():
    """
    Ensures functional correctness and compatibility of SPHINCS+-SHAKE-192s-simple DSA implementation,
    against original reference implementation https://github.com/sphincs/sphincsplus

    Known Answer Tests generated using https://gist.github.com/itzmeanjan/d483872509b8a1a7c4d6614ec9d43e6c
    """

    alg = "SPHINCS+-SHAKE-192s-simple"

    with open("sphincs-shake-192s-simple.kat", "r") as fd:
        while True:
            sk_seed = fd.readline()
            if not sk_seed:
                # no more KAT
                break

            sk_prf = fd.readline()
            pk_seed = fd.readline()
            pk_root = fd.readline()
            mlen = fd.readline()
            msg = fd.readline()
            opt = fd.readline()
            sig = fd.readline()

            # extract out required fields
            sk_seed = extract_out_value(sk_seed)
            sk_prf = extract_out_value(sk_prf)
            pk_seed = extract_out_value(pk_seed)
            pk_root = extract_out_value(pk_root)
            mlen = extract_out_value(mlen)
            msg = extract_out_value(msg)
            opt = extract_out_value(opt)
            sig = extract_out_value(sig)

            # convert input hex strings to bytes object
            sk_seed = hex_to_bytes(sk_seed)
            sk_prf = hex_to_bytes(sk_prf)
            pk_seed = hex_to_bytes(pk_seed)
            pk_root = hex_to_bytes(pk_root)
            mlen = int(mlen)
            msg = hex_to_bytes(msg)
            opt = hex_to_bytes(opt)
            sig = hex_to_bytes(sig)

            pkey = pk_seed + pk_root
            skey = sk_seed + sk_prf + pkey

            keys = spx.sphincs_shake_192s_simple_keygen(sk_seed, sk_prf, pk_seed)
            sig_ = spx.sphincs_shake_192s_simple_sign(msg, mlen, keys[0], opt)
            verified = spx.sphincs_shake_192s_simple_verify(msg, mlen, sig_, keys[1])

            assert verified, f"[{alg}] Failed to verify signature"
            assert pkey == keys[1], f"[{alg}] Public key doesn't match"
            assert skey == keys[0], f"[{alg}] Secret key doesn't match"
            assert sig_ == sig, f"[{alg}] Signature doesn't match"

            fd.readline()


def test_sphincs_shake_192f_robust():
    """
    Ensures functional correctness and compatibility of SPHINCS+-SHAKE-192f-robust DSA implementation,
    against original reference implementation https://github.com/sphincs/sphincsplus

    Known Answer Tests generated using https://gist.github.com/itzmeanjan/d483872509b8a1a7c4d6614ec9d43e6c
    """

    alg = "SPHINCS+-SHAKE-192f-robust"

    with open("sphincs-shake-192f-robust.kat", "r") as fd:
        while True:
            sk_seed = fd.readline()
            if not sk_seed:
                # no more KAT
                break

            sk_prf = fd.readline()
            pk_seed = fd.readline()
            pk_root = fd.readline()
            mlen = fd.readline()
            msg = fd.readline()
            opt = fd.readline()
            sig = fd.readline()

            # extract out required fields
            sk_seed = extract_out_value(sk_seed)
            sk_prf = extract_out_value(sk_prf)
            pk_seed = extract_out_value(pk_seed)
            pk_root = extract_out_value(pk_root)
            mlen = extract_out_value(mlen)
            msg = extract_out_value(msg)
            opt = extract_out_value(opt)
            sig = extract_out_value(sig)

            # convert input hex strings to bytes object
            sk_seed = hex_to_bytes(sk_seed)
            sk_prf = hex_to_bytes(sk_prf)
            pk_seed = hex_to_bytes(pk_seed)
            pk_root = hex_to_bytes(pk_root)
            mlen = int(mlen)
            msg = hex_to_bytes(msg)
            opt = hex_to_bytes(opt)
            sig = hex_to_bytes(sig)

            pkey = pk_seed + pk_root
            skey = sk_seed + sk_prf + pkey

            keys = spx.sphincs_shake_192f_robust_keygen(sk_seed, sk_prf, pk_seed)
            sig_ = spx.sphincs_shake_192f_robust_sign(msg, mlen, keys[0], opt)
            verified = spx.sphincs_shake_192f_robust_verify(msg, mlen, sig_, keys[1])

            assert verified, f"[{alg}] Failed to verify signature"
            assert pkey == keys[1], f"[{alg}] Public key doesn't match"
            assert skey == keys[0], f"[{alg}] Secret key doesn't match"
            assert sig_ == sig, f"[{alg}] Signature doesn't match"

            fd.readline()


def test_sphincs_shake_192f_simple():
    """
    Ensures functional correctness and compatibility of SPHINCS+-SHAKE-192f-simple DSA implementation,
    against original reference implementation https://github.com/sphincs/sphincsplus

    Known Answer Tests generated using https://gist.github.com/itzmeanjan/d483872509b8a1a7c4d6614ec9d43e6c
    """

    alg = "SPHINCS+-SHAKE-192f-simple"

    with open("sphincs-shake-192f-simple.kat", "r") as fd:
        while True:
            sk_seed = fd.readline()
            if not sk_seed:
                # no more KAT
                break

            sk_prf = fd.readline()
            pk_seed = fd.readline()
            pk_root = fd.readline()
            mlen = fd.readline()
            msg = fd.readline()
            opt = fd.readline()
            sig = fd.readline()

            # extract out required fields
            sk_seed = extract_out_value(sk_seed)
            sk_prf = extract_out_value(sk_prf)
            pk_seed = extract_out_value(pk_seed)
            pk_root = extract_out_value(pk_root)
            mlen = extract_out_value(mlen)
            msg = extract_out_value(msg)
            opt = extract_out_value(opt)
            sig = extract_out_value(sig)

            # convert input hex strings to bytes object
            sk_seed = hex_to_bytes(sk_seed)
            sk_prf = hex_to_bytes(sk_prf)
            pk_seed = hex_to_bytes(pk_seed)
            pk_root = hex_to_bytes(pk_root)
            mlen = int(mlen)
            msg = hex_to_bytes(msg)
            opt = hex_to_bytes(opt)
            sig = hex_to_bytes(sig)

            pkey = pk_seed + pk_root
            skey = sk_seed + sk_prf + pkey

            keys = spx.sphincs_shake_192f_simple_keygen(sk_seed, sk_prf, pk_seed)
            sig_ = spx.sphincs_shake_192f_simple_sign(msg, mlen, keys[0], opt)
            verified = spx.sphincs_shake_192f_simple_verify(msg, mlen, sig_, keys[1])

            assert verified, f"[{alg}] Failed to verify signature"
            assert pkey == keys[1], f"[{alg}] Public key doesn't match"
            assert skey == keys[0], f"[{alg}] Secret key doesn't match"
            assert sig_ == sig, f"[{alg}] Signature doesn't match"

            fd.readline()


def test_sphincs_shake_256s_robust():
    """
    Ensures functional correctness and compatibility of SPHINCS+-SHAKE-256s-robust DSA implementation,
    against original reference implementation https://github.com/sphincs/sphincsplus

    Known Answer Tests generated using https://gist.github.com/itzmeanjan/d483872509b8a1a7c4d6614ec9d43e6c
    """

    alg = "SPHINCS+-SHAKE-256s-robust"

    with open("sphincs-shake-256s-robust.kat", "r") as fd:
        while True:
            sk_seed = fd.readline()
            if not sk_seed:
                # no more KAT
                break

            sk_prf = fd.readline()
            pk_seed = fd.readline()
            pk_root = fd.readline()
            mlen = fd.readline()
            msg = fd.readline()
            opt = fd.readline()
            sig = fd.readline()

            # extract out required fields
            sk_seed = extract_out_value(sk_seed)
            sk_prf = extract_out_value(sk_prf)
            pk_seed = extract_out_value(pk_seed)
            pk_root = extract_out_value(pk_root)
            mlen = extract_out_value(mlen)
            msg = extract_out_value(msg)
            opt = extract_out_value(opt)
            sig = extract_out_value(sig)

            # convert input hex strings to bytes object
            sk_seed = hex_to_bytes(sk_seed)
            sk_prf = hex_to_bytes(sk_prf)
            pk_seed = hex_to_bytes(pk_seed)
            pk_root = hex_to_bytes(pk_root)
            mlen = int(mlen)
            msg = hex_to_bytes(msg)
            opt = hex_to_bytes(opt)
            sig = hex_to_bytes(sig)

            pkey = pk_seed + pk_root
            skey = sk_seed + sk_prf + pkey

            keys = spx.sphincs_shake_256s_robust_keygen(sk_seed, sk_prf, pk_seed)
            sig_ = spx.sphincs_shake_256s_robust_sign(msg, mlen, keys[0], opt)
            verified = spx.sphincs_shake_256s_robust_verify(msg, mlen, sig_, keys[1])

            assert verified, f"[{alg}] Failed to verify signature"
            assert pkey == keys[1], f"[{alg}] Public key doesn't match"
            assert skey == keys[0], f"[{alg}] Secret key doesn't match"
            assert sig_ == sig, f"[{alg}] Signature doesn't match"

            fd.readline()


def test_sphincs_shake_256s_simple():
    """
    Ensures functional correctness and compatibility of SPHINCS+-SHAKE-256s-simple DSA implementation,
    against original reference implementation https://github.com/sphincs/sphincsplus

    Known Answer Tests generated using https://gist.github.com/itzmeanjan/d483872509b8a1a7c4d6614ec9d43e6c
    """

    alg = "SPHINCS+-SHAKE-256s-simple"

    with open("sphincs-shake-256s-simple.kat", "r") as fd:
        while True:
            sk_seed = fd.readline()
            if not sk_seed:
                # no more KAT
                break

            sk_prf = fd.readline()
            pk_seed = fd.readline()
            pk_root = fd.readline()
            mlen = fd.readline()
            msg = fd.readline()
            opt = fd.readline()
            sig = fd.readline()

            # extract out required fields
            sk_seed = extract_out_value(sk_seed)
            sk_prf = extract_out_value(sk_prf)
            pk_seed = extract_out_value(pk_seed)
            pk_root = extract_out_value(pk_root)
            mlen = extract_out_value(mlen)
            msg = extract_out_value(msg)
            opt = extract_out_value(opt)
            sig = extract_out_value(sig)

            # convert input hex strings to bytes object
            sk_seed = hex_to_bytes(sk_seed)
            sk_prf = hex_to_bytes(sk_prf)
            pk_seed = hex_to_bytes(pk_seed)
            pk_root = hex_to_bytes(pk_root)
            mlen = int(mlen)
            msg = hex_to_bytes(msg)
            opt = hex_to_bytes(opt)
            sig = hex_to_bytes(sig)

            pkey = pk_seed + pk_root
            skey = sk_seed + sk_prf + pkey

            keys = spx.sphincs_shake_256s_simple_keygen(sk_seed, sk_prf, pk_seed)
            sig_ = spx.sphincs_shake_256s_simple_sign(msg, mlen, keys[0], opt)
            verified = spx.sphincs_shake_256s_simple_verify(msg, mlen, sig_, keys[1])

            assert verified, f"[{alg}] Failed to verify signature"
            assert pkey == keys[1], f"[{alg}] Public key doesn't match"
            assert skey == keys[0], f"[{alg}] Secret key doesn't match"
            assert sig_ == sig, f"[{alg}] Signature doesn't match"

            fd.readline()


def test_sphincs_shake_256f_robust():
    """
    Ensures functional correctness and compatibility of SPHINCS+-SHAKE-256f-robust DSA implementation,
    against original reference implementation https://github.com/sphincs/sphincsplus

    Known Answer Tests generated using https://gist.github.com/itzmeanjan/d483872509b8a1a7c4d6614ec9d43e6c
    """

    alg = "SPHINCS+-SHAKE-256f-robust"

    with open("sphincs-shake-256f-robust.kat", "r") as fd:
        while True:
            sk_seed = fd.readline()
            if not sk_seed:
                # no more KAT
                break

            sk_prf = fd.readline()
            pk_seed = fd.readline()
            pk_root = fd.readline()
            mlen = fd.readline()
            msg = fd.readline()
            opt = fd.readline()
            sig = fd.readline()

            # extract out required fields
            sk_seed = extract_out_value(sk_seed)
            sk_prf = extract_out_value(sk_prf)
            pk_seed = extract_out_value(pk_seed)
            pk_root = extract_out_value(pk_root)
            mlen = extract_out_value(mlen)
            msg = extract_out_value(msg)
            opt = extract_out_value(opt)
            sig = extract_out_value(sig)

            # convert input hex strings to bytes object
            sk_seed = hex_to_bytes(sk_seed)
            sk_prf = hex_to_bytes(sk_prf)
            pk_seed = hex_to_bytes(pk_seed)
            pk_root = hex_to_bytes(pk_root)
            mlen = int(mlen)
            msg = hex_to_bytes(msg)
            opt = hex_to_bytes(opt)
            sig = hex_to_bytes(sig)

            pkey = pk_seed + pk_root
            skey = sk_seed + sk_prf + pkey

            keys = spx.sphincs_shake_256f_robust_keygen(sk_seed, sk_prf, pk_seed)
            sig_ = spx.sphincs_shake_256f_robust_sign(msg, mlen, keys[0], opt)
            verified = spx.sphincs_shake_256f_robust_verify(msg, mlen, sig_, keys[1])

            assert verified, f"[{alg}] Failed to verify signature"
            assert pkey == keys[1], f"[{alg}] Public key doesn't match"
            assert skey == keys[0], f"[{alg}] Secret key doesn't match"
            assert sig_ == sig, f"[{alg}] Signature doesn't match"

            fd.readline()


def test_sphincs_shake_256f_simple():
    """
    Ensures functional correctness and compatibility of SPHINCS+-SHAKE-256f-simple DSA implementation,
    against original reference implementation https://github.com/sphincs/sphincsplus

    Known Answer Tests generated using https://gist.github.com/itzmeanjan/d483872509b8a1a7c4d6614ec9d43e6c
    """

    alg = "SPHINCS+-SHAKE-256f-simple"

    with open("sphincs-shake-256f-simple.kat", "r") as fd:
        while True:
            sk_seed = fd.readline()
            if not sk_seed:
                # no more KAT
                break

            sk_prf = fd.readline()
            pk_seed = fd.readline()
            pk_root = fd.readline()
            mlen = fd.readline()
            msg = fd.readline()
            opt = fd.readline()
            sig = fd.readline()

            # extract out required fields
            sk_seed = extract_out_value(sk_seed)
            sk_prf = extract_out_value(sk_prf)
            pk_seed = extract_out_value(pk_seed)
            pk_root = extract_out_value(pk_root)
            mlen = extract_out_value(mlen)
            msg = extract_out_value(msg)
            opt = extract_out_value(opt)
            sig = extract_out_value(sig)

            # convert input hex strings to bytes object
            sk_seed = hex_to_bytes(sk_seed)
            sk_prf = hex_to_bytes(sk_prf)
            pk_seed = hex_to_bytes(pk_seed)
            pk_root = hex_to_bytes(pk_root)
            mlen = int(mlen)
            msg = hex_to_bytes(msg)
            opt = hex_to_bytes(opt)
            sig = hex_to_bytes(sig)

            pkey = pk_seed + pk_root
            skey = sk_seed + sk_prf + pkey

            keys = spx.sphincs_shake_256f_simple_keygen(sk_seed, sk_prf, pk_seed)
            sig_ = spx.sphincs_shake_256f_simple_sign(msg, mlen, keys[0], opt)
            verified = spx.sphincs_shake_256f_simple_verify(msg, mlen, sig_, keys[1])

            assert verified, f"[{alg}] Failed to verify signature"
            assert pkey == keys[1], f"[{alg}] Public key doesn't match"
            assert skey == keys[0], f"[{alg}] Secret key doesn't match"
            assert sig_ == sig, f"[{alg}] Signature doesn't match"

            fd.readline()


if __name__ == "__main__":
    print("Use `pytest` for running test cases")
