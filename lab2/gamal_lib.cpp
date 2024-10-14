#include "shamir_lib.hpp"
#include "lab1_lib.hpp"
#include "shamir_lib.hpp"
#include "lab1_lib.hpp"
#include <fstream>
#include <vector>
#include <iostream>
#include <random>

void test_gamal(){
    std::random_device rd;
    std::mt19937 gen(rd());
    long long q = generateRandomPrime();
    long long p;
    do{
        q = generateRandomPrime();
        p = 2*q +1;
    }while(testFerma(p,100));


    std::cout<<"p="<<p<<std::endl;

    long long g = gen_g(p,q);

    std::cout<<"g="<<g<<std::endl;

    int a,k,m,b;

    std::cout<<"m="<<m<<std::endl;

    //BOB
    std::uniform_int_distribution<> distrib(1, p - 1);
    long long x = distrib(gen);
    std::cout<<"x ="<<x<<std::endl;
    long long y = pow_module(g,x,p);


    //1SHAG
    k = rand() % (1000000000 - 1000000 + 1) + 1000000; 
    std::cout<<"k ="<< k << std::endl;

    //2SHAG
    a = pow_module(g,k,p);
    std::cout<<"a ="<<a<<std::endl;
    std::cout<<g << "^" << k << " mod " << p << " = " << a << std::endl;

    std::uniform_int_distribution<> distrib2(1, p);
    m = distrib2(gen);
    std::cout<<"m="<<m<<std::endl;

    if (m >= p) {
        std::cerr << "Error: message must be less than p!" << std::endl;
    }
    b = m * pow_module(y,k,p);
    std::cout<<"b="<<m<<"*"<<y << "^" << k << " mod " << p << " = " << b << std::endl;
    long long m_=b*pow_module(a,p-1-x,p);
    std::cout<<"m_="<<m_<<std::endl;
    std::cout<<"m_="<<b<<"*"<<a << "^" << p <<"- 1 -"<< x<< " mod " << p << " = " << b << std::endl;
}

// Функция для шифрования одного байта (или блока данных) с помощью Эль-Гамаля
std::pair<long long, long long> encrypt_byte(long long byte, long long g, long long y, long long p, long long k) {
    long long a = pow_module(g, k, p);
    long long b = (byte * pow_module(y, k, p)) % p;
    return {a, b};
}

// Функция для дешифрования одного блока данных с помощью Эль-Гамаля
long long decrypt_byte(long long a, long long b, long long p, long long x) {
    long long decrypted_byte = (b * pow_module(a, p - 1 - x, p)) % p;
    return decrypted_byte;
}

// Функция шифрования файла
void encrypt_file(const std::string& input_file, const std::string& output_file, long long g, long long y, long long p) {
    std::ifstream in(input_file, std::ios::binary);
    std::ofstream out(output_file, std::ios::binary);

    if (!in || !out) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    // Генератор случайных чисел для генерации нового k для каждого байта
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, p - 1); // k должен быть в диапазоне [1, p-1]

    char byte;
    while (in.get(byte)) {
        // Генерация нового k для каждого байта
        long long k = distrib(gen);

        // Шифруем один байт (или блок данных)
        auto encrypted_pair = encrypt_byte(static_cast<unsigned char>(byte), g, y, p, k);

        // Записываем зашифрованные данные (a и b) в файл
        out.write(reinterpret_cast<char*>(&encrypted_pair.first), sizeof(long long));
        out.write(reinterpret_cast<char*>(&encrypted_pair.second), sizeof(long long));
    }

    in.close();
    out.close();
}

// Функция дешифрования файла
void decrypt_file(const std::string& input_file, const std::string& output_file, long long p, long long x) {
    std::ifstream in(input_file, std::ios::binary);
    std::ofstream out(output_file, std::ios::binary);

    if (!in || !out) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    long long a, b;
    while (in.read(reinterpret_cast<char*>(&a), sizeof(long long)) && in.read(reinterpret_cast<char*>(&b), sizeof(long long))) {
        // Дешифруем один блок данных
        char decrypted_byte = static_cast<char>(decrypt_byte(a, b, p, x));

        // Записываем расшифрованный байт в файл
        out.put(decrypted_byte);
    }

    in.close();
    out.close();
}

void test_gamal_file_encryption() {
    std::random_device rd;
    std::mt19937 gen(rd());

    // Генерация q и p
    long long q, p;
    do {
        q = generateRandomPrime();
        p = 2 * q + 1;
    } while (!millerRabinTest(p, 100)); 

    std::cout << "p = " << p << std::endl;

    // Генерация g
    long long g = gen_g(p, q);
    std::cout << "g = " << g << std::endl;

    std::uniform_int_distribution<> distrib(1, p - 1);
    long long x = distrib(gen);  // Закрытый ключ
    long long y = pow_module(g, x, p);  // Открытый ключ
    std::cout << "private key (x) = " << x << std::endl;
    std::cout << "public key (y) = " << y << std::endl;

    // Шифрование файла
    std::string input_file = "file.txt"; 
    //std::string encrypted_file1 = "encrypted_gamalll.txt"; 
    std::string encrypted_file = "encrypted_gamal.txt"; 
    encrypt_file(input_file, encrypted_file, g, y, p);

    // Дешифрование файла
    std::string decrypted_file = "decrypted_gamal.txt"; 
    decrypt_file(encrypted_file, decrypted_file, p, x);
}