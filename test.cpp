#include "parsing.hpp"
#include <complex>

int main(){
    
    auto test = Parsing::Expression<long double>("sin(a+b)");
    
    std::cout << test.evaluate({{'a', 2}, {'b', (long double)1.141592}}) << "\n";
    
}