#include "parsing.hpp"

int main(){
    
    auto test = Parsing::Expression<double>("exp(cos(x^2))");
    for(int i = -2; i < 3; i++)
    {
        std::cout << test.evaluate({(double)i}) << "\n";
    }

/*
    double a = 2, b = 3;
    std::cout << Parsing::Token("/").function_eval<double>(a, b);*/
    
}