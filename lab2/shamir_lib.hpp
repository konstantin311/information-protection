#include <tuple>
#include <random>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

std::vector<int> readFileAsNumbers(const std::string& filename);

void writeNumbersToFile(const std::string& filename, const std::vector<int>& data);

int generateSecretKey(int cA, int p);

int test_Shamir();