#ifndef PARSING_HPP_INCLUDE
#define PARSING_HPP_INCLUDE

#include <string>
#include <vector>
#include <iostream>

namespace Parsing{
    
    const std::vector<std::string> operators = 
    {
        "+",
        "-",
        "*",
        "/",
        "^",
        "sqrt(",
        "exp(",
        "sin(",
        "cos(",
        "tan(",
        "asin(",
        "acos(",
        "atan(",
        "log(",
        "log2(",
        "ln("
    }
    
    const std::vector<std::string> basic_operators = 
    {
        "+",
        "-",
        "*",
        "/",
        "^",
    }
    
    class Token{
        private:
            const std::string self;
            
        public:
            Token(std::string value) : self(value) 
            {
                for (char& it : self)
                {
                    if(!is_valid_char(it)) throw std::invalid_argument("Invalid character detected: \' " + it + " \'\n");
                }
            }
            
            constexpr const std::string& self()
            {
                return self;
            }
            
            static constexpr bool is_valid_char(char& c)
            {
                return (c <= '9' && c >= '0') || (c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A') || c == '.' || c == '^' || c == '(' || c == ')'
            }
            
            constexpr bool is_numerical()
            {
                for (char& it : self)
                {
                    if((it > '9' || it < '0') && it != '.') return false;
                }
                return true;
            }
            
            constexpr bool is_operator()
            {
                for(auto& op : operators)
                {
                    if(self == op) return true;
                }
                return false;
            }
            
            static constexpr bool is_operator(std::string& self)
            {
                for(auto& op : operators)
                {
                    if(self == op) return true;
                }
                return false;
            }
            
            constexpr bool is_basic_operator()
            {
                for(auto& op : basic_operators)
                {
                    if(self == op) return true;
                }
                return false;
            }
            
            static constexpr bool is_basic_operator(std::string& self)
            {
                for(auto& op : basic_operators)
                {
                    if(self == op) return true;
                }
                return false;
            }
            
            constexpr bool is_any_bracket()
            {
                return self == '(' || self == ')';
            }
            
            constexpr bool is_r_bracket()
            {
                return self == ')';
            }
            
            constexpr bool is_l_bracket()
            {
                return self == '(';
            }
            
            static constexpr bool is_any_bracket(char& self)
            {
                return self == '(' || self == ')';
            }
            
            static constexpr bool is_r_bracket(char& self)
            {
                return self == ')';
            }
            
            static constexpr bool is_l_bracket(char& self)
            {
                return self == '(';
            }
    };
    
    template<typename T>
    class Expression{
        
        private:
            std::vector<Token> self;
            
        public:
            Expression(std::string expr)
            {
                std::vector<string>
                for (int i = 0; i < expr.length(); ++i)
                {
                    if(is_basic_operator(expr[i]))
                    {
                        switch
                    }
                }
            }
    };
    
    
    /*
    class Tokenizer{
        public:
        
        std::vector<std::string> tokenize(std::string expr){
            if(expr.length() == 0)
                throw std::invalid_argument("Empty string passed to tokenizer\n");
                
            auto i = expr.begin();
            std::string temp;
            
            std::vector<std::string> out;
            out.reserve(expr.length());
            
            while(i != expr.end()){
                if(is_numchar(*i)){
                    while(is_numchar(*i)){
                        temp.push_back(*i++);
                    }
                    out.push_back(temp);
                    temp.erase();
                    continue;
                }
                if(is_operator(*i)){
                    if(*i == '-' && (i == expr.begin() || is_operator(*(i-1))) && (i != expr.end() - 1 && is_numchar(*(i+1)))){
                        //if it's a unary minus
                        temp.push_back('-');
                        i++;
                        continue;
                    }else{
                        assert(temp.length() == 0);
                        temp.push_back(*i++);
                        out.push_back(temp);
                        temp.erase();
                        continue;
                    }
                }
                if(m_funcs_pre.find(*i) != std::string::npos){
                    while(isalpha(*i)){
                        temp.push_back(*i++);
                    }
                    assert(is_bracket(*i));
                    temp.push_back(*i++);
                    out.push_back(temp);
                    temp.erase();
                    continue;
                }
                if(is_bracket(*i)){
                    assert(temp.length() == 0);
                    temp.push_back(*i++);
                    out.push_back(temp);
                    temp.erase();
                    continue;
                }else{
                    
                }
            }
            return out;
        }
    };
    */
}

#endif
