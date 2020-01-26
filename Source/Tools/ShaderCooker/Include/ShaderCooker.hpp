#pragma once

#include "Path.hpp"
#include "Types.hpp"
#include "ShaderHeader.hpp"

#include <vector>

namespace engine
{
    enum class ShaderType
    {
        Vertex,
        Fragment,
        Geometry,
    };

    class ShaderCooker
    {
        public:
            ShaderCooker() {}

            ~ShaderCooker() {}

            bool Cook(const Path& input, const Path& output);

            bool ReadShader(const Path& inputPath);

        private:
            ShaderBinary ProcessFile(const Path& inputFile,
                                     const Path& outputFile);

            std::vector<Uint32> CompileFile(const std::string& source,
                                            const std::string& fileName,
                                            ShaderType type) const;

            ShaderType GetShaderType(const Path& filePath) const;

            ShaderHeader Reflect(const std::vector<Uint32>& binary);
    };


}
