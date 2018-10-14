#include "Path.hpp"

Path::Path(): m_valid(false)
{
}

Path::Path(const std::string& path): m_path(path), m_valid(true)
{
    // TODO validation
}

const std::string& Path::GetStr() const
{
    return m_path;
}

bool Path::IsValid() const
{
    return m_valid;
}
