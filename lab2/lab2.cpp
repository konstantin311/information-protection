#include <iostream>
#include <tuple>
#include <random>
#include <algorithm>
#include <fstream>
#include <vector>
#include "lab1_lib.hpp"  // Assuming the necessary functions: extendedGCD(), generateRandomPrime(), pow_module()

// Function to read a file into a vector of numbers
std::vector<int> readFileAsNumbers(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    std::vector<int> data;
    char byte;
    while (file.get(byte)) {
        data.push_back(static_cast<unsigned char>(byte));
    }
    return data;
}

// Function to write numbers to a file
void writeNumbersToFile(const std::string& filename, const std::vector<int>& data) {
    std::ofstream file(filename, std::ios::binary);
    for (int number : data) {
        file.put(static_cast<char>(number));
    }
}

// Function to generate a secret key
int generateSecretKey(int cA, int p) {
    int d, gcd, x, y;
    std::tie(gcd, x, y) = extendedGCD(cA, p - 1);

    if (gcd != 1) {
        std::cerr << "Error: cA and p-1 are not relatively prime!" << std::endl;
        return -1;
    } else {
        d = (x % (p - 1) + (p - 1)) % (p - 1); // Convert to positive value
        return d;
    }
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1000000, 1000000000);

    // Generate a prime number p
    int p;
    std::cout << "Enter prime number p (or 0 to generate a random prime): ";
    std::cin >> p;

    if (p == 0) {
        do {
            p = dist(gen);
        } while (!testFerma(p, 500)); 
        std::cout << "Generated prime number p: " << p << std::endl;
    }

    // Generate secret key A (cA) and dA
    int cA, dA;
    do {
        cA = dist(gen); // Случайное число от 1 до p-1
        dA = generateSecretKey(cA, p);
    } while (dA == -1); // Продолжать, пока dA не будет валидным
    std::cout << "Generated secret key A (cA): " << cA << std::endl;
    std::cout << "Secret key A (dA): " << dA << std::endl;

    // Generate secret key B (cB) and dB
    int cB, dB;
    do {
        cB = dist(gen); // Случайное число от 1 до p-1
        dB = generateSecretKey(cB, p);
    } while (dB == -1); // Продолжать, пока dB не будет валидным
    std::cout << "Generated secret key B (cB): " << cB << std::endl;
    std::cout << "Secret key B (dB): " << dB << std::endl;

    // Menu for choosing mode: number or file
    int mode;
    std::cout << "Choose mode: 1 - Number, 2 - File: ";
    std::cin >> mode;

    if (mode == 1) {
        // Number mode
        int m;
        std::cout << "Enter message m (number): ";
        std::cin >> m;

        // Check that m is less than p
        if (m >= p) {
            std::cerr << "Error: message must be less than p!" << std::endl;
            return 1;
        }

        // Encryption
        int x1 = pow_module(m, cA, p);
        int x2 = pow_module(x1, cB, p);
        std::cout << "Encrypted message: " << x2 << std::endl;

        // Decryption
        int x3 = pow_module(x2, dA, p);
        int x4 = pow_module(x3, dB, p);
        std::cout << "Decrypted message: " << x4 << std::endl;

        if (x4 == m) {
            std::cout << "Message successfully decrypted!" << std::endl;
        } else {
            std::cerr << "Decryption error!" << std::endl;
        }
    } else if (mode == 2) {
        // File mode
        std::string inputFileName;
        std::cout << "Enter the file name for encryption: ";
        std::cin >> inputFileName;

        // Read file
        std::vector<int> message = readFileAsNumbers(inputFileName);

        // Encrypt file
        std::vector<int> encryptedData;
        for (int byte : message) {
            if (byte >= p) {
                std::cerr << "Error: byte " << byte << " is greater than or equal to p!" << std::endl;
                return 1;
            }
            int x1 = pow_module(byte, cA, p);
            int x2 = pow_module(x1, cB, p);
            encryptedData.push_back(x2);
        }

        // Write encrypted file
        std::string encryptedFileName = "encrypted_" + inputFileName;
        writeNumbersToFile(encryptedFileName, encryptedData);
        std::cout << "Encrypted data written to file: " << encryptedFileName << std::endl;

        // Decrypt file
        std::vector<int> decryptedData;
        for (int byte : encryptedData) {
            int x3 = pow_module(byte, dA, p);
            int x4 = pow_module(x3, dB, p);
            decryptedData.push_back(x4);
        }

        // Write decrypted file
        std::string decryptedFileName = "decrypted_" + inputFileName;
        writeNumbersToFile(decryptedFileName, decryptedData);
        std::cout << "Decrypted data written to file: " << decryptedFileName << std::endl;
    } else {
        std::cerr << "Invalid mode selected!" << std::endl;
        return 1;
    }

    return 0;
}
