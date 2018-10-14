#pragma once

#include <string>

class PathImpl;

class Path
{
    public:
        Path();

        ~Path() {}

        Path(const std::string& path);

        Path(const Path& path);

        const std::string& GetStr() const;

        bool IsValid() const;

    private:
        std::string m_path;
        bool m_valid;
};
