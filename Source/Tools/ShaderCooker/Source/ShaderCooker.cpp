#include "ShaderCooker.hpp"

#include "Path.hpp"
#include "shaderc.hpp"

#include <filesystem>

namespace engine
{

    namespace fs = std::filesystem;

    bool ShaderCooker::Cook(const Path& input, const Path& output)
    {
        for(auto& p: fs::recursive_directory_iterator(input.GetStr()))
        {
            if (p.is_regular_file())
            {
                Path inputFile(p.path().c_str());
                Path outputFile = inputFile.GetSubPath(input.GetSize());
                std::vector<Uint32> binary = CompileToSpirv(inputFile, outputFile);
            }
        }
        return true;
    }


    std::vector<Uint32> ShaderCooker::CompileToSpirv(const Path& inputFile,
                                                     const Path& outputFile)
    {
        std::vector<Uint32> out;
        LOGI("File %s | %s", inputFile.GetCStr(), outputFile.GetCStr());
        return out;
    }


} // namespace engine
