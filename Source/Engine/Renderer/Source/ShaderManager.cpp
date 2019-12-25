#include "ShaderManager.hpp"

#include "Config.hpp"
#include "FileUtility.hpp"
#include "DeviceVk.hpp"

#include <filesystem>

namespace engine
{

    namespace fs = std::filesystem;

    Bool ShaderManager::Initialize(DeviceVk* device)
    {
        m_device = device;
        Config& config = Config::GetInstance();
        std::string shaderPath = config.GetStringValue("shader_directory");

        return true;
    };

    Bool ShaderManager::CreateShaderCache(const Path& shaderDirPath)
    {
        ASSERT(m_device);
        // for now preload will be good enough
        for (auto& p: fs::recursive_directory_iterator(shaderDirPath.GetStr()))
        {
            if (p.is_regular_file())
            {
                Path shaderFilePath(p.path().c_str());
                std::vector<Uint8> fileData = LoadFileVec(shaderFilePath);
                m_device->CreateShader(fileData);
            }
        }
        return true;
    }

} // namespace engine
