#pragma once

#include "Types.hpp"

#include <string>

namespace engine
{

    class Path
    {
        public:
            static const char s_separator;

            Path();

            ~Path() {}

            Path(const std::string& path, bool isDirectory = false);

            Path(const Path& path);

            const std::string& GetStr() const;

            const char* GetCStr() const;

            Uint32 GetSize() const;

            bool IsValid() const;

            Path GetSubPath(Uint32 idx) const;

            std::string GetExtension() const;

            std::string GetDirectory() const;

            Bool IsDirectory() const;

            Path operator+(const Path& path) const;

        private:
            std::string m_path;
            bool m_valid;
    };

} // namespace engine
