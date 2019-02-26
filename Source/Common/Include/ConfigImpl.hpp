#pragma once

#include "Config.hpp"

#include <rapidjson/document.h>

namespace engine
{

    class ConfigImpl: public Config
    {
        public:
            ConfigImpl() {}
            ~ConfigImpl() {}

            virtual Bool Initialize(const Path& configFilePath) override;

            virtual Int32 GetInt32Value(const std::string& key) override;

            virtual Uint32 GetUint32Value(const std::string& key) override;

            virtual std::string GetStringValue(const std::string& key) override;


        private:
            Bool ValidateConfig() const;

        private:
            rapidjson::Document m_document;

    };

} // namespace engine
