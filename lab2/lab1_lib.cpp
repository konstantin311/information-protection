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