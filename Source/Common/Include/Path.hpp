#pragma once

#include "Types.hpp"

#include <string>

namespace engine
{

    class Path
    {
        public:
            static char s_separator;

            Path();

            ~Path() {}

            Path(const std::string& path);

            Path(const Path& path);

            const std::string& GetStr() const;

            const char* GetCStr() const;

            Uint32 GetSize() const;

            bool IsValid() const;

            Path GetSubPath(Uint32 idx) const;

            std::string GetExtension() const;

            Path operator+(const Path& path);

        private:
            std::string m_path;
            bool m_valid;
    };

} // namespace engine
