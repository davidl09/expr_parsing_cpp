#include "parsing.hpp"

int main(){

    auto test = Parsing::Expression<double>("3+a/2");
    auto val = test.evaluate({6.0});
    std::cout << "\n" << val << std::endl;
    
}