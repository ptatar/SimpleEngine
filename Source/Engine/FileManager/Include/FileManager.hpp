#pragma once

#include "Types.hpp"
#include "IFile.hpp"

#include <unordered_map>

namespace engine
{

    class FileManager
    {
        public:
            FileManager() {}
            ~FileManager() {}

            ObjectRef<IInputFile> OpenFile(const std::string& fileName);

            void RegisterFile(IInputFile* file);
            void UnregisterFile(IInputFile* file);

        private:
            std::unordered_map<std::string, IInputFile*> m_fileMap;
    };

} // namespace engine
