#include <iostream>
#include <tuple>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm> 

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

    std::cout << "Start Evclid:" << std::endl;
    std::cout << "u: (" << u1 << ", " << u2 << ", " << u3 << ")" << std::endl;
    std::cout << "v: (" << v1 << ", " << v2 << ", " << v3 << ")" << std::endl;

    while (v1 != 0) {
        int q = u1 / v1;
        int t1 = u1 % v1;
        int t2 = u2 - q * v2;
        int t3 = u3 - q * v3;

        std::cout << "q: " << q << std::endl;
        std::cout << "t: (" << t1 << ", " << t2 << ", " << t3 << ")" << std::endl;

        u1 = v1; 
        u2 = v2; 
        u3 = v3; 
        v1 = t1; 
        v2 = t2; 
        v3 = t3;

        std::cout << "UPDATE u: (" << u1 << ", " << u2 << ", " << u3 << ")" << std::endl;
        std::cout << "UPDATE v: (" << v1 << ", " << v2 << ", " << v3 << ")" << std::endl;
    }
    return std::make_tuple(u1, u2, u3);
}
 //a = a % p;
void test_gcd() {
    int a, b;

    while(true) {
        std::cout << "Enter two positive integers a and b (a >= b) or enter '0 0' for random generation: ";
        std::cin >> a >> b;

        if (a == 0 && b == 0) {
            a = rand() % (1000000000 - 1000000 + 1) + 1000000;
            b = rand() % (a - 1000000 + 1) + 1000000; // b <= a
            std::cout << "Generated values: a = " << a << ", b = " << b << "\n";
        }
        
        if (a < 1 || b < 1 || a < b) {
            std::cout << "Error. Please enter valid values." << std::endl;
        } else break;
    }

    int gcd, x, y;
    std::tie(gcd, x, y) = extendedGCD(a, b);

    std::cout << "GCD: " << gcd << std::endl;
    std::cout << "X = " << x << ", Y = " << y << std::endl;
}

bool isPrime(int p) {
    if (p <= 1) return false;
    int limit = (int)pow(p,0.5);;
    for (int i = 2; i <= limit; ++i) {
        if (p % i == 0) return false;        
    }
    return true;  
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
    //} while (!isPrime(p));
    } while(testFerma(p,100));
    return p;
}

void test_pow_module() {
    long long a, x, p;
    std::cout << "Print a, x, p or enter '0 0 0' for random generation: ";
    std::cin >> a >> x >> p;

    if (a == 0 && x == 0 && p == 0) {
        p = generateRandomPrime();
        a = rand() % (1000000000 - 1000000 + 1) + 1000000;
        //x = rand() % (1000000000 - 1000000 + 1) + 1000000;
        x = rand() % (p - 2) + 1;
        //p = generateRandomPrime();
        std::cout << "Generated values: a = " << a << ", x = " << x << ", p = " << p << "\n";
    }

    long long result = pow_module(a, x, p);
    std::cout << "Result: " << result << std::endl;
}

void diffieHellman(int g, int p) {
    int a = rand() % (1000000000 - 1000000 + 1) + 1000000; 
    int b = rand() % (1000000000 - 1000000 + 1) + 1000000;

    int publickeyA = pow_module(g, a, p); 
    int publickeyB = pow_module(g, b, p); 

    int sharedKeyA = pow_module(publickeyB, a, p); 
    int sharedKeyB = pow_module(publickeyA, b, p); 

    std::cout << "g: " << g << std::endl;
    std::cout << "p: " << p << std::endl;

    std::cout << "secret key Alice (a): " << a << std::endl;
    std::cout << "secret key Bob (b): " << b << std::endl;

    std::cout << "public key Alice (A): " << g << "^" << a << " mod " << p << " = " << publickeyA << std::endl;
    std::cout << "public key Bob (B): " << g << "^" << b << " mod " << p << " = " << publickeyB << std::endl;

    std::cout << "shared key Alice: " << publickeyB << "^" << a << " mod " << p << " = " << sharedKeyA << std::endl;
    std::cout << "shared key Bob: " << publickeyA << "^" << a << " mod " << p << " = " << sharedKeyB << std::endl;
}



long long gen_g(long long p) {
    long long phi = p - 1; // Функция Эйлера для простого числа P
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
    return -1; // Не найден первообразный корень
}

void test_diffieHellman(){
    long long q = generateRandomPrime();
    long long p = 2*q +1;
    long long g = gen_g(p);
    diffieHellman(g, p);
}

int main() {
    srand(static_cast<unsigned>(time(0))); 
    test_pow_module();
    std::cout << "---------------------------------------------------" << std::endl;
    test_gcd();
    std::cout << "---------------------------------------------------" << std::endl;
    test_diffieHellman();
    return 0;
}
