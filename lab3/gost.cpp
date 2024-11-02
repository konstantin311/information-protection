#include "gamal_lib.hpp"
#include "lab1_lib.hpp"
#include "gost.hpp"
#include "md5.hpp"
#include <fstream>
#include <vector>
#include <iostream>
#include <random>
#include <iomanip>
//#include <unistd.h>


void saveSignatureGost(long long r, const std::vector<long long>& s) {
    std::ofstream outFile("signature_gost.txt");
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

bool loadAndVerifySignatureGost(long long y, long long g, long long p, long long q, long long a) {
    std::ifstream inFile("signature_gost.txt");
    long long r;
    std::vector<long long> s;

    if (inFile.is_open()) {
        if (!(inFile >> r)) {
            std::cerr << "Error reading r from signature_gost.txt" << std::endl;
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

    if (r <= 0 || r >= q) {
        std::cerr << "Error: r is out of range (0 < r < q)" << r <<" "<< q << std::endl;
        return false;
    }
    for (const auto& si : s) {
        if (si <= 0 || si >= q) {
            std::cerr << "Error: s is out of range (0 < s < q)" << std::endl;
            return false;
        }
    }
     bool signatureValid = true;

    for (size_t i = 0; i < hash_vt.size(); ++i) {
        long long h = hash_vt[i];
        long long h_inv = modularInverse(h, q); 
        if (h_inv == -1) {
            std::cerr << "Error: No modular inverse for h element at index " << i << std::endl;
            signatureValid = false;
            continue;
        }

        long long u1 = (s[i] * h_inv) % q;
        long long u2 = (-r * h_inv) % q;
        if (u2 < 0) u2 += q; 
        long long v = (pow_module(a, u1, p) * pow_module(y, u2, p) % p) % q;
        if (v != r) {
            std::cerr << "Signature verification failed for hash element at index " << i << std::endl;
            signatureValid = false;
        }
    }

    if (signatureValid) {
        std::cout << "Signature is valid for all hash elements!" << std::endl;
        return true;
    } else {
        std::cerr << "Signature is invalid." << std::endl;
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
    std::cout<<std::endl;
    saveSignatureGost(r, s);
    //sleep(15);
    bool isVerified = loadAndVerifySignatureGost(y, g, p, q, a);
    if (isVerified) {
        std::cout << "Signature verified successfully!" << std::endl;
    } else {
        std::cout << "Signature verification failed." << std::endl;
    }
}
