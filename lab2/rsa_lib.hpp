#include "lab1_lib.hpp"
#include <iostream>
#include <fstream>
#include <random>
#include <vector>

long long rsa_encrypt_byte(long long byte, long long e, long long N);

long long rsa_decrypt_byte(long long byte, long long d, long long N);

// Функция шифрования файла с помощью RSA
void rsa_encrypt_file(const std::string& input_file, const std::string& output_file, long long e, long long N);
// Функция дешифрования файла с помощью RSA
void rsa_decrypt_file(const std::string& input_file, const std::string& output_file, long long d, long long N);
// Функция для генерации ключей RSA
std::pair<std::pair<long long, long long>, std::pair<long long, long long>> generate_rsa_keys(long long phi);

void test_rsa_file_encryption();

void test_rsa();