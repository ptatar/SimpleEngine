#include "ShaderCooker.hpp"

#include "Path.hpp"
#include "FileUtility.hpp"
#include "Assert.hpp"

#include "shaderc/shaderc.hpp"
#include "spirv_hlsl.hpp"

#include <filesystem>
#include <cstring>

#include "Logger.hpp"

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
                ShaderBinary binary = ProcessFile(inputFile, outputFile);
                if (!binary.IsValid())
                {
                    return false;
                }
                SerializationBuffer buffer = Serialize(binary);
                if (!SaveFile(outputFile, reinterpret_cast<const Uint8*>(buffer.GetData()), buffer.GetSize()))
                {
                    return false;
                }
            }
        }
        return true;
    }

    bool ShaderCooker::ReadShader(const Path& inputPath)
    {
        auto fileContent = LoadFileVec(inputPath);
        if (fileContent.size())
        {
            SerializationFrame frame(fileContent.data(), fileContent.size());
            ShaderBinary binary = Deserialize<ShaderBinary>(frame);

            LOGI("Shader heade:");
            LOGI("Inputs:");
            for (auto& input: binary.m_header.GetInput())
            {
                LOGI("  name:     %s", input.m_name.c_str());
                LOGI("  location: %d", input.m_slot);
                LOGI("  type:     %s", input.m_type.ToString().GetCStr());
            }

            LOGI("Outputs:");
            for (auto& output: binary.m_header.GetOutput())
            {
                LOGI("  name:     %s", output.m_name.c_str());
                LOGI("  location: %d", output.m_slot);
                LOGI("  type:     %s", output.m_type.ToString().GetCStr());
            }

            LOGI("Constant:");
            for (auto& constant: binary.m_header.GetConstant())
            {
                LOGI("  name:    %s", constant.m_name.c_str());
                LOGI("  size:    %d", constant.m_size);
                LOGI("  binding: %d", constant.m_binding);

            }
            return true;
        }
        else
        {
            LOGE("Could not load file: %s", inputPath.GetCStr());
            return false;
        }
    }


    ShaderBinary ShaderCooker::ProcessFile(
        const Path& inputFile,
        const Path& outputFile)
    {
        LOGI("File %s | %s", inputFile.GetCStr(), outputFile.GetCStr());
        std::string source = LoadFile(inputFile);
        if (source.size() > 0)
        {
            ShaderType type = GetShaderType(inputFile);
            auto binary = CompileFile(source, inputFile.GetCStr(), type);
            if (binary.size())
            {
                ShaderHeader header = Reflect(binary);
                return {header, binary};
            }
            else
            {
                LOGE("Could not reflect shader binary: %s", inputFile.GetCStr());
                return {};
            }
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

    ShaderDataType GetDataType(const spirv_cross::SPIRType& spirType)
    {
        switch(spirType.basetype)
        {
            case spirv_cross::SPIRType::BaseType::Float:
                return ShaderDataType::Build(ShaderDataType::BaseType::Float, spirType.columns, spirType.vecsize);
            case spirv_cross::SPIRType::BaseType::Int:
                return ShaderDataType::Build(ShaderDataType::BaseType::Int, spirType.columns, spirType.vecsize);
            default:
                LOGE("ShaderCooker: invalid data type!");
        }
        return ShaderDataType::Invalid();
    }

    ShaderHeader ShaderCooker::Reflect(const std::vector<Uint32>& binary)
    {
        spirv_cross::CompilerGLSL compiler(binary.data(), binary.size());

        spirv_cross::ShaderResources resources = compiler.get_shader_resources();

        ShaderHeader header;

        for (auto& input: resources.stage_inputs)
        {
            Uint32 descriptorSet = compiler.get_decoration(input.id, spv::DecorationDescriptorSet);
            Uint32 location = compiler.get_decoration(input.id, spv::DecorationLocation);
            const std::string& name = input.name;
            auto& type = compiler.get_type(input.base_type_id);
            ShaderDataType dataType = GetDataType(type);

            header.GetInput().emplace_back(name, location, dataType);
        }

        for (auto& output: resources.stage_outputs)
        {
            Uint32 descriptorSet = compiler.get_decoration(output.id, spv::DecorationDescriptorSet);
            Uint32 location = compiler.get_decoration(output.id, spv::DecorationLocation);
            const std::string& name = output.name;
            auto& type = compiler.get_type(output.base_type_id);
            ShaderDataType dataType = GetDataType(type);

            header.GetOutput().emplace_back(name, location, dataType);
        }

        for (auto& uniform: resources.uniform_buffers)
        {
            Uint32 descriptorSet = compiler.get_decoration(uniform.id, spv::DecorationDescriptorSet);
            Uint32 binding = compiler.get_decoration(uniform.id, spv::DecorationBinding);
            //const std::string& name = uniform.name;
            const std::string& name = compiler.get_name(uniform.base_type_id);
            auto& type = compiler.get_type(uniform.base_type_id);

            header.GetConstant().emplace_back(name, binding);

            auto& members = header.GetConstant().back().m_members;

            Uint32 memberCount = type.member_types.size();
            for (Uint32 i = 0; i < memberCount; i++)
            {
                const std::string& name = compiler.get_member_name(type.self, i);
                Uint32 binding = compiler.get_member_decoration(type.self, i, spv::DecorationBinding);
                auto& memberType = compiler.get_type(type.member_types[i]);
                ShaderDataType dataType = GetDataType(memberType);
                Uint32 offset = compiler.get_member_decoration(type.self, i, spv::DecorationOffset);

                members.emplace_back(name, binding, dataType, offset);
            }
        }

        return header;
    }

} // namespace engine
