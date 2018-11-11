#include "ArgumentParser.hpp"
#include "ShaderCooker.hpp"
#include "Logger.hpp"
#include "Path.hpp"

#include <iostream>

using namespace engine;

const char* helpStr=
"ShaderCooker help:\n\
    --inputPath, -i  input directory for shader cooker\n\
    --outputPath, -o output directory for shader cooker\n\
    --verbose, -v log more information\n\
    --help, -h       print help\n\
";

int main(int argc, char** argv)
{
    Arguments arguments(argc, argv);

    Path inputPath;
    Path outputPath;
    bool verbose = false;
    for(auto iter = ++arguments.Begin(); iter != arguments.End(); iter++)
    {
        if(ArgCmp(*iter, "--inputPath", "-i"))
        {
            iter++;
            if (iter != arguments.End())
            {
                inputPath = Path(*iter);
            }
        }
        else if (ArgCmp(*iter, "--outputPath", "-o"))
        {
            iter++;
            if (iter != arguments.End())
            {
                outputPath = Path(*iter);
            }
        }
        else if (ArgCmp(*iter, "--verbose", "-v"))
        {
            verbose = true;
            LOG_SET_THRESHOLD(LOG_THRESHOLD_DEBUG);
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

    LOGI("Running shader cooker\n\tinputPath = %s\n\toutputPath = %s", inputPath.GetCStr(), outputPath.GetCStr());

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
    shaderCooker.Cook(inputPath, outputPath);

    return 0;
}
