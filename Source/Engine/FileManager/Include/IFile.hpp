#pragma once

#include "Types.hpp"

namespace engine
{

    enum class FileMode: Uint32
    {
        None   = 0x00,
        Read   = 0x01,
        Write  = 0x02,
        Ate    = 0x04,
        Append = 0x08,
    };


    inline FileMode operator |(const FileMode& rs, const FileMode& ls)
    {
        return static_cast<FileMode>(static_cast<Uint32>(rs) | static_cast<Uint32>(ls));
    }


    inline FileMode operator &(const FileMode& rs, const FileMode& ls)
    {
        return static_cast<FileMode>(static_cast<Uint32>(rs) | static_cast<Uint32>(ls));
    }

    enum class Direction
    {
        Begin,
        Current,
        End,
    };

    class IInputFile
    {
        public:
        virtual ~IInputFile() {}

        virtual Bool Open(const std::string& name, FileMode mode) = 0;

        virtual Bool Read(Uint8* output, Uint32 bytes) = 0;

        virtual Uint32 GetSize() const  = 0;

        virtual Uint32 GetPosition() const = 0;

        virtual void SetPosition(Uint32 position, Direction direction = Direction::Begin) = 0;

        virtual const std::string& GetFileName() const = 0;

        virtual Bool Load() = 0;

        virtual void Unload() = 0;
    };

    class IOutputFile
    {
        public:
            virtual ~IOutputFile() {}

            virtual Bool Open(const std::string& name, FileMode fileMode) = 0;

            virtual Bool Write(const Uint8* input, Uint32 bytes) = 0;

            virtual Uint32 GetSize() const  = 0;

            virtual Uint32 GetPosition() const = 0;

            virtual void SetPosition(Uint32 position, Direction direction) = 0;

            virtual const std::string& GetFileName() const = 0;
    };

} // namespace engine
