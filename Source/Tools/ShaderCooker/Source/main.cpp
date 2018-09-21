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


    const ArgumentMap& argumentMap = argumentParser.GetArgumentMap();

    {   
        const auto& valueMap = argumentMap.GetFullnameMap();
        std::cout << "Argument fullname provided: \n";
        for (auto iter = valueMap.begin(); iter != valueMap.end(); ++iter)
        {
            std::cout << iter->first << ", ";
        }
    }
    {
        const auto& valueMap = argumentMap.GetAbbreviationMap();
        std::cout << "\n\nArgument abbreviation provided: \n";
        for (auto iter = valueMap.begin(); iter != valueMap.end(); ++iter)
        {
            std::cout << iter->first << ", ";
        }
    }
    return 0;
}
