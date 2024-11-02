#include "gamal_lib.hpp"
#include "lab1_lib.hpp"
#include "gost.hpp"
#include "md5.hpp"
#include <fstream>
#include <vector>
#include <iostream>
#include <random>
#include <iomanip>


bool loadAndVerifySignatureGost(long long y, long long g, long long p) {
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

    std::string document = loadMessage("file.txt");

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

void test_gost() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(32768, 65535);
    long long q; 
    long long p;
    int b;
    std::uniform_int_distribution<> dist2(2, 10000);
    do{
        do {
        q = dist(gen);
        } while (!millerRabinTest(q, 100));
        b = dist2(gen);
        p = b * q + 1;
    }while(!millerRabinTest(p, 100));
    std::cout << "q = " << q << std::endl;
    std::cout << "p = " << p << std::endl;
    
    long long g, a = 0;
    std::uniform_int_distribution<> dist3(1, p - 1);
    do{
        do{
            g = dist3(gen);
            a = pow_module(g,b,p);
        }while(a<=1);
        std::cout << "a = " << a << std::endl;
    }while(pow_module(a,q,p)!=1); 

    std::cout << "g = " << g << std::endl;
    std::cout << "a = " << a << std::endl;

    std::uniform_int_distribution<> dist4(1, q);
    long long x = dist4(gen);
    long long y = pow_module(a, x, p);
    std::cout << "x = " << x << std::endl;
    std::cout << "y = " << y << std::endl;

    std::string document = loadMessage("file.txt");
    std::string hashHex = md5(document);
    std::vector<long long> hash_vt = HashToVector(hashHex);

    for (const long long& value : hash_vt) {
        std::cout << value << " ";
    }

    long long k = 0;
    long long r = 0;
    std::vector<long long> s;
    bool notzero = true;
    do {
        s.clear();
        do {
            k = dist4(gen);
            r = pow_module(a, k, p) % q;
        } while (r == 0);

        notzero = true;
        for (const auto& it : hash_vt) {
            long long s_value = (k * it + x * r) % q;
            s.push_back(s_value);
            if (s_value == 0) {
                notzero = false;
            }
        }
    } while (!notzero); 
    
    std::cout << "\nk = " << k << std::endl;
    std::cout << "r = " << r << std::endl;
    for (const auto& it : s) {
        std::cout << it << " ";
    }
}
