#include "Array.hpp"
#include <vector>
#include <cstring>
#include <iostream>

int g_TestNumber = 10;

class Foo
{
    public:
    Foo(): m_var(0)
    {
        std::cout << "Foo()" << "\n";
    }
    Foo(int a, int b): m_var(a*b)
    {
        std::cout << "Foo(a, b)" << "\n";
    }

    ~Foo()
    {
        std::cout << "~Foo()" << "\n";
    }

    int m_var;
};

int VectorTest(int var)
{
    std::vector<int> vector(var);
    for(int i = 0; i < g_TestNumber; i++)
    {
        vector[i%var] += i;
    }
    int sum = 0;
    for(int i = 0; i < var; i++)
    {
        sum += vector[i];
    }
    return sum;
}

int ArrayTest(int var)
{
    engine::Array<int> vector(var);
    for(int i = 0; i < g_TestNumber; i++)
    {
        vector[i%var] += i;
    }
    int sum = 0;
    for(int i = 0; i < var; i++)
    {
        sum += vector[i];
    }
    return sum;
}

int VectorTest2(int var)
{
    std::vector<Foo> vector(var);
    for(int i = 0; i < g_TestNumber; i++)
    {
        vector.emplace(vector.begin() + i%var, i, i%var);
    }
    int sum = 0;
    for(int i = 0; i < var; i++)
    {
        sum += vector[i].m_var;
    }
    return sum;
}

int ArrayTest2(int var)
{
    engine::Array<Foo> vector(var);
    for(int i = 0; i < g_TestNumber; i++)
    {
        vector.Emplace(i%var, i, i%var);
    }
    int sum = 0;
    for(int i = 0; i < var; i++)
    {
        sum += vector[i].m_var;
    }
    return sum;
}

int main(int argc, char** argv)
{
    if(argc < 3)
    {
        std::cout << "test and size required" << std::endl; 
        return 0;
    }
    int sum = 0;
    int var = atoi(argv[2]);
    if(std::strcmp(argv[1], "array") == 0)
    {
        std::cout << "Array test for size: " << var << std::endl;
        sum = ArrayTest2(var);
    }
    else if (std::strcmp(argv[1], "vector") == 0)
    {
        std::cout << "Vector test for size: " << var << std::endl;
        sum = VectorTest2(var);
    }
    std::cout << sum << std::endl;
    return 0;
}
