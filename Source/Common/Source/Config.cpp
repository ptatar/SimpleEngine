#include "Config.hpp"

#include "ConfigImpl.hpp"

#include <memory>

namespace engine
{

    Config& Config::GetInstance()
    {
        static ConfigImpl config;

        return config;
    }

} // namespace engine
