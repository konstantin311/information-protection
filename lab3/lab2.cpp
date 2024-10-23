#include <iostream>
#include "lab1_lib.hpp"  
#include "rsa_lib.hpp"

int main() {
    //test_rsa();
    std::cout << "---------------------------------------------------" << std::endl;
    //test_rsa_file_encryption();
     std::string input_file = "file.txt";          // Имя файла для подписи
    std::string signature_file = "signature.txt";     // Имя файла для сохранения подписи

    test_rsa_file_encryption(input_file, signature_file);
    return 0;
}
