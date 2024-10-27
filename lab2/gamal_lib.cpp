#include "shamir_lib.hpp"
#include "lab1_lib.hpp"
#include "shamir_lib.hpp"
#include "lab1_lib.hpp"
#include <fstream>
#include <vector>
#include <iostream>
#include <random>


std::pair<long long, long long> encrypt_byte(long long byte, long long g, long long y, long long p, long long k) {
    long long a = pow_module(g, k, p);
    long long b = (byte * pow_module(y, k, p));// % p;
    return {a, b};
}

long long decrypt_byte(long long a, long long b, long long p, long long x) {
    long long decrypted_byte = (b * pow_module(a, p - 1 - x, p));// % p;
    return decrypted_byte;
}

void encrypt_file(const std::string& input_file, const std::string& output_file, long long g, long long y, long long p) {
    std::ifstream in(input_file, std::ios::binary);
    std::ofstream out(output_file, std::ios::binary);

    if (!in || !out) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, p - 1); 

    char byte;
    while (in.get(byte)) {
        
        long long k = distrib(gen);

        
        auto encrypted_pair = encrypt_byte(static_cast<unsigned char>(byte), g, y, p, k);

        out.write(reinterpret_cast<char*>(&encrypted_pair.first), sizeof(long long));
        out.write(reinterpret_cast<char*>(&encrypted_pair.second), sizeof(long long));
    }

    in.close();
    out.close();
}

void decrypt_file(const std::string& input_file, const std::string& output_file, long long p, long long x) {
    std::ifstream in(input_file, std::ios::binary);
    std::ofstream out(output_file, std::ios::binary);

    if (!in || !out) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    long long a, b;
    while (in.read(reinterpret_cast<char*>(&a), sizeof(long long)) && in.read(reinterpret_cast<char*>(&b), sizeof(long long))) {
        char decrypted_byte = static_cast<char>(decrypt_byte(a, b, p, x));

        out.put(decrypted_byte);
    }

    in.close();
    out.close();
}

void test_gamal_file_encryption() {
    std::random_device rd;
    std::mt19937 gen(rd());

    long long q, p;
    do {
        q = generateRandomPrime();
        p = 2 * q + 1;
    } while (!millerRabinTest(p, 100)); 

    std::cout << "p = " << p << std::endl;

    long long g = gen_g(p, q);
    std::cout << "g = " << g << std::endl;

    std::uniform_int_distribution<> distrib(1, p - 1);
    long long x = distrib(gen);  // Закрытый ключ
    long long y = pow_module(g, x, p);  // Открытый ключ
    std::cout << "private key (x) = " << x << std::endl;
    std::cout << "public key (y) = " << y << std::endl;


    std::string input_file = "file.txt"; 
    //std::string encrypted_file1 = "encrypted_gamalll.txt"; 
    std::string encrypted_file = "encrypted_gamal.txt"; 
    encrypt_file(input_file, encrypted_file, g, y, p);

    std::string decrypted_file = "decrypted_gamal.txt"; 
    decrypt_file(encrypted_file, decrypted_file, p, x);
}