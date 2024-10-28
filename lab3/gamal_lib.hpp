#include "lab1_lib.hpp"
#include "md5.hpp"
#include <tuple>
#include <random>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <iostream>
#include <random>

std::vector<long long> HashToVector(const std::string& hexHash);

long long* HashToArray(const std::string& hexHash);

void saveSignature(long long r, const std::vector<long long>& s);

bool loadAndVerifySignature(long long y, long long g, long long p);

std::string loadMessage(const std::string& filename);

void test_gamal();
