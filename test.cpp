#include "parsing.hpp"

int main(){

    auto test = Parsing::Token::tokenize("sin(3/2)+3-a");
    for(auto& a : test) {std::cout << a.string_val() << "\n";}
    test = Parsing::ParsingShunt().convert_to_rpn(test);

    std::cout << "\n\n";
    for(auto& a : test) {std::cout << a.string_val();}

}