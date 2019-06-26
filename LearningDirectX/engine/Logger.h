#pragma once
//#include <varargs.h>
//#include <vadefs.h>

#include <stdio.h>
#include <windows.h>

#define LOG(X,Y) \
char *str = new char[1000]; \
sprintf_s(str, 1000, X, Y);\
OutputDebugString(str);\
delete str\

//class Logger
//{
//public:
//    Logger()
//    {
//        m_instance = this;
//    }
//    ~Logger();
//
//
//    template<typename... Args>		// note the "..."
//    void Log(const char* s,Args... args)	// note the "..."
//    {
//        while (s && *s) {
//            if (*s == '%' && *++s != '%') {	// a format specifier (ignore which one it is)
//                //std::cout << value;		// use first non-format argument
//                char buffer[256];
//
//                sprintf_s(buffer, "%d", args);
//                OutputDebugString(++s, args...); 	// ``peel off'' first argument
//                return;
//            }
//        }
//    }
//    static Logger* sharedLogger() { return m_instance; }
//private:
//    static Logger* m_instance;
//};
