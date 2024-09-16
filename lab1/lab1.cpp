#include <iostream>

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

void test_pow_module(){
    long long a, x, p;
    std::cout << "Print a, x, p: ";
    std::cin >> a >> x >> p;
    long long result = pow_module(a, x, p);
    std::cout << "Result: " << result << std::endl;
}

int main() {
    test_pow_module();
    return 0;
}
