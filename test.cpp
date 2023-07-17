#include "parsing.hpp"
#include <complex>

int main(){
    
    char input[100];
    while(true)
    {
        std::cout << "Enter an expression: \n";
        std::cin.getline(&input[0], 100, '\n');
        try{
            auto t = Parsing::Expression<double>(input);
            std::cout << "Value: " << t.evaluate({}) << "\n\n";
        }
        catch(...)
        {
            std::cout << "Malformed expression, try again\n";
        }
    }
    
}