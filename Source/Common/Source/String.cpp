#include "String.hpp"

#include "StringUtility.hpp"
#include "Exception.hpp"

#include <cstring>
#include <cstdio>

namespace engine
{

    StringImpl::StringImpl()
        : m_size(0)
        , m_buffer(0)
        , m_capacity(0) {}

    StringImpl::StringImpl(const char* cstr)
    {
        m_size = StringLen(cstr);

        if (m_size == 0)
        {
            return;
        }

        Alloc(m_size + 1);

        // copy string and null termination
        std::memcpy(m_buffer, cstr, m_capacity);
    }

    StringImpl::StringImpl(const char* cstr, Uint32 size)
    {
#if DEBUG_BUILD
        Uint32 len = StringLen(cstr);
        ASSERT(len > size);
#endif

        m_size = size;
        Alloc(m_size + 1);

        std::memcpy(m_buffer, cstr, m_size);
        m_buffer[m_size] = '\0';
    }

    StringImpl::StringImpl(const StringImpl& str)
    {
        Alloc(str.m_capacity);
        m_size = str.m_size;

        std::memcpy(m_buffer, str.m_buffer, m_capacity);
    }

    StringImpl::StringImpl(StringImpl&& str)
    {
        m_buffer = str.m_buffer;
        str.m_buffer = nullptr;
        m_size = str.m_size;
        str.m_size = 0;
        m_capacity = str.m_capacity;
        str.m_capacity = 0;
    }

    Int64 StringImpl::ToInt64(Uint32 startPos, Uint32& endPos)
    {
        return StringToInt64(m_buffer, nullptr);
    }

    StringImpl StringImpl::Strip() const
    {
        if (m_buffer == nullptr)
        {
            return StringImpl();
        }

        const char* startPtr = m_buffer;
        while(*startPtr != '\0' && (*startPtr < 33 || *startPtr > 126))
        {
            startPtr++;
        }

        const char* endPtr = m_buffer + m_size;
        while(endPtr != startPtr && (*endPtr < 33 || *endPtr > 126))
        {
            endPtr--;
        }

        return StringImpl(startPtr, endPtr - startPtr);

    }

    String StringImpl::operator+(const String& other) const
    {
        StringImpl str;
        str.m_capacity = str.m_size = m_size + other.m_size;
        str.m_buffer = new char[str.m_size];
        std::memcpy(str.m_buffer, m_buffer, m_size);
        std::memcpy(str.m_buffer + m_size, other.m_buffer, other.m_size);

        return str;
    }

    StringImpl& StringImpl::operator=(const char* str)
    {
        // maybe new object????
        Uint32 strLen = StringLen(str);
        Clear();
        Alloc(strLen + 1);
        std::memcpy(m_buffer, str, m_capacity);
        return *this;
    }

    StringImpl& StringImpl::operator=(const StringImpl& str)
    {
        Clear();
        Alloc(str.m_size + 1);
        std::memcpy(m_buffer, str.m_buffer, m_capacity);

        return *this;
    }

    StringImpl& StringImpl::operator=(StringImpl&& str)
    {
        Clear();
        std::swap(m_size, str.m_size);
        std::swap(m_capacity, str.m_capacity);
        std::swap(m_buffer, str.m_buffer);
        return *this;
    }

    void StringImpl::Alloc(Uint32 size)
    {
        m_buffer = new char[size];
        m_capacity = size;
    }

    bool StringImpl::Realloc(Uint32 size)
    {
        char* new_buffer = reinterpret_cast<char*>(realloc(m_buffer, size));
        if (new_buffer != nullptr)
        {
            m_buffer = new_buffer;
            return true;
        }
        else
        {
            return false;
        }
    }

    void StringImpl::Clear()
    {
        if (m_buffer != nullptr)
        {
            delete[] m_buffer;
            m_size = 0;
            m_capacity = 0;
        }
    }

    namespace impl
    {
        #include <stdarg.h>

        StringImpl FormatImpl(const char* format, ...)
        {
            constexpr Uint32 bufferSize = 256;
            char buffer[256];

            va_list args;
            va_start(args, format);
            int ret = vsnprintf(buffer,256,format, args);
            va_end(args);
            if (ret < bufferSize && ret >= 0)
            {
                StringImpl str(buffer);
                return str;
            }
            else if (ret < 0)
            {
                throw InvalidArgument("Invalid format", 0);

                return StringImpl();
            }
            else
            {
                throw InvalidArgument("Buffer to small", 1);

                return StringImpl();
            }
        }
    } //namespace impl
} // namespace engine
