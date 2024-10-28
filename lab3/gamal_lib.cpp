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
    std::ofstream outFile("signature_gamal.txt");
    if (outFile.is_open()) {
        outFile << r << std::endl; 
        for (const auto& value : s) { 
            outFile << value << " ";
        }
        outFile.close();
    } else {
        std::cerr << "Error: could not open signature.txt" << std::endl;
    }
}

bool loadAndVerifySignature(long long y, long long g, long long p) {
    std::ifstream inFile("signature_gamal.txt");
    long long r;
    std::vector<long long> s;

    if (inFile.is_open()) {
        if (!(inFile >> r)) {
            std::cerr << "Error reading r from signature.txt" << std::endl;
            return false;
        }

        long long value;
        while (inFile >> value) {
            s.push_back(value);
        }
        inFile.close();

        if (s.empty()) {
            std::cerr << "Error: no values loaded into s" << std::endl;
            return false;
        }
    } else {
        std::cerr << "Error: could not open signature.txt" << std::endl;
        return false;
    }

    // Чтение исходного текста из file.txt
    std::string document = loadMessage("signature_rsa.txt");

    std::string hashHex = md5(document);
    std::vector<long long> hash_vt = HashToVector(hashHex);

    std::vector<long long> left;
    std::vector<long long> right;

    for (size_t i = 0; i < s.size(); ++i) {
        left.push_back((pow_module(y, r, p) * pow_module(r, s[i], p)) % p);
        right.push_back(pow_module(g, hash_vt[i], p));
    }

    if (left == right) {
        std::cout << "Verification successful!" << std::endl;
        return true;
    } else {
        std::cout << "Verification failed." << std::endl;
        return false;
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

    saveSignature(r, s);
    //sleep(15);
    bool isVerified = loadAndVerifySignature(y, g, p);
    if (isVerified) {
        std::cout << "Signature verified successfully!" << std::endl;
    } else {
        std::cout << "Signature verification failed." << std::endl;
    }

}
