#include "rsa_lib.hpp"
#include "lab1_lib.hpp"
#include <iostream>
#include <fstream>
#include <random>
#include <vector>

// Функция для шифрования одного байта с помощью RSA
long long rsa_encrypt_byte(long long byte, long long e, long long N) {
    return pow_module(byte, e, N);
}

// Функция для дешифрования одного байта с помощью RSA
long long rsa_decrypt_byte(long long byte, long long d, long long N) {
    return pow_module(byte, d, N);
}

// Функция шифрования файла с помощью RSA
void rsa_encrypt_file(const std::string& input_file, const std::string& output_file, long long e, long long N) {
    std::ifstream in(input_file, std::ios::binary);
    std::ofstream out(output_file, std::ios::binary);

    if (!in || !out) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    char byte;
    while (in.get(byte)) {
        long long encrypted_byte = rsa_encrypt_byte(static_cast<unsigned char>(byte), e, N);
        out.write(reinterpret_cast<char*>(&encrypted_byte), sizeof(long long));
    }

    in.close();
    out.close();
}

// Функция дешифрования файла с помощью RSA
void rsa_decrypt_file(const std::string& input_file, const std::string& output_file, long long d, long long N) {
    std::ifstream in(input_file, std::ios::binary);
    std::ofstream out(output_file, std::ios::binary);

    if (!in || !out) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    long long encrypted_byte;
    while (in.read(reinterpret_cast<char*>(&encrypted_byte), sizeof(long long))) {
        char decrypted_byte = static_cast<char>(rsa_decrypt_byte(encrypted_byte, d, N));
        out.put(decrypted_byte);
    }

    in.close();
    out.close();
}

// Функция для генерации ключей RSA
std::pair<std::pair<long long, long long>, std::pair<long long, long long>> generate_rsa_keys(long long phi) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(2, phi - 1);

    while (true) {
        long long e = distrib(gen);
        long long gcd, d, _;
        std::tie(gcd, d, _) = extendedGCD(e, phi);
        if (gcd == 1) {
            d %= phi;
            if (d < 0) {
                d += phi;
            }
            return std::make_pair(std::make_pair(e, phi), std::make_pair(d, phi)); // (e, phi) и (d, phi)
        }
    }
}

// Тестирование RSA с шифрованием и дешифрованием файла
void test_rsa_file_encryption() {
    long long N, e, d, p, q;

    // Генерация случайных простых чисел p и q
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(100, 10000);

    do {
        p = dist(gen);
    } while (!millerRabinTest(p, 100));
    std::cout << "Generated prime number p: " << p << std::endl;

    do {
        q = dist(gen);
    } while (!millerRabinTest(q, 100));
    std::cout << "Generated prime number q: " << q << std::endl;

    // Вычисление N и phi
    N = p * q;
    long long phi = (p - 1) * (q - 1);
    std::cout << "phi = " << phi << std::endl;

    // Генерация ключей RSA
    auto keys = generate_rsa_keys(phi);
    e = keys.first.first;
    d = keys.second.first;

    // Вывод ключей
    std::cout << "Generated keys:" << std::endl;
    std::cout << "Public key (N, e): (" << N << ", " << e << ")" << std::endl;
    std::cout << "Private key (d): " << d << std::endl;

    // Шифрование файла
    std::string input_file = "file.txt";
    std::string encrypted_file = "encrypted_rsa.txt";
    rsa_encrypt_file(input_file, encrypted_file, e, N);

    // Дешифрование файла
    std::string decrypted_file = "decrypted_rsa.txt";
    rsa_decrypt_file(encrypted_file, decrypted_file, d, N);
}

void test_rsa() {
    long long N, e, d, p, q, c;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(100, 10000);

    do {
        p = dist(gen);
    } while (!millerRabinTest(p, 100)); 
    std::cout << "Generated prime number p: " << p << std::endl;

    do {
        q = dist(gen);
    } while (!millerRabinTest(q, 100));
    std::cout << "Generated prime number q: " << q << std::endl;

    N = p * q;
    long long phi = (p - 1) * (q - 1);
    std::cout << "phi = " << phi << std::endl;

    auto keys = generate_rsa_keys(phi);
    c = keys.first.first; 
    d = keys.second.first; 

  
    std::cout << "Generated keys:" << std::endl;
    std::cout << "Public key (N, c): (" << N << ", " << c << ")" << std::endl; 
    std::cout << "Private key (d): " << d << std::endl; 


    int message;
    std::cout << "Enter the message (number < N): ";
    
    std::cin >> message;


    long long encryptedMessage = (pow_module(message, c, N));
    std::cout << "Encrypted message: " << encryptedMessage << std::endl;


    long long decryptedMessage = (pow_module(encryptedMessage, d, N));
    std::cout << "The decrypted message: " << decryptedMessage << std::endl;
}


