#include "FileUtility.hpp"

#include "IFile.hpp"
#include "Logger.hpp"

namespace engine
{
    std::string LoadFile(const Path& filePath)
    {
        IInputFilePtr inputFile = CreateInputFile();
        inputFile->Open(filePath);
        std::string source;
        source.resize(inputFile->GetSize());
        if (inputFile->Read(reinterpret_cast<Uint8*>(&source[0]), inputFile->GetSize()))
        {
            return source;
        }
        else
        {
            return {};
        }
    }

    std::vector<Uint8> LoadFileVec(const Path& path)
    {
        IInputFilePtr inputFile = CreateInputFile();
        inputFile->Open(path);
        std::vector<Uint8> source;
        source.resize(inputFile->GetSize());
        if (inputFile->Read(reinterpret_cast<Uint8*>(&source[0]), inputFile->GetSize()))
        {
            return source;
        }
        else
        {
            return {};
        }
    }

    Bool SaveFile(const Path& path, const Uint8* data, Uint32 size)
    {
        IOutputFilePtr outputFile = CreateOutputFile();
        if (!outputFile->Open(path, FileMode::CreateD))
        {
            LOGE("Could not open output file: %s", path.GetCStr());
            return false;
        }

        if (!outputFile->Write(data, size))
        {
            LOGE("Output file write error: %s", path.GetCStr());
            return false;
        }
        return true;
    }

} // namespace engine
