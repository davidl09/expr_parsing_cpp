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
            std::string self;
            std::string variables;

            static bool is_any_bracket(const char& self)
            {
                return self == '(' || self == ')';
            }
            
            static bool is_r_bracket(const char& self)
            {
                return self == ')';
            }
            
            static bool is_l_bracket(const char& self)
            {
                return self == '(';
            }

            static bool matched_brackets(std::string expr)
            {
                int b = 0;
                for(auto& c : expr)
                {
                    if(c == '(') ++b;
                    if(c == ')') --b;
                }
                return b == 0;
            }

            static bool is_basic_operator(const char& self)
            {
                for(auto& op : basic_operators)
                {
                    if(self == op[0]) return true;
                }
                return false;
            }

            static bool is_basic_operator(std::string& self)
            {
                for(auto& op : basic_operators)
                {
                    if(self == op) return true;
                }
                return false;
            }

            static bool is_operator(std::string& self)
            {
                for(auto& op : operators)
                {
                    if(self == op) return true;
                }
                return false;
            }

        public:  

            Token(std::string value) : self(value)
            {
                for (auto& it : self)
                {
                    if(!is_valid_char(it)) throw std::invalid_argument("Invalid character detected\n");
                }
            }
            
            const std::string& string_val()
            {
                return self;
            }
            
            static bool is_valid_char(const char& c)
            {
                return  
                is_numerical(c) ||
                is_any_bracket(c) ||
                is_basic_operator(c) ||
                is_alpha(c);
            }

            static bool is_alpha(const char& c)
            {
                return (c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A');
            }
            
            bool is_numerical()
            {
                for (auto& it : self)
                {
                    if((it > '9' || it < '0') && it != '.') return false;
                }
                return true;
            }

            static bool is_numerical(const char& c)
            {
                return (c <= '9' && c >= '0') || c == '.';
            }
            
            bool is_operator()
            {
                for(auto& op : operators)
                {
                    if(self == op) return true;
                }
                return false;
            }

            const int op_precedence()
            {
                if(!is_operator()) return 0; //values have '0' precedence
                if(!is_basic_operator()) return 1; //all functions have lowest precedence

                switch(self[0])
                {
                    case '+': return 1;
                    case '-': return 1;
                    case '/': return 2;
                    case '*': return 3;
                    case '^': return 4;
                }
                return 0;
            }
            
            bool is_basic_operator()
            {
                for(auto& op : basic_operators)
                {
                    if(self == op) return true;
                }
                return false;
            }
            
            bool is_any_bracket()
            {
                return self == "(" || self == ")";
            }
            
            bool is_r_bracket()
            {
                return self == ")";
            }
            
            bool is_l_bracket()
            {
                return self == "(";
            }

            bool is_variable()
            {
                return self.length() == 1 && is_alpha(self[0]);
            }

            bool is_function()
            {
                return is_operator() && !is_basic_operator();
            }

            bool right_associate()
            {
                return self == "^";
            }

            Token& operator=(Token& a)
            {
                this->self = a.self;
                this->variables = a.variables;
                return *this;
            }

            static std::vector<Token> tokenize(std::string expression)
            {

                if(!matched_brackets(expression)) throw std::invalid_argument("Mismatched parentheses\n");

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
                            temp.push_back(*it++);
                            out.push_back(Token(temp));
                            temp.erase();
                        }
                    }

                    else if(is_basic_operator(*it))
                    {
                        temp.push_back(*it++);
                        out.push_back(temp);
                        temp.erase();
                    }

                    else if(is_alpha(*it)){
                        if(it == expression.end() || !is_alpha(*(it + 1))) //if variable
                        {
                            temp.push_back(*it++);
                            out.push_back(temp);
                            temp.erase();
                        }
                        else 
                        {
                            temp.push_back(*it);
                            while(!is_l_bracket(*it++))
                            {
                                temp.push_back(*it);
                            }
                            out.push_back(Token(temp));
                            temp.erase();
                        }
                        
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
                return out;
            }
    };

    class ParsingShunt{
        std::vector<Token> queue, output;

        public:

            auto convert_to_rpn(std::vector<Token>& expression)
            {
                output.reserve(expression.size());
                queue.reserve(expression.size() / 2);

                auto i = expression.begin();
                while (i != expression.end())
                {
                    if(i->is_l_bracket() || (i->is_function()))
                    {
                        queue.push_back(*i++);
                    }
                    else if(i->is_r_bracket())
                    {
                        while(queue.size() > 1 && !(queue.back().is_l_bracket() || (queue.back().is_function()))) //while the top of the queue is not a func or l bracket
                        {
                            output.push_back(queue.back());
                            queue.pop_back();
                        }   
                        assert(queue.back().is_function() || queue.back().is_l_bracket());
                        if(queue.back().is_function())
                        {
                            output.push_back(queue.back());
                        }
                        queue.pop_back();
                        ++i;
                    }
                    else if(i->is_operator())
                    {
                        while(
                            queue.size() > 0 &&
                            (
                                (i->right_associate() && (i->op_precedence() < queue.back().op_precedence())) ||
                                (i->op_precedence() <= queue.back().op_precedence())
                            )
                        )
                        {
                            output.push_back(queue.back());
                            queue.pop_back();
                        }
                        queue.push_back(*i++);
                    }
                    else
                    {
                        output.push_back(*i++);
                    }
                    
                }

                while(queue.size() > 0)
                {
                    output.push_back(queue.back());
                    queue.pop_back();
                }
                return output;
            }
    };
    
    template<typename T>
    class Expression{
        
        private:
            std::vector<Token> self;
            std::string variables;


            
        public:
            Expression(std::string expr)
            {
                variables = "";
                self = Token::tokenize(expr);

                for(auto& t : self) //add variables to list
                {
                    if(t.is_variable())
                    {
                        variables.push_back(t.self()[0]);
                    }
                }


            }
    };

}

#endif
