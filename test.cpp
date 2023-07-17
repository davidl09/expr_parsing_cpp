#include "parsing.hpp"

#include <complex>

int main(){
    
    auto test = Parsing::Expression<std::complex<double>>("sin(4+z)");
    
    std::cout << test.evaluate({std::complex<double>(0, 3)}) << "\n";
    
}