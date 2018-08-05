#include "File.hpp"

namespace engine
{

    Bool InputFile::Open(const std::string& name, FileMode fileMode)
    {
        if (fileMode == FileMode::Ate)
        {
            m_file.open(name.c_str(), std::ios::binary | std::ios::ate);
        }
        else
        {
            m_file.open(name.c_str(), std::ios::binary);
        }

        if (m_file.is_open())
        {
            m_mode = fileMode;
            m_position = m_file.tellg();
            m_file.seekg(0, std::ios_base::end);
            m_size = m_file.tellg();
            m_file.seekg(m_position);
            return true;
        }
        else
        {
            LOGE("File open failed");
            return false;
        }
    }


    Bool InputFile::Read(Uint8* output, Uint32 bytes)
    {
        m_file.read(reinterpret_cast<char*>(output), bytes);
        m_position = m_file.tellg();

        if(m_file)
        {
            return true;
        }
        else
        {
            return false;
        }
    }


    Bool OutputFile::Open(const std::string& name, FileMode fileMode)
    {
        if (fileMode == FileMode::Ate)
        {
            m_file.open(name.c_str(), std::ios::binary | std::ios::ate);
        }
        else
        {
            m_file.open(name.c_str(), std::ios::binary);
        }

        if (m_file.is_open())
        {
            m_mode = fileMode;
            m_position = m_file.tellp();
            m_file.seekp(0, std::ios_base::end);
            m_size = m_file.tellp();
            m_file.seekp(m_position);
            return true;
        }
        else
        {
            LOGE("File open failed");
            return false;
        }
    }


    Bool OutputFile::Write(const Uint8* input, Uint32 bytes)
    {
        m_file.write(reinterpret_cast<const char*>(input), bytes);
        m_position = m_file.tellp();

        if (m_file)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

}
