#include "Path.hpp"

namespace engine
{

#ifndef DIRECTORY_SEPARATOR
    #ifdef PLATFORM_WINDOWS
        char Path::s_separator = '\\';
    #elif PLATFORM_LINUX
        char Path::s_separator = '/';
    #else
        char Path::s_separator = '/';
    #endif
#endif

    Path::Path(): m_valid(false)
    {
    }


    Path::Path(const std::string& path): m_path(path), m_valid(true)
    {
        // TODO validation
        for (int i = 0; i < m_path.size(); ++i)
        {
            if (s_separator != '/')
            {
                if (m_path[i] == '/')
                {
                    m_path[i] = s_separator;
                }
            }
            else if (s_separator != '\\')
            {
                if (m_path[i] == '\\')
                {
                    m_path[i] = s_separator;
                }
            }
        }
    }


    const std::string& Path::GetStr() const
    {
        return m_path;
    }


    const char* Path::GetCStr() const
    {
        return m_path.c_str();
    }


    Uint32 Path::GetSize() const
    {
        return m_path.size();
    }


    bool Path::IsValid() const
    {
        return m_valid;
    }


    Path Path::GetSubPath(Uint32 idx) const
    {
        Path out;
        out.m_path = m_path.substr(idx);
        return out;
    }


    Path Path::operator+(const Path& path)
    {
        Path out;
        if (m_path[m_path.size() - 1] == s_separator ||
            path.m_path[path.GetSize()] == s_separator)
        {
            out.m_path = m_path + path.m_path;
        }
        else
        {
            out.m_path = m_path + s_separator + path.m_path;
        }

        return out;
    }


}
