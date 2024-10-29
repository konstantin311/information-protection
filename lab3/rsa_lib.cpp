#include "rsa_lib.hpp"
#include "lab1_lib.hpp"
#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <string>
#include <sstream>


std::pair<std::pair<long long, long long>, std::pair<long long, long long>> generate_rsa_keys(long long phi) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(2, phi - 1);

    while (true) {
        long long c = distrib(gen);
        long long gcd, d, _;
        std::tie(gcd, d, _) = extendedGCD(c, phi);
        if (gcd == 1) {
            d %= phi;
            if (d < 0) {
                d += phi;
            }
            return std::make_pair(std::make_pair(c, phi), std::make_pair(d, phi)); 
        }
    }
}

void save_signatures_to_file(const std::vector<long long>& signatures, const std::string& filename) {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Error opening file for writing signatures!" << std::endl;
        return;
    }
    for (const long long& s : signatures) {
        out << s << " ";
    }
    out << std::endl;
    out.close();
    std::cout << "\nSignatures saved to " << filename << std::endl;
}

void verify_rsa_signature(long long N, long long d) {
    std::string document = loadMessage("file.txt");
    std::string hashHex = md5(document);
    std::vector<long long> hash_vt = HashToVector(hashHex);

    std::ifstream in("signature_rsa.txt");
    if (!in) {
        std::cerr << "Error opening signature file!" << std::endl;
        return;
    }

    std::vector<long long> signatures;
    long long signature;
    while (in >> signature) {
        signatures.push_back(signature);
    }
    in.close();

    for (size_t i = 0; i < signatures.size(); ++i) {
        long long s = signatures[i];
        long long w = pow_module(s, d, N);  
        
        if (w == hash_vt[i]) {
            std::cout << "h(m) " << hash_vt[i] << " = w " << w << std::endl;
        } else {
            std::cout << "h(m) " << hash_vt[i] << " != w " << w << std::endl;
        }
    }
}

void test_rsa_signature() {
    long long N, d, p, q, c;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(32500,45000);

    do {
        p = dist(gen);
    } while (!millerRabinTest(p, 100));
    std::cout << "Generated prime number p: " << p << std::endl;

    do {
        q = dist(gen);
    } while (!millerRabinTest(q, 100));
    std::cout << "Generated prime number q: " << q << std::endl;

    N = p * q;
    long long phi = (p - 1) * (q - 1);
    std::cout << "phi = " << phi << std::endl;

    auto keys = generate_rsa_keys(phi);
    c = keys.first.first; 
    d = keys.second.first;

    std::cout << "Generated keys:" << std::endl;
    std::cout << "Public key (N, c): (" << N << ", " << c << ")" << std::endl;
    std::cout << "Private key (d): " << d << std::endl;

    std::string document = loadMessage("file.txt");
    std::string hashHex = md5(document);
    std::vector<long long> hash_vt = HashToVector(hashHex);
    std::cout <<"Hash: ";
    for (const long long& value : hash_vt) {
        std::cout << value << " ";
    }
    std::cout <<"\nS: ";
    std::vector<long long> signatures;
    for (const long long& value : hash_vt) {
        long long s = pow_module(value, c, N);  
        signatures.push_back(s);
        std::cout << s << " ";
    }
    save_signatures_to_file(signatures, "signature_rsa.txt");
    verify_rsa_signature(N, d);
}

