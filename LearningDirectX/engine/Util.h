#pragma once
#include <sstream>
namespace core 
{
    class Util
    {
    public:
        template<class T>
        static const std::string getStr(const T number)
        {
            std::stringstream ss;
            ss << number;
            return ss.str();
        }
    };
}