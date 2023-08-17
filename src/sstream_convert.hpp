#include <sstream>
#include <string>

template <typename T> 
T convert_to (const std::string &str)
{
    std::istringstream ss(str);
    T num;
    ss >> num;
    return num;
}
