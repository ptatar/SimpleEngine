#pragma once

#include "Path.hpp"
#include "Types.hpp"

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

        private:
            std::vector<engine::Uint32> ProcessFile(const Path& inputFile,
                                                    const Path& outputFile);

            std::string LoadFile(const Path& filePath) const;

            std::vector<Uint32> CompileFile(const std::string& source,
                                            const std::string& fileName,
                                            ShaderType type) const;

            bool WriteOutputFile(const Path& path, const std::vector<Uint32>& binary);

            ShaderType GetShaderType(const Path& filePath) const;
    };


}
