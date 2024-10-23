#include "rsa_lib.hpp"
#include "lab1_lib.hpp"
#include "md5.hpp"
#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>

std::vector<long long> sign_document(const std::string& document, long long c, long long N) {
    MD5 md5(document);
    std::string hash = md5.hexdigest();

    std::vector<long long> signatures;
    for (char ch : hash) {
        long long byte_value = static_cast<unsigned char>(ch);
        long long s = pow_module(byte_value, c, N); 
        signatures.push_back(s);
    }

    return signatures;
}

bool verify_signature(const std::string& document, const std::vector<long long>& signatures, long long d, long long N) {
    MD5 md5(document);
    std::string hash = md5.hexdigest();

    for (size_t i = 0; i < signatures.size(); ++i) {
        long long e = pow_module(signatures[i], d, N); 
        long long byte_value = static_cast<unsigned char>(hash[i]);

        if (e != byte_value) {
            return false; 
        }
    }
    return true; 
}

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

void save_signed_document(const std::string& document, const std::vector<long long>& signatures, const std::string& signed_file) {
    std::ofstream out(signed_file);
    if (!out) {
        std::cerr << "Error opening signed file!" << std::endl;
        return;
    }
    out << document << std::endl; 
    out << "Signatures: ";
    for (long long s : signatures) {
        out << s << " "; 
    }
    out << std::endl;
    out.close();
    std::cout << "Signed document saved to " << signed_file << std::endl;
}

std::vector<long long> read_signatures_from_file(const std::string& signed_file) {
    std::ifstream in(signed_file);
    if (!in) {
        std::cerr << "Error opening signed file!" << std::endl;
        return {}; 
    }

    std::string line;
    std::vector<long long> signatures;

    
    while (std::getline(in, line)) {
        if (line.find("Signatures: ") != std::string::npos) {
            std::istringstream iss(line.substr(line.find(": ") + 2));
            long long sig;
            while (iss >> sig) {
                signatures.push_back(sig);
            }
            break;
        }
    }

    in.close();
    return signatures;
}

void test_rsa_signature(const std::string& input_file, const std::string& signed_file) {
    long long N, e, d, p, q, c;

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

    std::ifstream in(input_file);
    if (!in) {
        std::cerr << "Error opening input file!" << std::endl;
        return;
    }

    std::string document((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();

    std::vector<long long> signatures = sign_document(document, c, N);
    std::cout << "Signatures: ";
    for (long long s : signatures) {
        std::cout << s << " ";
    }
    std::cout << std::endl;

    save_signed_document(document, signatures, signed_file);        

    sleep(10);

    std::vector<long long> signatures_from_file = read_signatures_from_file(signed_file);
    if (signatures_from_file.empty()) {
        std::cerr << "Failed to read signatures from file." << std::endl;
        return;
    }

    bool is_valid = verify_signature(document, signatures_from_file, d, N);
    if (is_valid) {
        std::cout << "e = h => good!" << std::endl;
    } else {
        std::cout << "e != h => bad" << std::endl;
    }
}

