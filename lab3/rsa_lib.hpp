#include "lab1_lib.hpp"
#include "md5.hpp"
#include <iostream>
#include <fstream>
#include <random>
#include <vector>

// Функция для генерации ключей RSA
std::pair<std::pair<long long, long long>, std::pair<long long, long long>> generate_rsa_keys(long long phi);

void test_rsa();

void test_rsa_signature(const std::string& input_file, const std::string& signature_file);