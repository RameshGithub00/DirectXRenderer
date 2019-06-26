// Practice.cpp : Defines the entry point for the console application.
//
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory.h>

using namespace std;

class Base
{
public:
    virtual void Func() {}
};

template<class DERIVED_TYPE>
class ClassBase : public virtual Base
{
    DERIVED_TYPE* derived;
public:
    ClassBase(DERIVED_TYPE* der) :derived(der)
    {}

    void Func() 
    {
        derived->Func();
        cout << "\n Base::Func";
    }
};

class DerivedA :public ClassBase<DerivedA>
{
public:
    DerivedA() :ClassBase<DerivedA>(this) {}
    void Func() 
    {
        cout << "\n DerivedA::Func";
    }
};

class DerivedB :public ClassBase<DerivedB>
{
public:
    DerivedB() :ClassBase<DerivedB>(this) {}
    virtual void Func()
    {
        cout << "\n DerivedB::Func";
    }
};

class DerivedAB :public DerivedA, public DerivedB
{
public:
    void Func()
    {
        cout << "\n DerivedAB::Func";
    }
};

int main() 
{
    DerivedAB ab;
    Base* b = &ab;
    b->Func();
    return 0;
}

