#include "lab1_lib.hpp"

long long pow_module(long long a, long long x, long long p) {
    long long result = 1;
     
    if (a == 0) return 0; 
    if (p == 0 ) return 0;
    while (x > 0) {
        if (x & 1) {
            result = (result * a) % p;
        }
        a = (a * a) % p;
        x >>= 1;
    }
    return result;
}



std::tuple<int, int, int> extendedGCD(int a, int b) {
    int u1 = a, u2 = 1, u3 = 0;
    int v1 = b, v2 = 0, v3 = 1;

    /*std::cout << "Start Evclid:" << std::endl;
    std::cout << "u: (" << u1 << ", " << u2 << ", " << u3 << ")" << std::endl;
    std::cout << "v: (" << v1 << ", " << v2 << ", " << v3 << ")" << std::endl;*/

    while (v1 != 0) {
        int q = u1 / v1;
        int t1 = u1 % v1;
        int t2 = u2 - q * v2;
        int t3 = u3 - q * v3;

        /*std::cout << "q: " << q << std::endl;
        std::cout << "t: (" << t1 << ", " << t2 << ", " << t3 << ")" << std::endl;*/

        u1 = v1; 
        u2 = v2; 
        u3 = v3; 
        v1 = t1; 
        v2 = t2; 
        v3 = t3;

        /*std::cout << "UPDATE u: (" << u1 << ", " << u2 << ", " << u3 << ")" << std::endl;
        std::cout << "UPDATE v: (" << v1 << ", " << v2 << ", " << v3 << ")" << std::endl;*/
    }
    return std::make_tuple(u1, u2, u3);
}

long long mod(long long a, long long b)
{
    if (b < 0)
        return -mod(-a, -b);
    int ret = a % b;
    if (ret < 0)
        ret += b;
    return ret;
}

int findCoprime(int phi) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(2, phi - 1); 

    while (true) {
        int candidate = distrib(gen);
        int gcd, x, y;
        std::tie(gcd, x, y) = extendedGCD(candidate, phi);

        if (gcd == 1) { 
            return candidate;
        }
    }
}

int modularInverse(int a, int m) {
    auto [gcd, x, y] = extendedGCD(a, m);
    if (gcd != 1) {
        throw std::invalid_argument("error inversion");
    }
    return (x % m + m) % m;
}

bool millerRabinTest(long long n, int iterations) {
    if (n <= 1 || n == 4) return false;
    if (n <= 3) return true;

    // Найдем  s  и  r  такие, что  n - 1 = 2^s * r  и  r  нечетное
    long long s = 0;
    long long r = n - 1;
    while (r % 2 == 0) {
        s++;
        r /= 2;
    }

    for (int i = 0; i < iterations; i++) {
        long long a = rand() % (n - 2) + 2;
        long long x = pow_module(a, r, n);

        if (x == 1 || x == n - 1) continue;

        for (int j = 1; j < s; j++) {
            x = pow_module(x, 2, n);
            if (x == n - 1) continue;
        }
        return false; // Не проходит тест Миллера-Рабина
    }
    return true; // Вероятно, простое
}

long long gen_g(long long p, long long q) {
    long long phi = p - 1; 
    for (long long g = 2; g < p; ++g) {
        bool isPrimitive = true;
        for (long long i = 1; i < phi; ++i) {
            if (pow_module(g, i, p) == 1) {
                isPrimitive = false;
                break;
            }
        }
        if (isPrimitive) {
            return g;
        }
    }
    return -1; 
}

bool testFerma(long long p, int k)
{
  if(p==2) return true;
  if(p&1) return false;
  for(int i=0; i<k; ++i)
  {
    long long a=rand()%(p-1)+1;
    if(std::__gcd(a,p)!=1 || pow_module(a,p-1,p)!=1)
      return false;
  }
  return true;
}

int generateRandomPrime() {
    int p;
    do {
        p = rand() % (1000000000 - 1000000 + 1) + 1000000; 
    } while(testFerma(p,100));
    return p;
}

std::pair<std::pair<long long, long long>, std::pair<long long, long long>> generate_rsa_keys(long long phi) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(2, phi - 1);

    while (true) {
        long long c = distrib(gen);
        long long gcd, d, _;
        std::tie(gcd, d, _) = extendedGCD(c, phi);
        if (gcd == 1) {
            d %= phi;
            if (d < 0) {
                d += phi;
            }
            return std::make_pair(std::make_pair(c, phi), std::make_pair(d, phi)); 
        }
    }
}