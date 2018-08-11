#include "File.hpp"

#include "FileManager.hpp"

#include <cstring>

namespace engine
{

    InputFile::~InputFile()
    {
        if (m_file.is_open())
        {
            m_file.close();
        }

        m_fileManager->UnregisterFile(this);
    }

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

            m_fileName = name;

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
        if (m_buffer.size())
        {
            ASSERT(m_position < m_size);
            ASSERT(m_position + bytes < m_size);
            std::memcpy(output, m_buffer.data() + m_position, bytes);
            return true;
        }
        else
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
    }


    Bool InputFile::Load()
    {
        m_buffer.resize(m_size);
        Bool ret = Read(m_buffer.data(), m_size);
        if(ret)
        {
            LOGW("File loading failed. Clearing buffer");
            m_buffer.clear();
        }
        return ret;
    }

    void InputFile::Unload()
    {
        m_buffer.clear();
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

            m_fileName = name;
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
