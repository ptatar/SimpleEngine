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
        }
    }

    ShaderVk::ShaderVk(DeviceVk* device, const std::string& name, ShaderType type)
        : m_device(device)
        , m_name(name)
        , m_type(type)
    {
        ASSERT(device);
    }

    Bool ShaderVk::Initialize(const std::vector<Uint8>& binary)
    {
        m_shader = m_device->CreateShader(binary);
        if (!m_shader)
        {
            LOGE("%s shader initialization failed: %s", ShaderTypeToString(m_type), m_name);
            return false;
        }
        return true;
    }

} // namespace engine
