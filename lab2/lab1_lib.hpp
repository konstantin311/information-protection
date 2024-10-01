#pragma once
#include <iostream>
#include <tuple>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm> 
#include <unordered_map>

long long pow_module(long long a, long long x, long long p);

std::tuple<int, int, int> extendedGCD(int a, int b);

bool testFerma(long long p, int k);

int generateRandomPrime();