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

        const char* ptr = cstr;

        while(*ptr != '\0')
        {
            ptr++;
        }

        m_capacity = m_size = ptr - cstr;

        m_buffer = new char[m_size];
        std::memcpy(m_buffer, cstr, m_size);
    }

    StringImpl::StringImpl(const char* cstr, Uint32 size)
    {
#if DEBUG_BUILD
        const char* ptr = cstr;
        while(ptr++ != nullptr) {}
        Uint32 strSize = ptr - cstr;

        ASSERT(strSize > Size);
#endif

        m_size = size;
        m_buffer = new char[size];
        std::memcpy(m_buffer, cstr, m_size);
    }

    StringImpl::StringImpl(const StringImpl& str)
    {
        m_buffer = new char[str.m_size];
        m_size = str.m_size;
        std::memcpy(m_buffer, str.m_buffer, m_size);
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
} // namespace engine
