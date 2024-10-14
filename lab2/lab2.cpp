#include <iostream>
#include "lab1_lib.hpp"  
#include "shamir_lib.hpp"
#include "vernam_lib.hpp"
#include "gamal_lib.hpp"

int main() {
    int code;
    //std::cout<<"Shamir code"<<std::endl;
    //test_Shamir();
    std::cout << "---------------------------------------------------" << std::endl;
    //code = test_Vernam();
    std::cout << "---------------------------------------------------" << std::endl;
    //test_gamal();
    std::cout << "---------------------------------------------------" << std::endl;
    test_gamal_file_encryption();
    return 0;
}
