#include <tuple>
#include <random>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include "shamir_lib.hpp"
#include "lab1_lib.hpp"

void test_gamal();

#include "shamir_lib.hpp"
#include "lab1_lib.hpp"
#include <fstream>
#include <vector>
#include <iostream>
#include <random>

std::pair<long long, long long> encrypt_byte(long long byte, long long g, long long y, long long p, long long k);

long long decrypt_byte(long long a, long long b, long long p, long long x);

void encrypt_file(const std::string& input_file, const std::string& output_file, long long g, long long y, long long p);

void decrypt_file(const std::string& input_file, const std::string& output_file, long long p, long long x);

void test_gamal_file_encryption();
