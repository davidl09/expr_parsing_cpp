# Math Expression Parser

## Introduction 
to-do

## Prerequisites:
- Install Conan*: `python3 -m pip install conan==1.60`
- Install CMake

\* Note that Conan is migrating to version 2, but this setup still uses version 1. Be sure to install version 1 as described above. 

## Building
```
mkdir build
cd build
conan install ..
conan build ..
```

## Testing 
The Conan build process will generate an `expressionParserTest` GTest executable that you can use to run the test suite. 

In the `/build` directory, run: 

```
./expressionParserTest
```
## Usage

Create a `Parsing::Expression<T>()` objects with a string representing a valid mathematical expression to parse this expression. Call the `.evaluate({})` method on this object to calculate its value. The string may contain any decimal representation of numbers, any single character variables, as well as the following operators:
`{
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
}`

If the expression contains variables, pass a non-empty `std::unordered_map<char, T>` to the `.evaluate({})` method: This will substitute in the values in place of the variables. Otherwise, pass an empty map by adding empty curly braces as the argument to `.evalueate({})`.
