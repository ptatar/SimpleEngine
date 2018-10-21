#include "ShaderCooker.hpp"

#include "Path.hpp"
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

bool ShaderCooker::Cook(const Path& input, const Path& output)
{
    for(auto& p: fs::recursive_directory_iterator(input.GetStr()))
    {
        fs::path path = p.path();
    }
    return true;
}
