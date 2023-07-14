#ifndef PARSING_HPP
#define PARSING_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
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
    };
    
    const std::vector<std::string> basic_operators = 
    {
        "+",
        "-",
        "*",
        "/",
        "^",
    };
    
    class Token{
        private:
            const std::string self;
            
        public:
            Token(std::string value) : self(value) 
            {
                for (auto& it : self)
                {
                    if(!is_valid_char(it)) throw std::invalid_argument("Invalid character detected");
                }
            }
            
            constexpr const std::string& val()
            {
                return self;
            }
            
            static constexpr bool is_valid_char(const char& c)
            {
                return (c <= '9' && c >= '0') || (c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A') || c == '.' || c == '^' || c == '(' || c == ')';
            }

            static constexpr bool is_alpha(const char& c)
            {
                return (c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A');
            }
            
            constexpr bool is_numerical()
            {
                for (auto& it : self)
                {
                    if((it > '9' || it < '0') && it != '.') return false;
                }
                return true;
            }

            static constexpr bool is_numerical(const char& c)
            {
                return (c <= '9' && c >= '0') || c == '.';
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

            static constexpr bool is_basic_operator(const char& self)
            {
                for(auto& op : basic_operators)
                {
                    if(self == op[0]) return true;
                }
                return false;
            }
            
            constexpr bool is_any_bracket()
            {
                return self == "(" || self == ")";
            }
            
            constexpr bool is_r_bracket()
            {
                return self == ")";
            }
            
            constexpr bool is_l_bracket()
            {
                return self == "(";
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

            static std::vector<Token> tokenize(std::string expression)
            {
                std::string temp;
                std::vector<Token> out;

                auto it = expression.begin();
                while(it != expression.end())
                {
                    
                    if (is_r_bracket(*it))
                    {
                        temp.push_back(*it);
                        out.push_back(Token(temp));
                        temp.erase();
                        ++it;
                        continue;
                    }

                    else if(is_numerical(*it))
                    {
                        while (is_numerical(*it))
                        {
                            temp.push_back(*it);
                            ++it;
                        }
                        out.push_back(Token(temp));
                        temp.erase();
                    }

                    else if(*it == '-')
                    {
                        if(is_l_bracket(*(it - 1)) || is_basic_operator(*(it - 1))) //unary minus
                        {
                            assert(is_numerical(*(it + 1)));
                            temp.push_back(*it++);
                        }
                        else
                        {
                            assert(it != expression.begin() && is_numerical(*(it - 1)) && is_numerical(*(it + 1))); //operator minus
                            temp.push_back(*it++);
                            out.push_back(Token(temp));
                            temp.erase();
                        }
                    }

                    else if(is_alpha(*it)){
                        temp.push_back(*it);
                        while(!is_l_bracket(*it++))
                        {
                            temp.push_back(*it);
                        }
                        out.push_back(Token(temp));
                        temp.erase();
                    }

                    else if(is_l_bracket(*it))
                    {
                        temp.push_back(*it);
                        out.push_back(Token(temp));
                        temp.erase();
                        ++it;
                    }

                    else throw std::invalid_argument("Unknown symbol encountered");
                }
            }
    };
    
    template<typename T>
    class Expression{
        
        private:
            std::vector<Token> self;
            
        public:
            Expression(std::string expr)
            {

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
