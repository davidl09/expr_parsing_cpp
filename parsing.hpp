#ifndef PARSING_HPP
#define PARSING_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <map>
#include <cmath>
#include <type_traits>
#include <stdexcept>
#include <complex>
#include <iostream>

#include "sstream_convert.hpp"

namespace Parsing
{

    const static std::vector<std::string> operators = //duplicate of unordered_map below, needs to be integrated
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
        "ln(",
        "real(",
        "imag(",
        "arg(",
        "abs("
    };

    template<typename T>
    static std::unordered_map<std::string, std::function<T(T)>> unary_funcs(
        {
            {"sqrt(", [](T input){return std::sqrt(input);}},
            {"exp(", [](T input){return std::exp(input);}},
            {"sin(", [](T input){return std::sin(input);}},
            {"cos(", [](T input){return std::cos(input);}},
            {"tan(", [](T input){return std::tan(input);}},
            {"asin(", [](T input){return std::asin(input);}},
            {"acos(", [](T input){return std::acos(input);}},
            {"atan(", [](T input){return std::atan(input);}},
            {"ln(", [](T input){return std::log(input);}},
            //{"log2(", [](T input){return std::log2(input);}},
            {"log(", [](T input){return std::log10(input);}},
            {"abs(", [](T input){return std::abs(input);}},
        }
    );

    template<typename T>
    static std::unordered_map<std::string, std::function<T(T,T)>> binary_ops(
        {
            {"+", [](T left, T right){return left + right;}},
            {"-", [](T left, T right){return left - right;}},
            {"*", [](T left, T right){return left * right;}},
            {"/", [](T left, T right){return left / right;}},
            {"^", [](T left, T right){return std::pow(left, right);}},
        }
    );

    const static std::vector<std::string> basic_operators =
    {
        "+",
        "-",
        "*",
        "/",
        "^",
    };

    template<typename T>
    struct is_complex_t : public std::false_type {};

    template<typename T>
    struct is_complex_t<std::complex<T>> : public std::true_type {};

    template<typename T>
    constexpr bool is_complex() { return is_complex_t<T>::value; }


    class Token
    {
    private:
        std::string self;

        static bool is_any_bracket(const char &self)
        {
            return self == '(' || self == ')';
        }

        static bool is_r_bracket(const char &self)
        {
            return self == ')';
        }

        static bool is_l_bracket(const char &self)
        {
            return self == '(';
        }

        static bool matched_brackets(std::string expr)
        {
            int b = 0;
            for (auto &c : expr)
            {
                if (c == '(')
                    ++b;
                if (c == ')')
                    --b;
            }
            return b == 0;
        }

        static bool is_basic_operator(const char &self)
        {
            for (auto &op : basic_operators)
            {
                if (self == op[0])
                    return true;
            }
            return false;
        }

        static bool is_basic_operator(std::string &self)
        {
            for (auto &op : basic_operators)
            {
                if (self == op)
                    return true;
            }
            return false;
        }

        static bool is_operator(std::string &self)
        {
            for (auto &op : operators)
            {
                if (self == op)
                    return true;
            }
            return false;
        }

    public:
        Token(std::string value) : self(value)
        {
            for (auto &it : self)
            {
                if (!is_valid_char(it))
                    throw std::invalid_argument("Invalid character detected\n");
            }
        }

        const std::string &string_val()
        {
            return self;
        }

        static bool is_valid_char(const char &c)
        {
            return is_numerical(c) ||
                   is_any_bracket(c) ||
                   is_basic_operator(c) ||
                   is_alpha(c);
        }

        static bool is_alpha(const char &c)
        {
            return (c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A');
        }

        bool is_numerical()
        {
            for (auto &it : self)
            {
                if ((it > '9' || it < '0') && it != '.')
                    return false;
            }
            return true;
        }

        static bool is_numerical(const char &c)
        {
            return (c <= '9' && c >= '0') || c == '.';
        }

        bool is_operator()
        {
            for (auto &op : operators)
            {
                if (self == op)
                    return true;
            }
            return false;
        }

        const int op_precedence()
        {
            if (!is_operator())
                return 0; // values have '0' precedence
            if (!is_binary_op())
                return 1; // all functions have lowest precedence

            switch (self[0])
            {
            case '+':
                return 2;
            case '-':
                return 2;
            case '/':
                return 3;
            case '*':
                return 4;
            case '^':
                return 5;
            }
            return 0;
        }

        bool is_binary_op()
        {
            for (auto &op : basic_operators)
            {
                if (self == op)
                    return true;
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

        bool is_unary_func()
        {
            return is_operator() && !is_binary_op();
        }

        bool right_associate()
        {
            return self == "^";
        }

        Token &operator=(Token &a)
        {
            this->self = a.self;
            return *this;
        }

        operator int()
        {
            if (!is_numerical())
                throw std::bad_function_call();
            return std::stoi(self);
        }

        operator float()
        {
            if (!is_numerical())
                throw std::bad_function_call();
            return std::stof(self);
        }

        operator double()
        {
            if (!is_numerical())
                throw std::bad_function_call();
            return std::stod(self);
        }

        operator long double()
        {
            if (!is_numerical())
                throw std::bad_function_call();
            return std::stold(self);
        }

        template <typename T>
        T function_eval(T& input)
        {
            if(unary_funcs<T>.find(self) != unary_funcs<T>.end()) return (unary_funcs<T>[self])(input);
            
            throw std::invalid_argument("Unknown function token");
        }

        template <typename T>
        T function_eval(T left, T right)
        {

            if(binary_ops<T>.find(self) != binary_ops<T>.end()) return (binary_ops<T>[self])(left, right);
            throw std::invalid_argument("Unknown operator token");
        }

        static std::vector<Token> tokenize(std::string expression)
        {
            if (!matched_brackets(expression))
                throw std::invalid_argument("Mismatched parentheses\n");

            std::string temp;
            std::vector<Token> out;

            auto it = expression.begin();
            while (it != expression.end())
            {

                if(*it < 33) 
                    ++it;
                    
                else if (is_r_bracket(*it))
                {
                    temp.push_back(*it);
                    out.push_back(Token(temp));
                    temp.erase();
                    ++it;
                    continue;
                }

                else if (is_numerical(*it))
                {
                    while (is_numerical(*it))
                    {
                        temp.push_back(*it);
                        ++it;
                    }
                    out.push_back(Token(temp));
                    temp.erase();
                }

                else if (*it == '-')
                {
                    if (it == expression.begin() || is_l_bracket(*(it - 1)) || is_basic_operator(*(it - 1))) // unary minus
                    {
                        if(!is_numerical(*(it + 1)) && !is_alpha(*(it + 1))) throw std::invalid_argument("Malformed expression");
                        temp.push_back(*it++);
                    }
                    else
                    {
                        temp.push_back(*it++);
                        out.push_back(Token(temp));
                        temp.erase();
                    }
                }

                else if (is_basic_operator(*it))
                {
                    temp.push_back(*it++);
                    out.push_back(temp);
                    temp.erase();
                }

                else if (is_alpha(*it))
                {
                    if (it == expression.end() || !is_alpha(*(it + 1))) // if variable
                    {
                        temp.push_back(*it++);
                        out.push_back(temp);
                        temp.erase();
                    }
                    else
                    {
                        temp.push_back(*it);
                        while (!is_l_bracket(*it++))
                        {
                            temp.push_back(*it);
                        }
                        out.push_back(Token(temp));
                        temp.erase();
                    }
                }

                else if (is_l_bracket(*it))
                {
                    temp.push_back(*it);
                    out.push_back(Token(temp));
                    temp.erase();
                    ++it;
                }

                else
                    throw std::invalid_argument("Unknown symbol encountered");
            }
            return out;
        }
    };

    class ParsingShunt
    {
        std::vector<Token> queue, output;

    public:
        auto convert_to_rpn(std::vector<Token> &expression)
        {
            output.reserve(expression.size());
            queue.reserve(expression.size() / 2);

            auto i = expression.begin();
            while (i != expression.end())
            {
                if (i->is_l_bracket() || (i->is_unary_func()))
                {
                    queue.push_back(*i++);
                }
                else if (i->is_r_bracket())
                {
                    while (queue.size() > 1 && !(queue.back().is_l_bracket() || (queue.back().is_unary_func()))) // while the top of the queue is not a func or l bracket
                    {
                        output.push_back(queue.back());
                        queue.pop_back();
                    }
                    if(!(queue.back().is_unary_func() || queue.back().is_l_bracket())) throw std::invalid_argument("Malformed expression");
                    if (queue.back().is_unary_func())
                    {
                        output.push_back(queue.back());
                    }
                    queue.pop_back();
                    ++i;
                }
                else if (i->is_operator())
                {
                    while (
                        queue.size() > 0 &&
                        ((i->right_associate() && (i->op_precedence() < queue.back().op_precedence())) ||
                         (!i->right_associate() && (i->op_precedence() <= queue.back().op_precedence()))
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

            while (queue.size() > 0)
            {
                output.push_back(queue.back());
                queue.pop_back();
            }
            return output;
        }
    };

    template <typename T>
    class Expression
    {

    private:
        std::vector<Token> self;
        std::vector<Token> self_rpn;
        std::string variables;

    public:
        Expression(std::string expr)
        {
            variables = "";
            self = Token::tokenize(expr);
            self_rpn = ParsingShunt().convert_to_rpn(self);

            for (auto &t : self) // add variables to list
            {
                if (t.is_variable())
                {
                    variables.push_back(t.string_val()[0]);
                }
            }

            if(is_complex<T>())
            {
                unary_funcs<T>["real("] = [](T input){return std::real(input);};
                unary_funcs<T>["imag("] = [](T input){return std::imag(input);};
                unary_funcs<T>["arg("]  = [](T input){return std::arg(input);};
                variables.push_back('i');
            } 

        }

        T evaluate(std::unordered_map<char, T> vars)
        {
            if(is_complex<T>()) vars['i'] = {0,1};
            for(const auto& v : variables)
            {
                if(vars.find(v) == vars.end()) throw std::invalid_argument("Missing variable value\n");
            }
            
            std::vector<T> retval;


            auto it = self_rpn.begin();
            while (it != self_rpn.end())
            {
                if (!it->is_operator())
                {
                    if(it->is_variable())
                    {
                        retval.push_back(vars[it->string_val()[0]]);
                    }
                    else retval.push_back(convert_to<T>(it->string_val())); //may not work with custom types
                }
                else
                {
                    if(it->is_unary_func())
                    {
                        if(!(retval.size() > 0)) throw std::invalid_argument("Malformed expression");
                        retval.back() = it->function_eval(retval.back());
                    }
                    else
                    {
                        if(!(retval.size() > 1)) throw std::invalid_argument("Malformed expression");
                        retval[retval.size() - 2] = it->function_eval(retval[retval.size() - 2], retval.back());
                        retval.pop_back();
                    }
                }
                ++it;
            }
            if (!(retval.size() == 1)) throw std::invalid_argument("Extra operator, malformed expression");
            return retval.back();
        }
    };

}

#endif
