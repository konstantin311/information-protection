#include "shamir_lib.hpp"
#include "lab1_lib.hpp"

std::vector<int> readFileAsNumbers(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Error: cannot open the file.");
    }
    std::vector<int> data;
    char byte;
    while (file.get(byte)) {
        data.push_back(static_cast<unsigned char>(byte));
    }
    return data;
}


void writeNumbersToFile(const std::string& filename, const std::vector<int>& data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Error: cannot open the file to write.");
    }
    for (int number : data) {
        file.put(static_cast<char>(number));
    }
}


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

void test_Shamir(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1000000, 1000000000);

    int p;
    std::cout << "Enter prime number p (or 0 to generate a random prime): ";
    std::cin >> p;

    if (p == 0) {
        do {
            p = dist(gen); 
        } while (!millerRabinTest(p, 100));
        std::cout << "Generated prime number p: " << p << std::endl;
    }


    int cA, dA;
    do {
        cA = rand() % (p - 1) + 1;
        std::cout << "Generated (cA): " << cA << std::endl;
        dA = generateSecretKey(cA, p);
    } while (dA == -1); 
    std::cout << "Generated secret key A (cA): " << cA << std::endl;
    std::cout << "Secret key A (dA): " << dA << std::endl;

    int cB, dB;
    do {
        cB = rand() % (p - 1) + 1;
        std::cout << "Generated (cB): " << cB << std::endl;
        dB = generateSecretKey(cB, p);
    } while (dB == -1); // Continue until valid dB
    std::cout << "Generated secret key B (cB): " << cB << std::endl;
    std::cout << "Secret key B (dB): " << dB << std::endl;

    int mode;
    std::cout << "Choose mode: 1 - Number, 2 - File, 3 - Image: ";
    std::cin >> mode;

    if (mode == 1) {
        int m;
        std::cout << "Enter message m (number): ";
        std::cin >> m;

        if (m >= p) {
            std::cerr << "Error: message must be less than p!" << std::endl;
            //return 1;
        }


        int x1 = pow_module(m, cA, p);
        int x2 = pow_module(x1, cB, p);
        std::cout << "Encrypted message: " << x2 << std::endl;

        int x3 = pow_module(x2, dA, p);
        int x4 = pow_module(x3, dB, p);
        std::cout << "Decrypted message: " << x4 << std::endl;

        if (x4 == m) {
            std::cout << "Message successfully decrypted!" << std::endl;
        } else {
            std::cerr << "Decryption error!" << std::endl;
        }

    } else if (mode == 2 || mode == 3) {
        std::string inputFileName;
        if (mode == 2) {
            std::cout << "Enter the file name for encryption: ";
        } else {
            std::cout << "Enter the image file name for encryption: ";
        }
        std::cin >> inputFileName;

        std::vector<int> message;
        try {
            message = readFileAsNumbers(inputFileName);
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
           // return 1;
        }

        std::vector<int> encryptedData;
        for (int byte : message) {
            if (byte >= p) {
                std::cerr << "Error: byte " << byte << " is greater than or equal to p!" << std::endl;
                //return 1;
            }
            int x1 = pow_module(byte, cA, p);
            int x2 = pow_module(x1, cB, p);
            encryptedData.push_back(x2);
        }

        std::string encryptedFileName = "encrypted_" + inputFileName;
        try {
            writeNumbersToFile(encryptedFileName, encryptedData);
            std::cout << "Encrypted data written to file: " << encryptedFileName << std::endl;
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            //return 1;
        }

        std::vector<int> decryptedData;
        for (int byte : encryptedData) {
            int x3 = pow_module(byte, dA, p);
            int x4 = pow_module(x3, dB, p);
            decryptedData.push_back(x4);
        }

        std::string decryptedFileName = "decrypted_" + inputFileName;
        try {
            writeNumbersToFile(decryptedFileName, decryptedData);
            std::cout << "Decrypted data written to file: " << decryptedFileName << std::endl;
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            //return 1;
        }
    } else {
        std::cerr << "Invalid mode selected!" << std::endl;
        //return 1;
    }
}