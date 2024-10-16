#include "vernam_lib.hpp"

std::vector<int> generateVernamKey(int length) {
    std::vector<int> key(length);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 255); 
    for (int i = 0; i < length; ++i) {
        key[i] = dist(gen);
    }
    return key;
}

std::vector<int> vernamCipher(const std::vector<int>& data, const std::vector<int>& key) {
    std::vector<int> result(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] ^ key[i]; 
    }
    return result;
}


int test_Vernam() {
    std::string inputFileName;
    std::cout << "Enter the file name for encryption (Vernam cipher): ";
    std::cin >> inputFileName;

    std::vector<int> message;
    try {
        message = readFileAsNumbers(inputFileName);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    std::vector<int> key = generateVernamKey(message.size());

    std::vector<int> encryptedData = vernamCipher(message, key);

    std::string encryptedFileName = "vernam_encrypted_" + inputFileName;
    try {
        writeNumbersToFile(encryptedFileName, encryptedData);
        std::cout << "Encrypted data written to file: " << encryptedFileName << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    std::string keyFileName = "vernam_key_" + inputFileName;
    try {
        writeNumbersToFile(keyFileName, key);
        std::cout << "Key written to file: " << keyFileName << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    std::vector<int> decryptedData = vernamCipher(encryptedData, key);

    std::string decryptedFileName = "vernam_decrypted_" + inputFileName;
    try {
        writeNumbersToFile(decryptedFileName, decryptedData);
        std::cout << "Decrypted data written to file: " << decryptedFileName << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
