#include "lab1_lib.hpp"
#include "md5.hpp"
#include "gamal_lib.hpp"
#include <iostream>
#include <fstream>
#include <random>
#include <vector>

std::pair<std::pair<long long, long long>, std::pair<long long, long long>> generate_rsa_keys(long long phi);

void save_signatures_to_file(const std::vector<long long>& signatures, const std::string& filename);

void verify_rsa_signature(long long N, long long d);

void test_rsa_signature();

