#pragma once

#include "Types.hpp"
#include "String.hpp"

namespace engine
{

    class Exception
    {
        public:
            Exception() {}

            Exception(const String& message): m_message(message) {}

            ~Exception() {}

            const String& GetMessage() const { return m_message; }

        private:
            String m_message;
    }; // class Exception

    class InvalidArgument: public Exception
    {
        public:
            InvalidArgument(const String& message, Uint32 argumentIdx)
                : Exception(message)
                , m_argumentIdx(argumentIdx) {}

            Uint32 GetArgumentIdx() const { return m_argumentIdx; }

        private:
            Uint32 m_argumentIdx;
    };

    class Overflow: public Exception
    {
        public:
            Overflow(const String& message): Exception(message) {}
    };

} // namespace engine
