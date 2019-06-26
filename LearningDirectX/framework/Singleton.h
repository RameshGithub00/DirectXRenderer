#pragma once
#include <vector>

namespace core {

    template<class T>
    class Singleton
    {

    private:
        static T* mPtr;
    public:

        Singleton() {}
        Singleton(Singleton&) {}
        Singleton& operator=(Singleton&) {}

        void create()
        {
            if (mPtr == nullptr)
            {
                mPtr = new T;
            }
        }

        static T* getPtr()
        {
            return mPtr;
        }
    };

    template<class T>
    T* Singleton<T>::mPtr = 0;
}