#include <iostream>
#include "lab1_lib.hpp"  
#include "shamir_lib.hpp"
#include "vernam_lib.hpp"

int main() {
    int code;
    std::cout<<"Shamir code"<<std::endl;
    test_Shamir();
    std::cout << "---------------------------------------------------" << std::endl;
    code = test_Vernam();
    return 0;
}
