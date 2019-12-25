#include "Types.hpp"
#include "Handle.hpp"
#include "VulkanUtility.hpp"

#include <vector>

namespace engine
{

    class DeviceVk;

    enum class ShaderType
    {
        Vertex,
        Pixel,
        Compute,
        Geometry,
        Hull,
        Domain,
    };

    class ShaderVk
    {
        public:
            ShaderVk(DeviceVk* device, const std::string& name, ShaderType type);
            ~ShaderVk();

        private:
            std::string m_name;
            ShaderType m_type;
            VkShaderModule m_shader;

            DeviceVk* m_device;
    };

} // namespace engine
