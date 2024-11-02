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

void saveSignatureGost(long long r, const std::vector<long long>& s);

bool loadAndVerifySignatureGost(long long y, long long g, long long p, long long q, long long a);

void test_gost();
