#include "gamal_lib.hpp"
#include "lab1_lib.hpp"
#include "md5.hpp"
#include <fstream>
#include <vector>
#include <iostream>
#include <random>
#include <iomanip>

std::vector<long long> HashToVector(const std::string& hexHash){
    std::vector<long long> res(8);
    std::string binHash = HexToBin(hexHash);
    for (size_t i=0; i<8; ++i){
        res[i] = (std::bitset<16>(binHash.substr(i*16,16)).to_ulong());
    }
    return res;
}

long long* HashToArray(const std::string& hexHash){
    auto* res = new long long[8];
    std::string binHash = HexToBin(hexHash);
    for (size_t i=0; i<8; ++i){
        res[i] = (std::bitset<16>(binHash.substr(i*16,16)).to_ulong());
    }
    return res;
}

void saveSignature(long long r, const std::vector<long long>& s) {
    std::ofstream outFile("signature.txt");
    if (outFile.is_open()) {
        outFile << r << std::endl;
        for (const auto& value : s) {
            outFile << value << " ";
        }
        outFile.close();
    } else {
        std::cerr << "error" << std::endl;
    }
}

void loadSignature(long long& r, std::vector<long long>& s) {
    std::ifstream inFile("signature.txt");
    if (inFile.is_open()) {
        inFile >> r;
        long long value;
        while (inFile >> value) {
            s.push_back(value);
        }
        inFile.close();
    } else {
        std::cerr << "error" << std::endl;
    }
}

std::string loadMessage(const std::string& filename) {
    std::ifstream inFile(filename);
    std::string message;
    if (inFile.is_open()) {
        std::getline(inFile, message);
        inFile.close();
    } else {
        std::cerr << "error" << std::endl;
    }
    return message;
}

void test_gamal() {
    std::random_device rd;
    std::mt19937 gen(rd());
    long long q; 
    long long p;
    do {
        do {
            q = generateRandomPrime();
        } while (!millerRabinTest(q, 100));
        p = 2 * q + 1;
    } while (!millerRabinTest(p, 100));

    long long g = gen_g(p, q);
    std::cout << "q = " << q << std::endl;
    std::cout << "p = " << p << std::endl;
    std::cout << "g = " << g << std::endl;

    std::uniform_int_distribution<> distrib(1, p - 1);
    long long x = distrib(gen);
    std::cout << "x = " << x << std::endl;
    long long y = pow_module(g, x, p);
    std::cout << "y = " << y << std::endl;

    long long k = findCoprime(p - 1);
    std::cout << "k = " << k << std::endl;

    long long r = pow_module(g, k, p);
    std::cout << "r = " << r << std::endl;
    std::cout << g << "^" << k << " mod " << p << " = " << r << std::endl;

    // Load the message from a file
    std::string document = loadMessage("file.txt");
    std::string hashHex = md5(document);
    std::vector<long long> hash_vt = HashToVector(hashHex);

    for (const long long& value : hash_vt) {
        std::cout << value << " ";
    }

    std::vector<long long> s;
    long long negK = modularInverse(k, p - 1);
    std::cout << "\nnegK = " << negK << std::endl;

    for (auto &it : hash_vt) {
        long long u = mod(it - x * r, p - 1);
        s.push_back(((negK % (p - 1)) * (u % (p - 1))) % (p - 1));
    }

    // Save the signature (r, s)
    saveSignature(r, s);

    std::vector<long long> left;
    std::vector<long long> right;
    for (auto &it : hash_vt) {   
        right.push_back(pow_module(g, it, p));
    }
    std::cout << std::endl;

    for (auto &it : s) {   
        left.push_back(((pow_module(y, r, p) % p) * (pow_module(r, it, p) % p) % p));
    }

    if (right == left) {
        std::cout << "good!" << std::endl;
    } else {
        std::cout << "bad!" << std::endl;
    }

    // Load the signature to verify
    long long loadedR;
    std::vector<long long> loadedS;
    loadSignature(loadedR, loadedS);

    // Verify the signature
    std::cout << "Loaded r: " << loadedR << std::endl;
    std::cout << "Loaded s: ";
    for (const auto& value : loadedS) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    std::vector<long long> loadedLeft;
    for (auto &it : loadedS) {   
        loadedLeft.push_back(((pow_module(y, loadedR, p) % p) * (pow_module(loadedR, it, p) % p) % p));
    }

    if (right == loadedLeft) {
        std::cout << "Loaded signature is valid!" << std::endl;
    } else {
        std::cout << "Loaded signature is invalid!" << std::endl;
    }
}
