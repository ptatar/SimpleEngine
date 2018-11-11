#pragma once

#include "Path.hpp"
#include "Types.hpp"

#include <vector>

namespace engine
{

    class ShaderCooker
    {
        public:
            ShaderCooker() {}
            ~ShaderCooker() {}
            bool Cook(const Path& input, const Path& output);

        private:
            std::vector<engine::Uint32> CompileToSpirv(const Path& inputFile,
                                                       const Path& outputFile);
    };


}
