#ifndef PARSING_HPP
#define PARSING_HPP

#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <functional>
#include <map>
#include <cmath>
#include <type_traits>
#include <stdexcept>
#include <complex>
#include <iostream>
#include <unordered_map>
#include <execution>



template <typename T>
T convert_to(const std::string &str)
{
    std::istringstream ss(str);
    T num;
    ss >> num;
    return num;
}


namespace Parsing::Math {
    const long double LOWER_THRESHOLD = 1.0e-6;
    const long double UPPER_BOUND = 1.0e+4;
    const int MAXNUM = 100;

    static std::complex<long double> zeta(const std::complex<long double>& s)
    {
        std::complex<long double> a_arr[MAXNUM + 1];
        std::complex<long double> half(0.5, 0.0);
        std::complex<long double> one(1.0, 0.0);
        std::complex<long double> two(2.0, 0.0);
        std::complex<long double> rev(-1.0, 0.0);
        std::complex<long double> sum(0.0, 0.0);
        std::complex<long double> prev(1.0e+20, 0.0);
        
        a_arr[0] = half / (one - std::pow(two, (one - s))); //initialize with a_0 = 0.5 / (1 - 2^(1-s))
        sum += a_arr[0];

        for (int n = 1; n <= MAXNUM; n++)
        {
            std::complex<long double> nCplx(n, 0.0); //complex index

            for (int k = 0; k < n; k++)
            {
                std::complex<long double> kCplx(k, 0.0); //complex index

                a_arr[k] *= half * (nCplx / (nCplx - kCplx));
                sum += a_arr[k];
            }

            a_arr[n] = (rev * a_arr[n - 1] * std::pow((nCplx / (nCplx + one)), s) / nCplx);
            sum += a_arr[n];


            if (std::abs(prev - sum) < LOWER_THRESHOLD)//If the difference is less than or equal to the threshold value, it is considered to be convergent and the calculation is terminated.
                break;

            if (std::abs(sum) > UPPER_BOUND)//doesn't work for large values, so it gets terminated when it exceeds UPPER_BOUND
                break;

            prev = sum;
        }

        return sum;
    }
}

namespace Parsing {

    constexpr static std::array<std::string, 24> operators //duplicate of unordered_map below, needs to be integrated
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
        "csec(",
        "sec(",
        "cot(",
        "asin(",
        "acos(",
        "atan(",
        "log(",
        "log2(",
        "ln(",
        "real(",
        "imag(",
        "arg(",
        "abs(",
        //"gamma(",
        "zeta("
    };

    template<typename T>
    static std::unordered_map<std::string, std::function<T(T)>> unary_funcs
        {
            {"sqrt(", [](T input){return static_cast<T>(std::sqrt(static_cast<T>(input)));}},
            {"exp(", [](T input){return static_cast<T>(std::exp(static_cast<T>(input)));}},
            {"sin(", [](T input){return static_cast<T>(std::sin(static_cast<T>(input)));}},
            {"cos(", [](T input){return static_cast<T>(std::cos(static_cast<T>(input)));}},
            {"tan(", [](T input){return static_cast<T>(std::tan(static_cast<T>(input)));}},
            {"csec(", [](T input){return static_cast<T>(1.0 / std::sin(static_cast<T>(input)));}},
            {"sec(", [](T input){return static_cast<T>(1.0 / std::cos(static_cast<T>(input)));}},
            {"cot(", [](T input){return static_cast<T>(1) / std::tan(static_cast<T>(input));}},
            {"asin(", [](T input){return static_cast<T>(std::asin(static_cast<T>(input)));}},
            {"acos(", [](T input){return static_cast<T>(std::acos(static_cast<T>(input)));}},
            {"atan(", [](T input){return static_cast<T>(std::atan(static_cast<T>(input)));}},
            {"ln(", [](T input){return static_cast<T>(std::log(static_cast<T>(input)));}},
            {"log(", [](T input){return static_cast<T>(std::log10(static_cast<T>(input)));}},
            {"log2(", [](T input){return static_cast<T>(std::log(static_cast<T>(input)) / std::log(2));}},
            {"abs(", [](T input){return static_cast<T>(std::abs(static_cast<T>(input)));}},
            {"zeta(", [](T input){return static_cast<T>(Math::zeta(static_cast<std::complex<long double>>(input)));}}
        };

    //ln(Γ(x))=12ln(2π)+(x−12)ln(x)−x+x2ln(xsinh(1x)+1810x6)

    template<typename T>
    static std::unordered_map<std::string, std::function<T(T,T)>> binary_ops
        {
            {"+", [](T left, T right){return left + right;}},
            {"-", [](T left, T right){return left - right;}},
            {"*", [](T left, T right){return left * right;}},
            {"/", [](T left, T right){return left / right;}},
            {"^", [](T left, T right){return static_cast<T>(std::pow(static_cast<T>(left), static_cast<T>(right)));}},
        };

    constexpr static std::array<std::string, 6> basic_operators
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
        const std::string self;

        constexpr static bool is_any_bracket(const char &self)
        {
            return self == '(' || self == ')';
        }

        constexpr static bool is_r_bracket(const char &self)
        {
            return self == ')';
        }

        constexpr static bool is_l_bracket(const char &self)
        {
            return self == '(';
        }

        static bool matched_brackets(const std::string& expr)
        {
            int b = 0;
            std::for_each(std::execution::par_unseq, expr.begin(), expr.end(), [&b](const auto& c) {
                if (c == '(')
                    ++b;
                if (c == ')')
                    --b;
            });
            return b == 0;

        }

         constexpr static bool is_basic_operator(const char &self)
        {
            return std::ranges::any_of(basic_operators, [&](const auto& op) {
                if (self == op[0])
                    return true;
                return false;
            });
        }

         constexpr static bool is_basic_operator(const std::string &self)
        {
            return std::ranges::any_of(basic_operators, [&](const auto& op) {
                return self == op;
            });
        }

         constexpr static bool is_operator(const std::string &self)
        {
            return std::ranges::any_of(operators, [&](const auto& op){
                return self == op;
            });
        }

    public:
        explicit Token(std::string value) : self(std::move(value))
        {
            for (auto &it : self)
            {
                if (!is_valid_char(it))
                    throw std::invalid_argument("Invalid character detected\n");
            }
        }

        [[nodiscard]] constexpr const std::string &string_val() const
        {
            return self;
        }

        constexpr static bool is_valid_char(const char &c)
        {
            return is_numerical(c) ||
                   is_any_bracket(c) ||
                   is_basic_operator(c) ||
                   is_alpha(c);
        }

        constexpr static bool is_alpha(const char &c)
        {
            return (c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A');
        }

        [[nodiscard]] constexpr bool is_numerical() const
        {
            return std::ranges::none_of(self, [&](const auto& it) {
                return !((it > '9' || it < '0') && it != '.');
            });
        }

        constexpr static bool is_numerical(const char &c)
        {
            return (c <= '9' && c >= '0') || c == '.';
        }

        [[nodiscard]] constexpr bool is_operator() const
        {
            return std::ranges::any_of(
                    operators,
                    [&](const auto& op){
                        return self == op;
                    }
                    );
        }

        [[nodiscard]] constexpr int op_precedence() const
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

        [[nodiscard]] constexpr bool is_binary_op() const
        {
            return std::ranges::any_of(basic_operators,
                                       [&](const auto& op) {
                return op == self;
            });
        }

        [[nodiscard]] constexpr bool is_any_bracket() const
        {
            return self == "(" || self == ")";
        }

        [[nodiscard]] constexpr bool is_r_bracket() const
        {
            return self == ")";
        }

        [[nodiscard]] constexpr bool is_l_bracket() const
        {
            return self == "(";
        }

        [[nodiscard]] constexpr bool is_variable() const
        {
            return is_alpha(self.back());
        }

        [[nodiscard]] constexpr bool is_unary_func() const
        {
            return is_operator() && !is_binary_op();
        }

        [[nodiscard]] constexpr bool right_associate() const
        {
            return self == "^";
        }

        Token &operator=(const Token &a) = default;

        template <typename T>
        T function_eval(T input)//deleted &
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

        static std::vector<Token> tokenize(const std::string& expression)
        {
            if (!matched_brackets(expression))
                throw std::invalid_argument("Mismatched parentheses\n");

            std::string temp;
            std::vector<Token> out;

            auto it = expression.begin();
            while (it != expression.end())
            {

                if (*it < 33)
                    ++it;

                else if (is_r_bracket(*it))
                {
                    temp.push_back(*it);
                    out.emplace_back(temp);
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
                    out.emplace_back(temp);
                    temp.erase();
                }

                else if (*it == '-')
                {
                    if (it == expression.begin() || is_l_bracket(*(it - 1)) || is_basic_operator(*(it - 1))) // unary minus
                    {
                        if (!is_numerical(*(it + 1)) && !is_alpha(*(it + 1))) throw std::invalid_argument("Malformed expression");
                        temp.push_back(*it++);
                    }
                    else
                    {
                        temp.push_back(*it++);
                        out.emplace_back(temp);
                        temp.erase();
                    }
                }

                else if (is_basic_operator(*it))
                {
                    temp.push_back(*it++);
                    out.emplace_back(temp);
                    temp.erase();
                }

                else if (is_alpha(*it))
                {
                    if (it == expression.end() || (it + 1) == expression.end() || !is_alpha(*(it + 1))) // if variable
                    {
                        temp.push_back(*it++);
                        out.emplace_back(temp);
                        temp.erase();
                    }
                    else
                    {
                        temp.push_back(*it);
                        while (!is_l_bracket(*it++))
                        {
                            temp.push_back(*it);
                        }
                        out.emplace_back(temp);
                        temp.erase();
                    }
                }

                else if (is_l_bracket(*it))
                {
                    temp.push_back(*it);
                    out.emplace_back(temp);
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
        auto convert_to_rpn(const std::vector<Token> &expression)
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
                        !queue.empty() &&
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

            while (!queue.empty())
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
        explicit Expression(const std::string& expr)
        {
            variables = "";
            self = Token::tokenize(expr);
            self_rpn = ParsingShunt().convert_to_rpn(self);

            for (auto &t : self) // add variables to list
            {
                if (t.is_variable())
                {
                    variables.push_back(t.string_val().back());
                }
            }

            if(is_complex<T>())
            {
                unary_funcs<T>["real("] = [](T input){return std::real(input);};
                unary_funcs<T>["imag("] = [](T input){return std::imag(input);};
                unary_funcs<T>["arg("]  = [](T input){return std::arg(input);};
            } 

        }

        constexpr T evaluate(std::unordered_map<char, T> vars)
        {
            // Doesn't work unless you have complex type 'T' (i.e. cannot cast {0,1} to int for example)
            //if(is_complex<T>()) vars['i'] = {0,1}; //need to fix this or manually add 'i' to variable value list

            if(std::ranges::any_of(variables, [&](const auto& v) {
                return vars.find(v) == vars.end();
            })) throw std::invalid_argument("Missing variable value\n");

            
            std::vector<T> retval;


            auto it = self_rpn.begin();
            while (it != self_rpn.end())
            {
                if (!it->is_operator())
                {
                    if(it->is_variable())
                    {
                        const bool unary_minus = (it->string_val()[0] == '-');
                        retval.push_back(
                            (T)(unary_minus ? -1 : 1) * (vars[it->string_val().back()])
                        );
                    }
                    else retval.push_back(convert_to<T>(it->string_val())); //may not work with custom types
                }
                else
                {
                    if(it->is_unary_func())
                    {
                        if(retval.empty()) throw std::invalid_argument("Malformed expression");
                        retval.back() = it->function_eval(retval.back());
                    }
                    else
                    {
                        if(retval.size() <= 1) throw std::invalid_argument("Malformed expression");
                        retval[retval.size() - 2] = it->function_eval(retval[retval.size() - 2], retval.back());
                        retval.pop_back();
                    }
                }
                ++it;
            }
            if (retval.size() != 1) throw std::invalid_argument("Extra operator, malformed expression");
            return retval.back();
        }

        bool validate(std::unordered_map<char, T> vars)
        {
            try
            {
                evaluate(vars);
            }
            catch(std::invalid_argument& e)
            {
                return false;
            }
            return true;
        }

        void validate_with_except(std::unordered_map<char, T> vars)
        {
            evaluate(vars);
        }
    };

}


#endif
