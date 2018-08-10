#pragma once

#include "IFile.hpp"

#include <fstream>
#include <vector>

namespace engine
{

    class InputFile: public IInputFile
    {
        public:
            InputFile() {}

            virtual ~InputFile() { m_file.close(); }

            virtual Bool Open(const std::string& name, FileMode fileMode) override;

            virtual Bool Read(Uint8* output, Uint32 bytes) override;

            virtual Uint32 GetSize() const override { return m_size; }

            virtual Uint32 GetPosition() const override { return m_position; }

            virtual void SetPosition(Uint32 position, Direction direction) override
            {

                m_position = position;
                m_file.seekg(m_position);
            };

            virtual const std::string& GetFileName() const override { return m_fileName; }

            virtual Bool Load() override;

            virtual void Unload() override;

        private:
            std::ifstream m_file;

            Uint32 m_position;

            Uint32 m_size;

            FileMode m_mode;

            std::string m_fileName;

            std::vector<Uint8> m_buffer;
    };

    class OutputFile: public IOutputFile
    {
        public:
            OutputFile() {}

            virtual ~OutputFile() { m_file.close(); }

            virtual Bool Open(const std::string& name, FileMode fileMode) override;

            virtual Bool Write(const Uint8* input, Uint32 bytes) override;

            virtual Uint32 GetSize() const override { return m_size; }

            virtual Uint32 GetPosition() const override { return m_position; }

            virtual void SetPosition(Uint32 position, Direction direction) override
            {
                m_position = position;
                m_file.seekp(m_position);
            };

            virtual const std::string& GetFileName() const override { return m_fileName; }

        private:
            std::ofstream m_file;

            Uint32 m_position;

            Uint32 m_size;

            FileMode m_mode;

            std::string m_fileName;
    };

} // namespace engine
