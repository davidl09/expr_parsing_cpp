#include <iostream>

#include "parsing.hpp"

int main(){
    double num;
    std::string in;

    while(true){
        std::cout << "Enter an expression to evaluate\n";
        std::cin >> in;
        num = Parsing::parse_mstring<double>(in);
        std::cout << num << "\n";
    }
}