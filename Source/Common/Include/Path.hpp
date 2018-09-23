#pragma once

class Path
{
    public:
        Path() {}

        ~Path() {}

        Path(const std::string& path): m_path(path) {}

    private:
        std::string m_path;
};
