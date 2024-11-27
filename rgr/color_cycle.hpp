#include "lab1_lib.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>

void processGraphFile(const std::string& filename, std::vector<std::pair<int, char>>& V, std::vector<std::pair<int, int>>& E);

void test_color_cycle();