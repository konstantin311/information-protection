import random
import hashlib
import os

def pow_module(a, x, p):
    result = 1
    if a == 0:
        return 0
    if p == 0:
        return 0
    while x > 0:
        if x & 1:
            result = (result * a) % p
        a = (a * a) % p
        x >>= 1
    return result

def extended_gcd(a, b):
    u1, u2, u3 = a, 1, 0
    v1, v2, v3 = b, 0, 1

    while v1 != 0:
        q = u1 // v1
        t1 = u1 % v1
        t2 = u2 - q * v2
        t3 = u3 - q * v3

        u1, u2, u3 = v1, v2, v3
        v1, v2, v3 = t1, t2, t3

    return u1, u2, u3

def find_coprime(phi):
    while True:
        candidate = random.randint(2, phi - 1)
        gcd, x, y = extended_gcd(candidate, phi)

        if gcd == 1:
            return candidate

def modular_inverse(a, m):
    gcd, x, y = extended_gcd(a, m)
    if gcd != 1:
        raise ValueError("error inversion")
    return (x % m + m) % m

def miller_rabin_test(n, iterations):
    if n <= 1 or n == 4:
        return False
    if n <= 3:
        return True

    s = 0
    r = n - 1
    while r % 2 == 0:
        s += 1
        r //= 2

    for _ in range(iterations):
        a = random.randint(2, n - 2)
        x = pow_module(a, r, n)

        if x == 1 or x == n - 1:
            continue

        for _ in range(1, s):
            x = pow_module(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True

def gen_g(p, q):
    phi = p - 1
    for g in range(2, p):
        is_primitive = True
        for i in range(1, phi):
            if pow_module(g, i, p) == 1:
                is_primitive = False
                break
        if is_primitive:
            return g
    return -1

def generate_prime(bits):
    """Генерирует простое число заданной длины в битах."""
    while True:
        candidate = random.getrandbits(bits)  
        candidate |= (1 << bits - 1) | 1      
        if miller_rabin_test(candidate, 100):  
            return candidate


def generate_rsa_keys(phi):
    while True:
        c = random.randint(2, phi - 1)
        gcd, d, _ = extended_gcd(c, phi)
        if gcd == 1:
            d %= phi
            if d < 0:
                d += phi
            return (c, phi), (d, phi)
        
def generate_random_number(bits):
    return random.getrandbits(bits)

def compute_sha3_hash(n):
    n_bytes = n.to_bytes((n.bit_length() + 7) // 8, byteorder='big')  
    return int(hashlib.sha3_256(n_bytes).hexdigest(), 16)  

def calculate_h(n, N):
    h = compute_sha3_hash(n)
    if h >= N:
        return h % N  
    return h

def test_voting():
    print(f"SERVER")
    q = generate_prime(256)
    while True:
        p = 2 * q + 1
        if miller_rabin_test(p, 100): 
            break
        q = generate_prime(256) 

    print(f"q = {q}")
    print(f"p = {p}")
    N = p * q
    print(f"N = {N}")

    phi = (p - 1) * (q - 1)
    keys = generate_rsa_keys(phi)
    C = keys[0][0]
    D = keys[1][0]
    print(f"phi = {phi}")
    print(f"C = {C}")
    print(f"D = {D}")

    print(f"\nALISA")

    rnd = generate_random_number(128)
    print(f"rnd = {rnd}")

    # Формирование числа v (например, для голосования "Да" = 1, "Нет" = 0 "Воздержался" = 2)
    v = random.choice([0, 1, 2]) 
    print(f"v = {v}")
    v_bits = v  

    n = (rnd << 2) | v  
    print(f"n = {n}")
    h = calculate_h(n, N)
    print(f"Hash h = {h}")
    
    r = find_coprime(N)
    print(f"r = {r}")
    h_ = h * pow_module(r,D,N)
    
    print(f"\nSERVER")
    s_ = pow_module(h_,C,N)
    print(f"s_ = {s_}")

    print(f"\nALISA")
    rneg = modular_inverse(r,N)
    print(f"r^-1 = {rneg}")

    s = s_ * rneg % N
    print(f"s = {s}")
    print(f"\nОтправляем на сервер <n,s>\n")
    proverka = pow_module(s,D,N)
    print(f"proverka = {proverka}")
    h = calculate_h(n, N)
    print(f"Hash h = {h}")

    if proverka == h:
        print("Значения совпадают, все хорошо!")
    else:
        print("Значения не совпадают, все плохо!")


test_voting()
