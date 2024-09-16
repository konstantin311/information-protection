#include <iostream>
#include <tuple>

long long pow_module(long long a, long long x, long long p) {
    long long result = 1;
    a = a % p;  
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

void test_gcd(){
    int a, b;

    while(true){
        std::cout << "Enter two positive integers a and b(a >= b): ";
        std::cin >> a >> b;
        if (a <= 0 || b <= 0 || a < b) {
            std::cout << "Error." << std::endl;
        }
        else break;
    }

    int gcd, x, y;
    std::tie(gcd, x, y) = extendedGCD(a, b);

    std::cout << "GCD: " << gcd << std::endl;
    std::cout << "X = " << x << ", Y = " << y << std::endl;
}

void test_pow_module(){
    long long a, x, p;
    std::cout << "Print a, x, p: ";
    std::cin >> a >> x >> p;
    long long result = pow_module(a, x, p);
    std::cout << "Result: " << result << std::endl;
}

int main() {
    test_pow_module();
    std::cout << "---------------------------------------------------" << std::endl;
    test_gcd();
    return 0;
}
