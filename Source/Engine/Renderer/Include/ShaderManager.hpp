#include "Types.hpp"
#include "Path.hpp"
#include "ShaderVk.hpp"

#include <unordered_map>

namespace engine
{

    class DeviceVk;

    class ShaderManager
    {
        public:
            ShaderManager() {};
            ~ShaderManager() {};

            Bool Initialize();

        private:
            Bool CreateShaderCache(const Path& shaderPath);

        private:
            DeviceVk* m_device;
            std::unordered_map<std::string, ShaderVk> m_shaderMap;
    };

} // namespace engine
