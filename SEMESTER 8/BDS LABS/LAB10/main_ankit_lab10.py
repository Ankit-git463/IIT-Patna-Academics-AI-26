"""
Paillier Homomorphic Encryption System
"""

import random
import math
import time
import sys


# ─────────────────────────────────────────
#  Math utilities
# ─────────────────────────────────────────

def is_prime_miller_rabin(n: int, k: int = 20) -> bool:
    if n < 2:
        return False
    if n in (2, 3):
        return True
    if n % 2 == 0:
        return False

    r, d = 0, n - 1
    while d % 2 == 0:
        r += 1
        d //= 2

    witnesses = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37]
    for a in witnesses:
        if a >= n:
            continue
        x = pow(a, d, n)
        if x in (1, n - 1):
            continue
        for _ in range(r - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True


def generate_prime(bits: int) -> int:
    while True:
        p = random.getrandbits(bits) | (1 << (bits - 1)) | 1
        if is_prime_miller_rabin(p):
            return p


def lcm(a: int, b: int) -> int:
    return a * b // math.gcd(a, b)


def mod_inverse(a: int, m: int) -> int:
    g, x, _ = extended_gcd(a, m)
    if g != 1:
        raise ValueError(f"No modular inverse: gcd({a}, {m}) = {g}")
    return x % m


def extended_gcd(a: int, b: int):
    if a == 0:
        return b, 0, 1
    g, x, y = extended_gcd(b % a, a)
    return g, y - (b // a) * x, x


def L(u: int, n: int) -> int:
    return (u - 1) // n


def fmt_big(n: int, chars: int = 48) -> str:
    s = str(n)
    if len(s) <= chars:
        return s
    return s[:chars] + f"... [{len(s)} digits]"


def fmt_ms(t: float) -> str:
    ms = t * 1000
    return f"{ms:.1f}ms"


# ─────────────────────────────────────────
#  Paillier Cryptosystem
# ─────────────────────────────────────────

class Paillier:

    @staticmethod
    def generate_keys(key_size: int = 2048):
        """
        Generate Paillier public/private key pair.
        Public key : (n, g)
        Private key: (lambda_, mu)
        """
        half = key_size // 2

        # Generate two distinct primes p and q
        while True:
            p = generate_prime(half)
            q = generate_prime(half)
            if p != q:
                break

        n = p * q
        n2 = n * n

        # g = n + 1 is the standard, efficient choice
        g = n + 1

        # Private key components
        lambda_ = lcm(p - 1, q - 1)
        mu = mod_inverse(L(pow(g, lambda_, n2), n), n)

        public_key  = (n, g)
        private_key = (lambda_, mu, n)   # n carried for convenience
        return public_key, private_key

    @staticmethod
    def encrypt(public_key: tuple, m: int) -> int:
        """
        Encrypt plaintext m ∈ Z_n.
        Ciphertext = g^m · r^n  mod n²
        """
        n, g = public_key
        if not (0 <= m < n):
            raise ValueError(f"Plaintext {m} out of range [0, n)")

        n2 = n * n

        # Choose random r coprime to n
        while True:
            r = random.randint(2, n - 1)
            if math.gcd(r, n) == 1:
                break

        c = pow(g, m, n2) * pow(r, n, n2) % n2
        return c

    @staticmethod
    def decrypt(private_key: tuple, c: int) -> int:
        """
        Decrypt ciphertext c.
        m = L(c^lambda mod n²) · mu  mod n
        """
        lambda_, mu, n = private_key
        n2 = n * n
        m = L(pow(c, lambda_, n2), n) * mu % n
        return m

    @staticmethod
    def add(public_key: tuple, c1: int, c2: int) -> int:
        """
        Homomorphic addition: Enc(a) ⊕ Enc(b) = Enc(a + b mod n)
        Implementation: c1 * c2 mod n²
        """
        n, _ = public_key
        return c1 * c2 % (n * n)

    @staticmethod
    def multiply(public_key: tuple, c: int, k: int) -> int:
        """
        Homomorphic scalar multiplication: k ⊗ Enc(a) = Enc(k·a mod n)
        Implementation: c^k mod n²
        """
        n, _ = public_key
        return pow(c, k, n * n)


# ─────────────────────────────────────────
#  Demo helpers
# ─────────────────────────────────────────

def section(title: str):
    input(f"\nPress Enter to Proceed - {title} ")
    width = 60
    print()
    print("-" * width)
    print(f"  \t{title}")
    print("-" * width)


def timed_encrypt(pk, m, label):
    t0 = time.perf_counter()
    c = Paillier.encrypt(pk, m)
    elapsed = time.perf_counter() - t0
    print(f"  Encrypting {label}: E({m})")
    print(f"    Ciphertext : {fmt_big(c)}")
    print(f"    Time       : {fmt_ms(elapsed)}")
    return c, elapsed


def timed_decrypt(sk, c, label=""):
    t0 = time.perf_counter()
    m = Paillier.decrypt(sk, c)
    elapsed = time.perf_counter() - t0
    if label:
        print(f"    Dec({label}) = {m}  [{fmt_ms(elapsed)}]")
    return m, elapsed


# ─────────────────────────────────────────
#  Main demo
# ─────────────────────────────────────────

def run(a: int, b: int, c: int, k: int, key_size: int):


    print()
    print("╔══════════════════════════════════════════════════════════╗")
    print("║        PAILLIER HOMOMORPHIC ENCRYPTION                   ║")
    print("╚══════════════════════════════════════════════════════════╝")

    # ── Key Generation ──────────────────────────────────────────
    section("KEY GENERATION")
    print(f"  Generating Paillier keys ({key_size}-bit)...")
    t0 = time.perf_counter()
    public_key, private_key = Paillier.generate_keys(key_size)
    kg_time = time.perf_counter() - t0

    n, g = public_key
    print(f"  Public key  (n, g):")
    print(f"    n = {fmt_big(n)}")
    print(f"    g = n + 1  [standard efficient choice]")
    print(f"  Private key : (λ, μ) — kept secret")
    print(f"  Key gen time: {fmt_ms(kg_time)}")

    # ── Encryption ──────────────────────────────────────────────
    section("ENCRYPTION")
    print(f"  Plaintext numbers: a={a}, b={b}, c={c}\n")

    enc_times = []
    ea, t = timed_encrypt(public_key, a, "a")
    enc_times.append(t)
    print()
    eb, t = timed_encrypt(public_key, b, "b")
    enc_times.append(t)
    print()
    ec, t = timed_encrypt(public_key, c, "c")
    enc_times.append(t)

    # ── Homomorphic Addition ─────────────────────────────────────
    section("HOMOMORPHIC ADDITION")
    print(f"  Operation : E({a}) ⊕ E({b}) = E({a} + {b}) = E({a+b})")
    print(f"  Method    : c_sum = E({a}) × E({b}) mod n²")

    t0 = time.perf_counter()
    c_sum = Paillier.add(public_key, ea, eb)
    add_time = time.perf_counter() - t0

    print(f"  Time      : {fmt_ms(add_time)}")
    print(f"  Result    : {fmt_big(c_sum)}")

    result_add, dec_t = timed_decrypt(private_key, c_sum, "c_sum")
    expected_add = (a + b) % n
    ok = "✓" if result_add == expected_add else "✗"
    print(f"\n  Verification : Dec(c_sum) = {result_add}  {ok}")
    print(f"  Expected     : {a} + {b} = {a+b}  {ok}")

    # ── Homomorphic Scalar Multiplication ────────────────────────
    section("HOMOMORPHIC MULTIPLICATION BY CONSTANT")
    print(f"  Operation : {k} ⊗ E({b}) = E({k} × {b}) = E({k*b})")
    print(f"  Method    : c_prod = E({b})^{k} mod n²")

    t0 = time.perf_counter()
    c_prod = Paillier.multiply(public_key, eb, k)
    mul_time = time.perf_counter() - t0

    print(f"  Time      : {fmt_ms(mul_time)}")
    print(f"  Result    : {fmt_big(c_prod)}")

    result_mul, dec_t2 = timed_decrypt(private_key, c_prod, "c_prod")
    expected_mul = (k * b) % n
    ok = "✓" if result_mul == expected_mul else "✗"
    print(f"\n  Verification : Dec(c_prod) = {result_mul}  {ok}")
    print(f"  Expected     : {k} × {b} = {k*b}  {ok}")

    # ── Complex Expression ───────────────────────────────────────
    section("COMPLEX EXPRESSION")
    expr_val = (a + b) * k + c
    print(f"  Expression: (a + b) × {k} + c")
    print(f"  = ({a} + {b}) × {k} + {c} = {expr_val}\n")

    print(f"  Step 1: E(a + b) = E({a}) ⊕ E({b}) = E({a+b})")
    e_ab = Paillier.add(public_key, ea, eb)

    print(f"  Step 2: E((a+b) × {k}) = {k} ⊗ E({a+b}) = E({(a+b)*k})")
    e_ab_k = Paillier.multiply(public_key, e_ab, k)

    print(f"  Step 3: E(result) = E({(a+b)*k}) ⊕ E({c}) = E({expr_val})")
    e_final = Paillier.add(public_key, e_ab_k, ec)

    print(f"\n  All operations performed on ENCRYPTED data!")
    result_final, _ = timed_decrypt(private_key, e_final, "e_final")
    ok = "✓" if result_final == expr_val % n else "✗"
    print(f"\n  Final decryption : {result_final}  {ok}")
    print(f"  Verification     : ({a} + {b}) × {k} + {c} = {expr_val}  {ok}")

    # ── Performance Metrics ──────────────────────────────────────
    section("PERFORMANCE METRICS")
    avg_enc = sum(enc_times) / len(enc_times)
    avg_dec = (dec_t + dec_t2) / 2
    cipher_bits = ea.bit_length()

    print(f"  Key generation         : {fmt_ms(kg_time)}")
    print(f"  Encryption (avg)       : {fmt_ms(avg_enc)}")
    print(f"  Decryption (avg)       : {fmt_ms(avg_dec)}")
    print(f"  Homomorphic addition   : {fmt_ms(add_time)}")
    print(f"  Homomorphic multiply   : {fmt_ms(mul_time)}")
    print(f"  Ciphertext size        : {cipher_bits} bits  ({cipher_bits // 8} bytes)")
    print(f"  Key size               : {key_size}-bit")
    print(f"  Security               : IND-CPA (semantically secure)")
    
    print()


# ─────────────────────────────────────────
#  Entry point
# ─────────────────────────────────────────

def get_int(prompt: str, default: int, lo: int = 0, hi: int = None) -> int:
    try:
        raw = input(f"  {prompt} [ suggestion - {default} ]: ").strip()
        val = int(raw) if raw else default
    except (ValueError, EOFError):
        val = default
    if lo is not None and val < lo:
        print(f"    → Value too small, using {lo}")
        val = lo
    if hi is not None and val > hi:
        print(f"    → Value too large, using {hi}")
        val = hi
    return val


def main():
    print()
    print("  ┌─ Input parameters ──────────────────────────────────┐")
    print("  │  Press Enter to accept the default shown in [..]    │")
    print("  └─────────────────────────────────────────────────────┘")
    print()
    user = input("Enter UserName : ")
    print()

    a        = get_int("Plaintext  a",  42,  0, 10_000)
    b        = get_int("Plaintext  b",  17,  0, 10_000)
    c        = get_int("Plaintext  c", 100,  0, 10_000)
    k        = get_int("Constant   k",   3,  1,   500)
    key_size = get_int("Key size (bits, multiples of 64 recommended)", 512, 64, 4096)

    # Snap to nearest even number (need half-size primes)
    if key_size % 2 != 0:
        key_size += 1
        print(f"    → Rounded key size to {key_size}")

    run(a, b, c, k, key_size)


if __name__ == "__main__":
    main()
