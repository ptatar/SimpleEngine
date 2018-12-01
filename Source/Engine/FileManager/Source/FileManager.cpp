#include "FileManager.hpp"

#include "File.hpp"

namespace engine
{

    ObjectRef<IInputFile> FileManager::OpenFile(const std::string& fileName)
    {
        //{
        //    std::lock_guard<std::mutex> guard(m_fileMapMx);
        //    auto iter = m_fileMap.find(fileName);
        //    if (iter != m_fileMap.end())
        //    {
        //        return iter->second;
        //    }
        //}
//
        //ObjectRef<IInputFile> file( new InputFile(this));
        //if (file->Open(fileName, FileMode::Read))
        //{
        //    RegisterFile(file);
        //    return file;
        //}
        //else
        //{
        //    LOGE("File open failed: %s", fileName.c_str());
        //    return ObjectRef<IInputFile>();
        //}
        return ObjectRef<IInputFile>();
    }


    void FileManager::RegisterFile(ObjectRef<IInputFile>& file)
    {
        //std::lock_guard<std::mutex> guard(m_fileMapMx);
        //m_fileMap[file->GetFileName()] = file;
    }


    void FileManager::UnregisterFile(IInputFile* file)
    {
        //std::lock_guard<std::mutex> guard(m_fileMapMx);
        //auto iter = m_fileMap.find(file->GetFileName());
        //if (iter != m_fileMap.end())
        //{
        //    m_fileMap.erase(iter);
        //}
    }

} // namespace engine
