#include "parsing.hpp"

int main(){
    std::vector<Parsing::Token> test;
    test = Parsing::Token::tokenize("3+4");
    for(auto& a : test) {std::cout << a.val() << "\n";}


}