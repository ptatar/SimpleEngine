#include <iostream>
#include <cstring>

int main(int argc, char** argv)
{
    if (argc < 2)
        return 0;

    if (std::strcmp(argv[1], "ref") == 0)
    {
        std::cout << RefTest() << std::endl;
    }
    else if (std::strcmp(argv, "ref") == 0)
    {
        std::cout << PtrTest() << std::endl;
    }
    else if (std::strcmp(argv, "ref") == 0)
    {
        std::cout << Test() << std::endl;
    }

    return 0;
}
