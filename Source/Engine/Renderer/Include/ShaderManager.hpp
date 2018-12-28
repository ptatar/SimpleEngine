#include "Types.hpp"
#include "ShaderVk.hpp"

#include <unordered_map>

namespace engine
{

    class DeviceVk;

    class ShaderManager
    {
        public:
            ShaderManager();
            ~ShaderManager();

            Bool Initialize();

        private:
            DeviceVk* m_device;
            std::unordered_map<std::string, ShaderVk> m_shaderMap;
    };

} // namespace engine
