#include "ShaderVk.hpp"
#include "DeviceVk.hpp"
#include "Assert.hpp"

namespace engine
{

    std::string ShaderTypeToString(ShaderType type)
    {
        switch(type)
        {
            case ShaderType::Vertex:
                return "Vertex";
            case ShaderType::Pixel:
                return "Pixel";
            case ShaderType::Compute:
                return "Compute";
            case ShaderType::Geometry:
                return "Geometry";
            case ShaderType::Hull:
                return "Hull";
            case ShaderType::Domain:
                return "Domain";
            default:
                ASSERT(false);
                return "Vertex";
        }
    }

    ShaderVk::ShaderVk(DeviceVk* device, const std::string& name, ShaderType type)
        : m_device(device)
        , m_name(name)
        , m_type(type)
    {
        ASSERT(device);
    }

    ShaderVk::~ShaderVk()
    {
        m_device->DestroyShader(m_shader);
    }

} // namespace engine
