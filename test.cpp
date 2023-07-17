#include "parsing.hpp"
#include <complex>

int main(){
    
    auto test = Parsing::Expression<double>("exp(sin(1.57) + cos(1.57))");
    
    std::cout << test.evaluate({}) << "\n";
    
}