#include "ConfigImpl.hpp"

#include "File.hpp"
#include "Path.hpp"
#include "Logger.hpp"

namespace engine
{

    Bool ConfigImpl::Initialize(const Path& configFilePath)
    {
        InputFile file;
        if (!file.Open(configFilePath))
        {
            LOGE("Could not open config file:", configFilePath.GetCStr());
            return false;
        }

        std::string data;
        data.resize(file.GetSize());
        if (!file.Read(reinterpret_cast<Uint8*>(&data[0]), file.GetSize()))
        {
            LOGE("Could not read config file");
            return false;
        }


        if (m_document.Parse(data.c_str()).HasParseError())
        {
            return false;
        }

        return true;
    }


    Int32 ConfigImpl::GetInt32Value(const std::string& key)
    {
        ASSERT(m_document[key.c_str()].IsInt());
        return static_cast<Int32>(m_document[key.c_str()].GetInt());
    }

    Uint32 ConfigImpl::GetUint32Value(const std::string& key)
    {
        ASSERT(m_document[key.c_str()].IsUint());
        return static_cast<Uint32>(m_document[key.c_str()].GetUint());
    }

    std::string ConfigImpl::GetStringValue(const std::string& key)
    {
        ASSERT(m_document[key.c_str()].IsString());
        return m_document[key.c_str()].GetString();
    }

} // namespace engine
