#pragma once

#include "Types.hpp"
#include "IFile.hpp"

#include <unordered_map>
#include <mutex>

namespace engine
{

    class FileManager
    {
        public:
            FileManager() {}
            ~FileManager() {}

            ObjectRef<IInputFile> OpenFile(const std::string& fileName);

            void RegisterFile(ObjectRef<IInputFile>& file);
            void UnregisterFile(IInputFile* file);

        private:
            std::unordered_map<std::string, ObjectRef<IInputFile>> m_fileMap;
            std::mutex m_fileMapMx;
    };

} // namespace engine
