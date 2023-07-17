#include "parsing.hpp"
#include <complex>

int main(){
    
    auto test = Parsing::Expression<double>("a+b");
    
    std::cout << test.evaluate({{'a', 2}, {'b', 4}}) << "\n";
    
}