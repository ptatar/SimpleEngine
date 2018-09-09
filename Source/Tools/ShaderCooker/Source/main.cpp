#include "ArgumentParser.hpp"
#include "ShaderCooker.hpp"

#include <iostream>

int main(int argc, char** argv)
{
    Arguments arguments(argc, argv);
    ArgumentParser argumentParser;
    argumentParser.AddArgument(
            "input",
            "i",
            "Input directory.",
            ValueType::Path);
    argumentParser.AddArgument(
            "output",
            "o",
            "Output directory.",
            ValueType::Path);

    bool ret = argumentParser.Parse(arguments);
    if (!ret)
    {
        std::cout << "Arguments parsing error..." << std::endl;
    }
    for(auto iter = arguments.Begin(); iter != arguments.End(); iter++)
    {
        
    }
    return 0;
}
