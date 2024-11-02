#include <iostream>
#include "lab1_lib.hpp"  
#include "rsa_lib.hpp"
#include "gamal_lib.hpp"
#include "gost.hpp"
int main() {
    /*test_rsa_signature();
    std::cout << "---------------------------------------------------" << std::endl;
    test_gamal();
    std::cout << "---------------------------------------------------" << std::endl;*/
    test_gost();
    return 0;
}
