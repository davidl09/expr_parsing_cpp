# expr_parsing_cpp

Create Parsing::Expression<T>() objects with a string representing a valid mathematical expression to parse this expression. Call the .evaluate({}) method on this object to calculate its value. The string may contain any decimal representation of numbers, any single character variables, as well as the following operators:
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
        "ln("
}

If the expression contains variables, pass a non-empty std::unordered_map<char, T> to the .evaluate({}) method: This will substitute in the values in place of the variables. Otherwise, pass an empty map by adding empty curly braces as the argument to .evalueate({}).
