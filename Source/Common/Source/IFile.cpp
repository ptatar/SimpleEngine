#include "IFile.hpp"

#include "File.hpp"

namespace engine
{

    IInputFilePtr CreateInputFile()
    {
        IInputFilePtr ptr = std::make_unique<InputFile>();
        return std::move(ptr);
    }

    IOutputFilePtr CreateOutputFile()
    {
        IOutputFilePtr ptr = std::make_unique<OutputFile>();
        return std::move(ptr);
    }

} // namespace engine
