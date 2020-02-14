#pragma once

#include "Types.hpp"
//#include "Exception.hpp"

#include <stdio.h>

namespace engine
{
    class StringImpl
    {
        public:
            StringImpl();

            StringImpl(const char* cstr);

            StringImpl(const char* cstr, Uint32 size);

            StringImpl(const StringImpl& str);

            StringImpl(StringImpl&& str);

            template<typename ...Args>
            static StringImpl Format(const char* format, Args... args);

            const char* GetCStr() const { return m_buffer; };

            Uint32 GetSize() const { return m_size; }

            Uint32 GetCapacity() const { return m_capacity; }

            Int64 ToInt64(Uint32 startPos, Uint32& endPos);

            StringImpl Strip() const;

            StringImpl operator+(const StringImpl& str) const;

            StringImpl& operator=(const char* str);

            StringImpl& operator=(const StringImpl& str);

            StringImpl& operator=(StringImpl&& str);

        private:
            void Alloc(Uint32 size);

            bool Realloc(Uint32 size);

            void Clear();

        private:

            char* m_buffer;
            Uint32 m_size;
            Uint32 m_capacity;

    }; // StringImpl

    typedef StringImpl String;


    namespace impl
    {
        StringImpl FormatImpl(const char* format, ...);
    } // namespace impl

    template<typename ...Args>
    StringImpl StringImpl::Format(const char* format, Args... args)
    {
        return impl::FormatImpl(format, args...);
    }


} // namespace engine
