#include "ArgumentParser.hpp"
#include "ShaderCooker.hpp"
#include "Logger.hpp"
#include "Path.hpp"

#include <iostream>

const char* helpStr=
"ShaderCooker help:\n\
    --inputPath, -i  input directory for shader cooker\n\
    --outputPath, -o output directory for shader cooker\n\
    --help, -h       print help\n\
";

int main(int argc, char** argv)
{
    Arguments arguments(argc, argv);


    Path inputPath;
    Path outputPath;
    for(auto iter = ++arguments.Begin(); iter != arguments.End(); iter++)
    {
        if(ArgCmp(*iter, "--inputDir", "-i"))
        {
            inputPath = Path(*iter);
        }
        else if (ArgCmp(*iter, "--outputDir", "-o"))
        {
            outputPath = Path(*iter);
        }
        else if (ArgCmp(*iter, "--help", "-h"))
        {
            LOGI("%s", helpStr);
            return 0;
        }
        else
        {
            LOGE("Invalid argument: %s", (*iter).c_str());
            return -1;
        }
    }

    if (!inputPath.IsValid())
    {
        LOGE("Invalid input path: %s", inputPath.GetStr().c_str());
        return -1;
    }

    if (!outputPath.IsValid())
    {
        LOGE("Invalid output path: %s", inputPath.GetStr().c_str());
        return -1;
    }

    ShaderCooker shaderCooker;
    shaderCooker.Cook();

    return 0;
}
