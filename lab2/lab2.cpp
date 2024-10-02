#include <iostream>
#include <tuple>
#include <random>
#include <algorithm>
#include "lab1_lib.hpp"

int generateSecretKey(int cA, int p) {
    int d, gcd, x, y;
    std::tie(gcd, x, y) = extendedGCD(cA, p - 1);

    if (gcd != 1) {
        std::cerr << "Error: cA and p-1 are not relatively prime!" << std::endl;
        return -1;
    } else {
        d = (x % (p - 1) + (p - 1)) % (p - 1); // Convert to positive value
        return d;
    }
}


int main() {
    // Initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // 1. Generate a random prime number p
    int p;
    std::cout << "Enter prime number p (or 0 to generate a random prime): ";
    std::cin >> p;

    if (p == 0) {
        p = generateRandomPrime();
        std::cout << "Generated prime number p: " << p << std::endl;
    }

    // 2. Generate secret key cA
    int cA;
    std::cout << "Enter secret key A (cA) (or 0 to generate a random key): ";
    std::cin >> cA;

    if (cA == 0) {
         cA = rand() % (p - 1) + 1; // Random number from 1 to p-1
        std::cout << "Generated secret key A (cA): " << cA << std::endl;
    }

    // 3. Calculate dA as modular inverse of cA mod (p-1)
    int dA = generateSecretKey(cA, p);
    std::cout << "Secret key A (dA): " << dA << std::endl;

    // 4. Generate secret keys B
    int cB;
    std::cout << "Enter secret key B (cB) (or 0 to generate a random key): ";
    std::cin >> cB;

    if (cB == 0) {
        cB = rand() % (p - 1) + 1; // Random number from 1 to p-1
        std::cout << "Generated secret key B (cB): " << cB << std::endl;
    }

    int dB = generateSecretKey(cB, p);
    std::cout << "Secret key B (dB): " << dB << std::endl;

    // 5. Input the message
    int m;
    std::cout << "Enter the message m: ";
    std::cin >> m;

    // 6. Check the message (m < p)
    if (m >= p) {
        std::cerr << "Message m must be less than p!" << std::endl;
        return 1;
    }

    // 7. Encryption
    int x1 = pow_module(m, cA, p);

    std::cout << "x1: " <<m<<"^"<<cA<<"mod "<<p<<" = "<< x1 << std::endl;

    // 8. Transmit x1 to B
    // ... (simulation of transmission)

    // 9. Decryption by B
    int x2 = pow_module(x1, cB, p);
    std::cout << "x2: " << x2 << std::endl;

    // 10. Transmit x2 to A
    // ... (simulation of transmission)

    // 11. Decryption by A
    int x3 = pow_module(x2, dA, p);
    std::cout << "x3: " << x3 << std::endl;

    // 12. Transmit x3 to B
    // ... (simulation of transmission)

    // 13. Decryption by B
    int x4 = pow_module(x3, dB, p);
    std::cout << "x4: " << x4 << std::endl;

    if (x4 == m) {
        std::cout << "Message successfully decrypted!" << std::endl;
    } else {
        std::cerr << "Decryption error!" << std::endl;
    }

    return 0;
}