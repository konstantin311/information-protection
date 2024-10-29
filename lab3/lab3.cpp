#include <iostream>
#include "lab1_lib.hpp"  
#include "rsa_lib.hpp"
#include "gamal_lib.hpp"
int main() {
    test_gamal();
    std::cout << "---------------------------------------------------" << std::endl;
    /*std::string input_file = "file.txt";          
    std::string signature_file = "signature_rsa.txt";     */
    test_rsa_signature();
    return 0;
}
