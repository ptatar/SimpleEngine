#pragma once

#include "Types.hpp"

namespace engine
{

    class Path;

    class Config
    {
        public:
            static Config& GetInstance();
            Config() {}
            ~Config() {}

            virtual Bool Initialize(const Path& configFile) = 0;

            virtual Int32 GetInt32Value(const std::string& key) = 0;

            virtual Uint32 GetUint32Value(const std::string& key) = 0;

            virtual std::string GetStringValue(const std::string& key) = 0;

    };

}
