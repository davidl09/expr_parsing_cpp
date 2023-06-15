#include <string>
#include <vector>
#include <cmath>

namespace Parsing{

    const std::string brackets = "()";
    const std::string ops = "+-*/^";
    const std::string nums = "0123456789.";
    const std::string symbols("0123456789+-*/^.()");
    const std::string r_associate("^");


    size_t match_bracket(std::string& str, size_t index){ //assumes bracket parity is given
        
        size_t brackets = 1; //first bracket already counted
        if(str[index] == '('){
            for(int i = index + 1; i < str.size(); ++i){
                (str[i] == '(' ? ++brackets : (str[i] == ')' ? --brackets : brackets)); //change bracket count
                if(brackets == 0) //if match found return index
                    return i;
            }
            return std::string::npos;
        }
        if(str[index] == ')'){
            for(int i = index - 1; i >= 0; --i){
                (str[i] == ')' ? ++brackets : (str[i] == '(' ? --brackets : brackets)); //change bracket count
                if(brackets == 0) //if match found return index
                    return i;
            }
            return std::string::npos;
        }
        return std::string::npos;
    }

    bool is_numchar(char& c){
        return nums.find(c) != std::string::npos;
    }

    bool is_operator(char& c){
        return ops.find(c) != std::string::npos;
    }

    bool is_bracket(char& c){
        return c == '(' || c == ')';
    }

    bool right_associate(std::string& str){
        return r_associate.find(str) != std::string::npos;
    }

    int op_prec(std::string str){ //numbers: 0, +-: 1,  */: 2, ^: 3
        switch (str.length()) {
            case 1:
                if(str[0] == ops[0] || str[0] == ops[1])
                    return 1;
                if(str[0] == ops[2] || str[0] == ops[3])
                    return 2;
                if(str[0] == ops[4])
                    return 3;
                if(is_bracket(str[0]))
                    return 0;
            default:
                for(char& c : str){
                    if(nums.find(c) != std::string::npos)
                        return 0;
                }
                return -1; //error
        }
    }

    int str_charcount(std::string str, char c){
        int count = 0;
        for (char& ch : str) {
            if(c == ch){
                count++;
            }
        }
        return count;
    }

        class ShuntingStack{

        private:
            std::vector<std::string> stack;
            std::vector<std::string> output;
        public:
            bool is_empty(){
                return stack.size() == 0;
            }

            void push_stack(std::string str){
                if(str.length() == 0){
                    throw std::invalid_argument("Empty string passed to shunting yard algorithm\n");
                }if(is_operator(str.back())){
                    while(!is_empty() && is_operator(stack.back()[0]) && 
                                (
                                    (op_prec(stack.back()) > op_prec(str)) || 
                                    (op_prec(stack.back()) >= op_prec(str) && !right_associate(str))
                                )){
                        output.push_back(stack.back());
                        stack.pop_back();
                    }
                    stack.push_back(str);
                    return;
                }if(is_bracket(str[0])){
                    switch(str[0]){

                        case '(':
                            stack.push_back(str);
                            return;

                        case ')':
                            while(stack.back() != "("){

                                if(is_empty())
                                    throw std::invalid_argument("Mismatched brackets");
                                output.push_back(stack.back());
                                stack.pop_back();

                            }
                            stack.pop_back();

                    }
                    return;
                }
                output.push_back(str);
            }

            std::vector<std::string> result(){
                while(!is_empty()){
                    output.push_back(stack.back());
                    stack.pop_back();
                }
                return output;
            }

    };

    template<typename T>
    class ShuntingYard{

        private:
        std::vector<std::string> input;
        std::vector<std::string> stack;
        std::vector<std::string> out;
        T value;

        void toRPN(){
            ShuntingStack shunt;

            for(std::string& str : input){
                shunt.push_stack(str);
            }
            out = shunt.result();

        }

        void evaluate(){
            std::vector<T> retval;
            using std::pow;
            for(auto it = out.begin(); it != out.end(); ++it){

                if(!is_operator((*it).back())) //evaluate last element since unary minus would break this
                    if(str_charcount(*it, '.') == 1){
                        T temp_rat(std::stod(*it));
                        retval.push_back(temp_rat);
                    }
                    else {
                        T temp_rat = (T)std::stoi(*it);
                        retval.push_back(temp_rat);
                    }

                else if(retval.size() > 1){ 
                    switch((*it)[0]){
                        case '^':
                            retval[retval.size() - 2] = T(pow(retval[retval.size() - 2], retval.back()));
                            retval.pop_back();
                            break;
                        case '/':
                            retval[retval.size() - 2] /= retval[retval.size() - 1];
                            retval.pop_back(); 
                            break;
                        case '*':
                            retval[retval.size() - 2] *= retval[retval.size() - 1];
                            retval.pop_back(); 
                            break;
                        case '-':
                            retval[retval.size() - 2] -= retval[retval.size() - 1];
                            retval.pop_back();
                            break;
                        case '+':
                            retval[retval.size() - 2] += retval[retval.size() - 1];
                            retval.pop_back();
                            break;
                        default:
                            throw std::invalid_argument("Unknown symbol encountered\n");
                    }

                }
                
                else{
                    throw std::invalid_argument("Misformed expression, parsing failed\n");
                }
            }   
            value = retval[0];
        }



        public:

        ShuntingYard(std::string expr){
            if(expr.length() == 0)
                throw std::invalid_argument("Empty argument to parser\n");

            for(auto it = expr.begin(); it < expr.end(); ++it){
                if(*it == ' ')
                    expr.erase(it);
            }

            if(!is_valid_mstr(expr))
                throw std::invalid_argument("Malformed input string to parser\n");

            std::string temp;
            std::string::iterator it = expr.begin();
            while(it < expr.end()){
                if(is_operator(*it) || is_bracket(*it)){ //put operators/brackets into a single token
                    if(*it == '-'){
                        if(it == expr.begin() || is_operator(*(it - 1))){ //if '-' is unary minus and not subtraction
                            temp.push_back(*it);
                            it++;
                            while(is_numchar(*it) && it < expr.end()){ //put decimal numbers into one token
                                temp.push_back(*it);
                                ++it;
                            }
                        }
                    }else{
                        temp.push_back(*it);
                        ++it;
                    }
                    input.push_back(temp);
                    temp.erase();
                    
                }else{
                    while(is_numchar(*it) && it < expr.end()){ //put decimal numbers into one token
                        temp.push_back(*it);
                        ++it;
                    }
                    input.push_back(temp);
                    temp.erase();
                }
            }
        }


        

        
        bool is_valid_mstr(std::string& str){
            //check validity
            for(char& c : str){
                if(symbols.find(c) == std::string::npos){ //if this char is not in list of valid symbols
                    return false;
                }
            }

            for(int i = 0; i < str.length(); ++i){
                if(is_bracket(str[i]) && match_bracket(str, i) == std::string::npos){ //check mismatched brackets
                    return false;
                }
            }
            return true;
        }

        void compute(){
            toRPN();
            evaluate();
        }

        
        T getResult(){
            return value;
        }

        std::string getResult_s(){
            return value.tostr();
        }

        double getResult_lf(){
            return value.approx();
        }
    };

    template<typename T>
    T str_to_rational(const std::string& str){
        ShuntingYard<T> shunt(str);
        shunt.compute();
        return shunt.getResult();
    }
    
}
