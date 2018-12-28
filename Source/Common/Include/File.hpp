#pragma once

#include "IFile.hpp"

#include <fstream>
#include <vector>

namespace engine
{

    class FileManager;

    class InputFile: public IInputFile
    {
        public:
            InputFile()
                : m_position(0)
                , m_size(0)
                , m_mode(FileMode::None)
                , m_filePath()
                , m_buffer() {}

            virtual ~InputFile();

            virtual Bool Open(const Path& path, FileMode fileMode = FileMode::None) override;

            virtual Bool Read(Uint8* output, Uint32 bytes) override;

            virtual Uint32 GetSize() const override { return m_size; }

            virtual Uint32 GetPosition() const override { return m_position; }

            virtual void SetPosition(Uint32 position, Direction direction) override
            {

                m_position = position;
                m_file.seekg(m_position);
            };

            virtual const Path& GetFilePath() const override { return m_filePath; }

            virtual Bool Load() override;

            virtual void Unload() override;

            virtual const std::vector<Uint8>& GetBuffer() const { return m_buffer;}

        private:
            std::ifstream m_file;

            Uint32 m_position;

            Uint32 m_size;

            FileMode m_mode;

            Path m_filePath;

            std::vector<Uint8> m_buffer;

    };

    class OutputFile: public IOutputFile
    {
        public:
            OutputFile() {}

            virtual ~OutputFile() { m_file.close(); }

            virtual Bool Open(const Path& path, FileMode fileMode = FileMode::None) override;

            virtual Bool Write(const Uint8* input, Uint32 bytes) override;

            virtual Uint32 GetSize() const override { return m_size; }

            virtual Uint32 GetPosition() const override { return m_position; }

            virtual void SetPosition(Uint32 position, Direction direction) override
            {
                m_position = position;
                m_file.seekp(m_position);
            };

            virtual const Path& GetFilePath() const override { return m_filePath; }

        private:
            std::ofstream m_file;

            Uint32 m_position;

            Uint32 m_size;

            FileMode m_mode;

            Path m_filePath;
    };

} // namespace engine
