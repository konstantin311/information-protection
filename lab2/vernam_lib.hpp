#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include "lab1_lib.hpp"
#include "shamir_lib.hpp"

std::vector<int> generateVernamKey(int length);

std::vector<int> vernamCipher(const std::vector<int>& data, const std::vector<int>& key);

int test_Vernam();
