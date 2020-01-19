#include "String.hpp"

#include "StringUtility.hpp"

#include <cstring>

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

        ASSERT(strSize > Size);
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

} // namespace engine
