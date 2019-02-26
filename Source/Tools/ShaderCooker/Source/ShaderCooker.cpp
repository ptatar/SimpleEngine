#include "ShaderCooker.hpp"

#include "Path.hpp"
#include "FileUtility.hpp"
#include "Assert.hpp"

#include "shaderc/shaderc.hpp"

#include <filesystem>
#include <cstring>

namespace engine
{

    namespace fs = std::filesystem;

    bool ShaderCooker::Cook(const Path& input, const Path& output)
    {
        for (auto& p: fs::recursive_directory_iterator(input.GetStr()))
        {
            if (p.is_regular_file())
            {
                Path inputFile(p.path().c_str());
                Path outputFile = inputFile.GetSubPath(input.GetSize());
                outputFile = output + outputFile;
                std::vector<Uint32> binary = ProcessFile(inputFile, outputFile);
                if (!binary.size())
                {
                    return false;
                }

                if (!SaveFile(outputFile, reinterpret_cast<const Uint8*>(binary.data()), binary.size() * sizeof(Uint32)))
                {
                    return false;
                }
            }
        }
        return true;
    }


    std::vector<Uint32> ShaderCooker::ProcessFile(
        const Path& inputFile,
        const Path& outputFile)
    {
        LOGI("File %s | %s", inputFile.GetCStr(), outputFile.GetCStr());
        std::string source = LoadFile(inputFile);
        if (source.size() > 0)
        {
            ShaderType type = GetShaderType(inputFile);
            std::vector<Uint32> binary = CompileFile(source, inputFile.GetCStr(), type);
            return binary;
        }
        else
        {
            LOGE("Could not read file: %s", inputFile.GetCStr());
            return {};
        }
    }



    std::vector<Uint32> ShaderCooker::CompileFile(
        const std::string& source,
        const std::string& fileName,
        ShaderType type) const
    {
        shaderc_shader_kind kind;
        switch(type)
        {
            case ShaderType::Vertex:
                kind =  shaderc_vertex_shader;
                break;
            case ShaderType::Fragment:
                kind = shaderc_fragment_shader;
                break;
            case ShaderType::Geometry:
                kind = shaderc_geometry_shader;
                break;
        }

        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        shaderc::SpvCompilationResult module =
            compiler.CompileGlslToSpv(source, kind, fileName.c_str(), options);

        if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
            LOGE("Shader compilation error file: %s\n%s", fileName.c_str(), module.GetErrorMessage().c_str());
            return {};
        }

        return {module.cbegin(), module.cend()};
    }

    ShaderType ShaderCooker::GetShaderType(const Path& path) const
    {
        std::string extension = path.GetExtension();
        if (std::strcmp(extension.c_str(), "vs") == 0)
        {
            return ShaderType::Vertex;
        }
        else if (std::strcmp(extension.c_str(), "fs") == 0)
        {
            return ShaderType::Fragment;
        }
        else if (std::strcmp(extension.c_str(), "gs") == 0)
        {
            return ShaderType::Geometry;
        }
        else
        {
            ASSERT(false);
            return ShaderType::Vertex;
        }
    }


} // namespace engine
